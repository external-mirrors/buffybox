/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <ini.h>

struct Request
{
    uint64_t not_after;
    char* file;
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
    req->file = NULL;
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
    if (req->file)
        free(req->file);
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
int fd_epoll, fd_inotify;
pid_t pid_unl0kr;
bool unl0kr_exited;

timer_t id_timer;
enum {
    NO_ACTION,
    TERMINATE_UNL0KR,
    KILL_UNL0KR
} timer_action;
bool timer_expired;

void erase_and_free(char* p)
{
    const size_t length = strlen(p);
    for (size_t i = 0; i < length; i++)
        p[i] = 0;

    free(p);
}

int send_password(const char *password)
{
        int fd_socket = socket(AF_UNIX, SOCK_DGRAM|SOCK_CLOEXEC, 0);
        if (fd_socket < 0) {
            int ret = errno;
            perror("socket() is failed");
            return ret;
        }

        struct sockaddr_un address;
        address.sun_family = AF_UNIX;
        strncpy(address.sun_path, request.socket, sizeof(address.sun_path) - 1);
        address.sun_path[sizeof(address.sun_path) - 1] = 0;

        ssize_t n = sendto(fd_socket, password, strlen(password), MSG_NOSIGNAL, (const struct sockaddr*) &address, sizeof(address));
        if (n < 0) {
            int ret = errno;
            perror("sendto() is failed");
            close(fd_socket);
            return ret;
        }

        close(fd_socket);
        return 0;
}

bool to_bool(const char* value)
{
    if (strcmp(value, "true") == 0) {
        return true;
    } else if (strcmp(value, "false") == 0) {
        return false;
    } else if (strcmp(value, "1") == 0) {
        return true;
    } else if (strcmp(value, "0") == 0) {
        return false;
    } else if (strcmp(value, "yes") == 0) {
        return true;
    } else if (strcmp(value, "no") == 0) {
        return false;
    } else {
        fprintf(stderr, "The value '%s' is not a boolean\n", value);
        return false;
    }
}

int ini_parser(void* user, const char* section, const char* name, const char* value)
{
    struct Request* d = (struct Request*) user;

    if (strcmp(section, "Ask") != 0) {
        fprintf(stderr, "The ini file contains unknown section: %s\n", section);
        return 0;
    }

    if (value[0] == 0x00) {
        fprintf(stderr, "The ini file contains a key without a value: %s\n", name);
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
        fprintf(stderr, "The ini file contains unknown key: %s = %s\n", name, value);
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
        int ret = errno;
        if (errno != ENOENT) {
            fprintf(stderr, "Can't open '%s': %s\n", ask_folder, strerror(errno));
        }
        return ret;
    }

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type != DT_REG && entry->d_type != DT_LNK)
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
        fprintf(stderr, "Out of memory\n");
        return ENOMEM;
    }

    strcpy(file, ask_folder);
    strcpy(file + ask_folder_length, "/");
    strcpy(file + ask_folder_length + 1, entry->d_name);

    closedir(dir);

    *ret_file = file;
    return 0;
}

int process_inotify_events()
{
    /* We expect only IN_DELETE_SELF and IN_IGNORED */
    size_t buffer_size = sizeof(struct inotify_event) * 2;
    uint8_t buffer[buffer_size];

    ssize_t block_size = read(fd_inotify, buffer, buffer_size);
    if (block_size < 0) {
        int ret = errno;
        perror("read() is failed");
        return ret;
    }

    assert((size_t) block_size == buffer_size);

    struct inotify_event* ievent1 = (struct inotify_event*) buffer;
    struct inotify_event* ievent2 = ievent1 + 1;

    assert(ievent1->mask & IN_DELETE_SELF);
    assert(ievent2->mask & IN_IGNORED);

    assert(read(fd_inotify, buffer, buffer_size) == -1 && errno == EAGAIN); // no more events
    return 0;
}

