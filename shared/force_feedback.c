/**
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "force_feedback.h"

#include "log.h"

#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <libudev.h>
#include <stdint.h>
#include <unistd.h>

#define bit_is_set(array, bit)  ((array[bit / 8] >> bit % 8) & 0x01)

/**
 * Static variables
 */

static int fd = -1;
static struct input_event ff_event = {
    .type = EV_FF,
    .code = 0,
    .value = 1
};

/**
 * Public functions
 */

void bbx_force_feedback_connect(struct udev_device* device) {
    if (fd >= 0)
        return;

    const char* node = udev_device_get_devnode(device);

    int fd_event = open(node, O_RDWR);
    if (fd_event < 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Can't open %s", node);
        return;
    }

    int n_effects;
    if (ioctl(fd_event, EVIOCGEFFECTS, &n_effects) == -1 || n_effects == 0) {
        close(fd_event);
        return;
    }

    bbx_log(BBX_LOG_LEVEL_VERBOSE, "Found a force feedback device: %s", node);

    uint8_t features[(FF_CNT + 7) / 8];
    if (ioctl(fd_event, EVIOCGBIT(EV_FF, sizeof(features)), features) == -1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Can't request features of %s", node);
        close(fd_event);
        return;
    }

    if (!bit_is_set(features, FF_RUMBLE)) {
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "%s does not support FF_RUMBLE", node);
        close(fd_event);
        return;
    }

    struct ff_effect effect = {
        .type = FF_RUMBLE,
	.id = -1,
	.direction = 0x0000,
	.trigger.button = 0,
	.trigger.interval = 0,
	.replay.length = 50,
	.replay.delay = 0,
	.u.rumble.strong_magnitude = 0xC000,
	.u.rumble.weak_magnitude = 0xC000
    };

    if (ioctl(fd_event, EVIOCSFF, &effect) == -1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Can't upload the effect");
        close(fd_event);
        return;
    }

    ff_event.code = effect.id;

    if (bit_is_set(features, FF_GAIN)) {
        struct input_event gain = {
            .type = EV_FF,
            .code = FF_GAIN,
            .value = 0xFFFF
        };

        if (write(fd_event, &gain, sizeof(gain)) != sizeof(gain))
            bbx_log(BBX_LOG_LEVEL_WARNING, "Can't set the gain");
    }

    fd = fd_event;
}

void bbx_force_feedback_play() {
    if (fd < 0)
        return;

    if (write(fd, &ff_event, sizeof(ff_event)) != sizeof(ff_event)) {
        // Assume that the force feedback device was disconnected
        close(fd);
        fd = -1;
    }
}
