/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INI_STOP_ON_FIRST_ERROR 0 /* Ignore unknown keys */

#include "ini.h"

struct Request
{
    uint64_t not_after;
    char* socket;
    char* message;
    char* icon;
    pid_t pid;
    bool accept_cached;
    bool echo;
    bool silent;
};

void Request_init(struct Request* req)
{
    req->not_after = 0;
    req->socket = NULL;
    req->message = NULL;
    req->icon = NULL;
    req->pid = 0;
    req->accept_cached = false;
    req->echo = false;
    req->silent = false;
}

void Request_free(struct Request* req)
{
    if (req->socket)
        free(req->socket);
    if (req->message)
        free(req->message);
    if (req->icon)
        free(req->icon);
}

void Request_reset(struct Request* req)
{
    Request_free(req);
    Request_init(req);
}

struct Request request;

static int send_password(const char *password) {
        int socket_fd = socket(AF_UNIX, SOCK_DGRAM|SOCK_CLOEXEC, 0);
        if (socket_fd < 0) {
            perror("socket() is failed");
            return errno;
        }

        struct sockaddr_un address;
        address.sun_family = AF_UNIX;
        strncpy(address.sun_path, request.socket, sizeof(address.sun_path) - 1);

        ssize_t n = sendto(socket_fd, password, strlen(password), MSG_NOSIGNAL, (const struct sockaddr*) &address, sizeof(address));
        if (n < 0) {
            perror("sendto() is failed");
            int error = errno;
            (void) close(socket_fd);
            return error;
        }

        return 0;
}

static bool to_bool(const char* value)
{
    if (strcmp(value, "true") == 0) {
        return true;
    } else if (strcmp(value, "1") == 0) {
        return true;
    } else if (strcmp(value, "yes") == 0) {
        return true;
    } else if (strcmp(value, "false") == 0) {
        return false;
    } else if (strcmp(value, "0") == 0) {
        return false;
    } else if (strcmp(value, "no") == 0) {
        return false;
    } else if (strcmp(value, "") == 0) {
        return false;
    } else {
        fprintf(stderr, "The value '%s' is not a boolean", value);
        return false;
    }
}

static int ini_parser(void* user, const char* section, const char* name, const char* value)
{
    struct Request* d = (struct Request*) user;

    if (strcmp(section, "Ask") != 0) {
        fprintf(stderr, "The ini file contains unknown section: %s", section);
        return 0;
    }

    if (strcmp(name, "NotAfter") == 0) {
        d->not_after = atol(value);
    } else if (strcmp(name, "Socket") == 0) {
        d->socket = strdup(value);
    } else if (strcmp(name, "Message") == 0) {
        d->message = strdup(value);
    } else if (strcmp(name, "Icon") == 0) {
        d->icon = strdup(value);
    } else if (strcmp(name, "PID") == 0) {
        d->pid = atoi(value);
    } else if (strcmp(name, "AcceptCached") == 0) {
        d->accept_cached = to_bool(value);
    } else if (strcmp(name, "Echo") == 0) {
        d->echo = to_bool(value);
    } else if (strcmp(name, "Silent") == 0) {
        d->silent = to_bool(value);
    } else {
        fprintf(stderr, "The ini file contains unknown key: %s", name);
        return 0;
    }

    return 1;
}

int find_request(char** ret_file)
{
    const char* ask_folder = "/run/systemd/ask-password";
    const size_t ask_folder_length = strlen(ask_folder);

    DIR* dir = opendir(ask_folder);
    if (!dir) {
        if (errno != ENOENT) {
            fprintf(stderr, "Can\'t open '%s': %s", ask_folder, strerror(errno));
        }
        return errno;
    }

    struct dirent* entry;
    while (entry = readdir(dir)) {
        if (entry->d_type != DT_REG)
            continue;

        if (strncmp(entry->d_name, "ask.", 4) != 0)
            continue;

        break;
    }
    if (!entry) {
        closedir(dir);
        return ENOENT;
    }

    char* file = malloc(ask_folder_length + 1 + strlen(entry->d_name) + 1);
    if (!file) {
        closedir(dir);
        fprintf(stderr, "Out of memory");
        return ENOMEM;
    }

    strcpy(file, ask_folder);
    strcpy(file + ask_folder_length, "/");
    strcpy(file + ask_folder_length + 1, entry->d_name);

    closedir(dir);

    *ret_file = file;
    return 0;
}

static int exec_unl0kr(char** ret_password)
{
    int fd[2];
    if (pipe(fd) != 0) {
        perror("Can't create a pipe");
        return errno;
    }

    pid_t pid = fork();
    switch (pid) {
    case -1: {
        perror("fork() is failed");
        int error = errno;
        (void) close(fd[0]);
        (void) close(fd[1]);
        return error;
    }

    case 0: { /* Child */
        (void) close(fd[0]);

        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2() is failed");
            return errno;
        }

        execl("UNL0KR_BINARY", "unl0kr", (char*) 0);

        perror("exec() is failed");
        return errno;
    }

    default: { /* Parent */
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid() is failed");
            int error = errno;
            (void) close(fd[0]);
            (void) close(fd[1]);
            return error;
        }

        if (!WIFEXITED(status)) {
            fprintf(stderr, "unl0kr terminated abnormally");
            (void) close(fd[0]);
            (void) close(fd[1]);
            return ECHILD;
        }

        int password_size;
        if (ioctl(fd[0], FIONREAD, &password_size) == -1) {
            perror("ioctl() is failed");
            int error = errno;
            (void) close(fd[0]);
            (void) close(fd[1]);
            return error;
        }

        char* password = malloc(password_size + 1);
        if (!password) {
            fprintf(stderr, "Out of memory");
            (void) close(fd[0]);
            (void) close(fd[1]);
            return ENOMEM;
        }

        password[0] = '+';
        password_size = read(fd[0], password + 1, password_size);
        if (password_size == -1) {
            perror("read() is failed");
            int error = errno;
            (void) close(fd[0]);
            (void) close(fd[1]);
            free(password);
            return error;
        }

        *ret_password = password;
        return 0;
    }}
}

int main(/* int argc, char* argv[] */)
{
    Request_init(&request);

    int exit_code = EXIT_SUCCESS;
    int r;

    for (;;) {
        char* file;
        r = find_request(&file);
        if (r != 0) {
            if (r != ENOENT)
                exit_code = EXIT_FAILURE;
            break;
        }

        // start_tracking_file(file);

        Request_reset(&request);

        r = ini_parse(file, ini_parser, &request);
        if (r < 0) {
            fprintf(stderr, "The file '%s' can not be parsed: %d", file, r);
            free(file);
            exit_code = EXIT_FAILURE;
            break;
        }

        free(file);

        char* password;
        r = exec_unl0kr(&password);
        if (r != 0) {
            exit_code = EXIT_FAILURE;
            break;
        }

        r = send_password(password);
        if (r != 0) {
            exit_code = EXIT_FAILURE;
            break;
        }

        // wait_for_file_removed();
        sleep(1000);
    }

    Request_free(&request);
    return exit_code;
}