void sigalarm(int signo, siginfo_t *info, void *context)
{
    assert(signo == SIGALRM);
    (void)(context);

    if (info->si_code == SI_TIMER) {
        assert(info->si_value.sival_ptr == &id_timer);
        timer_expired = true;
    }

    switch (timer_action) {
    case TERMINATE_UNL0KR:
        if (kill(pid_unl0kr, SIGTERM) == 0) {
            struct itimerspec spec;
            spec.it_interval.tv_sec = 0;
            spec.it_interval.tv_nsec = 0;
            spec.it_value.tv_sec = 5;
            spec.it_value.tv_nsec = 0;
            timer_settime(id_timer, 0, &spec, NULL);
            timer_action = KILL_UNL0KR;
        }
        break;
    case KILL_UNL0KR:
        kill(pid_unl0kr, SIGKILL);
        break;
    default:
        break;
    }
}

void sigchild(int signo, siginfo_t *info, void *context)
{
    assert(signo == SIGCHLD);
    (void)(context);

    if (pid_unl0kr == 0)
        return;

    assert(info->si_pid == pid_unl0kr);

    if (info->si_code == CLD_EXITED ||
        info->si_code == CLD_KILLED ||
        info->si_code == CLD_DUMPED ) {
        unl0kr_exited = true;
    }
}

int event_loop(pid_t pid)
{
    int ret = 0;
    int r;

    if (request.not_after != 0) {
        struct itimerspec spec;
        spec.it_interval.tv_sec = 0;
        spec.it_interval.tv_nsec = 0;
        spec.it_value.tv_sec = request.not_after / 1000000;
        spec.it_value.tv_nsec = (request.not_after % 1000000) * 1000;
        r = timer_settime(id_timer, TIMER_ABSTIME, &spec, NULL);
        if (r == -1)
            perror("timer_settime() is failed");
    }

    struct epoll_event event;
    pid_unl0kr = pid;
    timer_action = TERMINATE_UNL0KR;
    timer_expired = false;
    unl0kr_exited = false;

    sigset_t sigmask;
    sigemptyset(&sigmask);

    for (;;) {
        r = epoll_pwait(fd_epoll, &event, 1, -1, &sigmask);
        if (r == -1) {
            if (errno != EINTR) {
                ret = errno;
                perror("epoll_pwait() is failed");
                break;
            }

            if (unl0kr_exited)
                break;

            if (timer_expired && ret != ECANCELED) {
                ret = ETIME;
                fprintf(stderr, "The request has expired\n");
            }

            continue;
        }

        r = process_inotify_events();
        if (r != 0) {
            ret = errno;
            break;
        }

        ret = ECANCELED;
        fprintf(stderr, "The request was cancelled\n");

        if (timer_expired)
            continue;

        if (request.not_after != 0) {
            struct itimerspec spec;
            spec.it_interval.tv_sec = 0;
            spec.it_interval.tv_nsec = 0;
            spec.it_value.tv_sec = 0;
            spec.it_value.tv_nsec = 0;
            r = timer_settime(id_timer, 0, &spec, NULL);
            if (r == -1)
                perror("Disarming the timer is failed");
        }

        r = raise(SIGALRM);
        if (r != 0) {
            ret = errno;
            perror("raise() is failed");
            break;
        }
    }

    timer_action = NO_ACTION;
    pid_unl0kr = 0;

    /* Stop the timer unconditionally because it can be armed by sigalarm() */
    struct itimerspec spec;
    spec.it_interval.tv_sec = 0;
    spec.it_interval.tv_nsec = 0;
    spec.it_value.tv_sec = 0;
    spec.it_value.tv_nsec = 0;
    r = timer_settime(id_timer, 0, &spec, NULL);
    if (r == -1)
        perror("Disarming the timer is failed");

    return ret;
}

