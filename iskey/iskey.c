// SPDX-License-Identifier: GPL-3.0-or-later

#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev.h>

#define MAX_KEYS 32

static int codes[MAX_KEYS] = { 0 };
static unsigned int types[MAX_KEYS] = { 0 };
static int debug = 0;
static int print_key = 0;

#define log(fmt, ...) \
	do { \
		if (debug) \
			fprintf(stderr, fmt, ##__VA_ARGS__); \
	} while (0)

#define INPUT_DIR "/dev/input/"

int usage(const char *name)
{
	fprintf(stderr, "Usage: %s [-d] [-s] key1...\n", name);
	fprintf(stderr, "  -d: debug mode\n");
	fprintf(stderr, "  -s: print the name of the key\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  Check if any input device has the specified keys pressed\n"
			"  and exit with code 0 if so, otherwise exit with code 1\n");
	return 1;
}

/* Check if any of the input devices have a non-zero value for any of the keys.
 * if so then return 0, otherwise return -1.
 */
int check_for_keys(struct libevdev *dev)
{
	int i;

	for (i = 0; i < MAX_KEYS; i++) {
		if (codes[i] == 0)
			break;
		if (!libevdev_has_event_code(dev, types[i], codes[i]))
			continue;
		if (libevdev_get_event_value(dev, types[i], codes[i])) {
			if (print_key)
				printf("%s\n", libevdev_event_code_get_name(types[i], codes[i]));
			return 0;
		}
	}

	return -1;
}

int main(int argc, char *argv[])
{
	struct libevdev *dev;
	struct dirent *dir;
	DIR *d;
	int i = 0, opt, fd;
	char path[64] = { 0 };

	/* getopt */
	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch (opt) {
		case 'd':
			debug = 1;
			break;
		case 's':
			print_key = 1;
			break;
		default:
			return usage(argv[0]);
		}
	}

	for (; optind < argc && i < MAX_KEYS; optind++) {
		codes[i] = libevdev_event_code_from_code_name(argv[optind]);
		if (codes[i] == -1) {
			fprintf(stderr, "Unknown key %s\n", argv[optind]);
			return 1;
		}
		types[i++] = libevdev_event_type_from_code_name(argv[optind]);
		log("Checking for %s %s (%d)\n", libevdev_event_type_get_name(types[i-1]), argv[optind], codes[i-1]);
	}

	d = opendir(INPUT_DIR);
	if (!d) {
		perror("couldn't open /dev/input/");
		return 1;
	}

	/* Walk through the entries in /dev/input */
	while ((dir = readdir(d)) != NULL) {
		memset(path, 0, sizeof(path));
		i = snprintf(path, sizeof(path), "%s%s", INPUT_DIR, dir->d_name);
		if (i < 0 || i >= sizeof(path)) {
			printf("Path '%s' too long\n", dir->d_name);
			return 1;
		}

		if (dir->d_type != DT_CHR || strncmp("event", dir->d_name, 5) != 0)
			continue;

		fd = open(path, O_RDONLY|O_NONBLOCK);
		if (fd < 0) {
			log("couldn't open device %s\n", dir->d_name);
			continue;
		}
		fd = libevdev_new_from_fd(fd, &dev);
		if (fd < 0) {
			log("couldn't init libevdev for %s: %s\n", dir->d_name, strerror(-fd));
			continue;
		}
		log("Checking device %s\n", libevdev_get_name(dev));
		if (!check_for_keys(dev)) {
			close(fd);
			return 0;
		}
		close(fd);
	}
	closedir(d);

	return 1;
}