int exec_unl0kr(char** ret_password)
{
    int ret = 0;
    int r;

    int fd_pipe[2];
    if (pipe(fd_pipe) != 0) {
        ret = errno;
        perror("Can't create a pipe");
        return ret;
    }

    sigset_t used_signals;
    sigemptyset(&used_signals);
    sigaddset(&used_signals, SIGCHLD);
    sigaddset(&used_signals, SIGALRM);

    r = sigprocmask(SIG_BLOCK, &used_signals, NULL);
    if (r == -1) {
        ret = errno;
        perror("sigprocmask(SIG_BLOCK) is failed");
        goto exit1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        ret = errno;
        perror("fork() is failed");
        goto exit2;
    }
    if (pid == 0) {
        /* Child */
        close(fd_pipe[0]);

        if (dup2(fd_pipe[1], STDOUT_FILENO) == -1) {
            perror("dup2() is failed");
            exit(EXIT_FAILURE);
        }

        char* argv[5];
        int argc = 2;
        argv[0] = UNL0KR_BINARY;
        argv[1] = "-n";
        if (request.message) {
            argv[2] = "-m";
            argv[3] = request.message;
            argc += 2;
        }
        argv[argc] = NULL;

        execv(UNL0KR_BINARY, argv);

        perror("exec() is failed");
        exit(EXIT_FAILURE);
    }

    /* Parent */
    r = event_loop(pid);

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        ret = errno;
        perror("waitpid() is failed");
        goto exit2;
    }

    if (r != 0) {
        ret = r;
        goto exit2;
    }

    if (!WIFEXITED(status)) {
        ret = ECHILD;
        fprintf(stderr, "unl0kr terminated abnormally\n");
        goto exit2;
    }

    int password_size;
    if (ioctl(fd_pipe[0], FIONREAD, &password_size) == -1) {
        ret = errno;
        perror("ioctl() is failed");
        goto exit2;
    }

    char* password = malloc(1 + password_size + 1);
    if (!password) {
        ret = ENOMEM;
        fprintf(stderr, "Out of memory\n");
        goto exit2;
    }

    password[0] = '+';
    if (password_size != 0) {
        password_size = read(fd_pipe[0], password + 1, password_size);
        if (password_size == -1) {
            ret = errno;
            perror("read() is failed");
            free(password);
            goto exit2;
        }
    }
    password[1 + password_size] = 0;

    *ret_password = password;
exit2:
    r = sigprocmask(SIG_UNBLOCK, &used_signals, NULL);
    if (r == -1)
        perror("sigprocmask(SIG_UNBLOCK) is failed");
exit1:
    close(fd_pipe[0]);
    close(fd_pipe[1]);
    return ret;
}

int wait_for_file_removed()
{
    struct epoll_event event;

    int r = epoll_wait(fd_epoll, &event, 1, 20000);
    if (r == -1) {
        int ret = errno;
        perror("epoll_wait() is failed");
        return ret;
    } else if (r == 0) {
        fprintf(stderr, "The file '%s' was not removed as expected, exiting.\n", request.file);
        return ETIME;
    }

    return process_inotify_events();
}

int main()
{
    int exit_code = EXIT_SUCCESS;
    int r;

    Request_init(&request);

    fd_epoll = epoll_create1(EPOLL_CLOEXEC);
    if (fd_epoll == -1) {
        perror("epoll_create1() is failed");
        exit_code = EXIT_FAILURE;
        goto exit1;
    }

    fd_inotify = inotify_init1(IN_NONBLOCK|IN_CLOEXEC);
    if (fd_inotify == -1) {
        perror("inotify_init1() is failed");
        exit_code = EXIT_FAILURE;
        goto exit2;
    }

    struct epoll_event epevent_inotify;
    epevent_inotify.events = EPOLLIN|EPOLLET;
    epevent_inotify.data.fd = fd_inotify;

    r = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_inotify, &epevent_inotify);
    if (r == -1) {
        perror("epoll_ctl() is failed");
        exit_code = EXIT_FAILURE;
        goto exit3;
    }

    struct sigevent sigevent_timer;
    sigevent_timer.sigev_notify = SIGEV_SIGNAL;
    sigevent_timer.sigev_signo = SIGALRM;
    sigevent_timer.sigev_value.sival_ptr = &id_timer;

    r = timer_create(CLOCK_MONOTONIC, &sigevent_timer, &id_timer);
    if (r == -1) {
        perror("timer_create() is failed");
        exit_code = EXIT_FAILURE;
        goto exit3;
    }

    struct sigaction sigaction_alarm;
    sigaction_alarm.sa_sigaction = sigalarm;
    sigaction_alarm.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction_alarm.sa_mask);
    sigaddset(&sigaction_alarm.sa_mask, SIGCHLD);

    r = sigaction(SIGALRM, &sigaction_alarm, NULL);
    if (r == -1) {
        perror("sigaction() for SIGALRM is failed");
        exit_code = EXIT_FAILURE;
        goto exit4;
    }

    struct sigaction sigaction_child;
    sigaction_child.sa_sigaction = sigchild;
    sigaction_child.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;
    sigemptyset(&sigaction_child.sa_mask);
    sigaddset(&sigaction_child.sa_mask, SIGALRM);

    r = sigaction(SIGCHLD, &sigaction_child, NULL);
    if (r == -1) {
        perror("sigaction() for SIGCHLD is failed");
        exit_code = EXIT_FAILURE;
        goto exit4;
    }

    for (;;) {
        char* file;
        r = find_request(&file);
        if (r != 0) {
            if (r != ENOENT)
                exit_code = EXIT_FAILURE;
            break;
        }

        int wd_inotify = inotify_add_watch(fd_inotify, file, IN_DELETE_SELF | IN_DONT_FOLLOW);
        if (wd_inotify == -1) {
            fprintf(stderr, "inotify_add_watch() is failed for '%s': %s\n", file, strerror(errno));
            free(file);
            exit_code = EXIT_FAILURE;
            break;
        }

        Request_reset(&request);
        request.file = file;

        r = ini_parse(file, ini_parser, &request);
        if (r < 0) {
            fprintf(stderr, "The file '%s' can't be parsed: %d\n", request.file, r);
            exit_code = EXIT_FAILURE;
            break;
        }

        if (request.pid != 0) {
            r = kill(request.pid, 0);
            if (r == -1 && errno == ESRCH) {
                fprintf(stderr, "The file '%s' contains invalid PID, removing.\n", request.file);
                remove(request.file);
                goto loop1;
            }
        }

        if (!request.socket) {
            fprintf(stderr, "The file '%s' doesn't contain a socket, waiting for removal.\n", request.file);
            goto loop1;
        }

        if (request.not_after != 0) {
            struct timespec ts;
            r = clock_gettime(CLOCK_MONOTONIC, &ts);
            if (r == -1) {
                perror("clock_gettime() is failed");
                exit_code = EXIT_FAILURE;
                break;
            }
            uint64_t now = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
            if (request.not_after <= now) {
                fprintf(stderr, "The request '%s' expired, waiting for removal.\n", request.file);
                goto loop1;
            }
        }

        char* password;
        r = exec_unl0kr(&password);
        if (r != 0) {
            if (r == ECANCELED)
                continue;
            else if (r == ETIME) {
                send_password("-");
                goto loop1;
            } else {
                exit_code = EXIT_FAILURE;
                break;
            }
        }

        r = send_password(password);
        erase_and_free(password);
        if (r != 0) {
            exit_code = EXIT_FAILURE;
            break;
        }

loop1:
        r = wait_for_file_removed();
        if (r != 0) {
            exit_code = EXIT_FAILURE;
            break;
        }
    }

exit4:
    timer_delete(id_timer);
exit3:
    close(fd_inotify);
exit2:
    close(fd_epoll);
exit1:
    Request_free(&request);
    return exit_code;
}
