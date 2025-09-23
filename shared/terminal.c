/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "terminal.h"

#include "../shared/log.h"

#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include <linux/kd.h>

#include <sys/ioctl.h>


/**
 * Static variables
 */

static int current_fd = -1;

static int original_mode = -1;
static int original_kb_mode = -1;

/**
 * Public functions
 */

void bbx_terminal_prepare_current_terminal(bool enable_graphics_mode, bool disable_keyboard_input) {
    /* Exit early if there is nothing to do */
    if (!enable_graphics_mode && !disable_keyboard_input) {
        return;
    }

    /* Open the current terminal */
    current_fd = open("/dev/tty0", O_RDWR);
    if (current_fd < 0) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not open /dev/tty0");
        return;
    }

    /* Disable terminal keyboard input (hides entered text) */
    if (disable_keyboard_input) {
        if (ioctl(current_fd, KDGKBMODE, &original_kb_mode) != 0) {
            bbx_log(BBX_LOG_LEVEL_WARNING, "Could not get terminal keyboard mode");
        }

        if (ioctl(current_fd, KDSKBMODE, K_OFF) != 0) {
            bbx_log(BBX_LOG_LEVEL_WARNING, "Could not set terminal keyboard mode to off");
        }
    }

    /* Switch terminal into graphics mode (hides command prompt) */
    if (enable_graphics_mode) {
        if (ioctl(current_fd, KDGETMODE, &original_mode) != 0) {
            bbx_log(BBX_LOG_LEVEL_WARNING, "Could not get terminal mode");
        }

        if (ioctl(current_fd, KDSETMODE, KD_GRAPHICS) != 0) {
            bbx_log(BBX_LOG_LEVEL_WARNING, "Could not set terminal mode to graphics");
        }
    }
}

void bbx_terminal_reset_current_terminal(void) {
    /* If we haven't previously opened the current terminal, exit */
    if (current_fd < 0) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not reset current terminal");
        return;
    }

    /* Reset terminal mode if needed */
    if (original_mode >= 0 && ioctl(current_fd, KDSETMODE, original_mode) != 0) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not reset terminal mode");
    }

    /* Reset terminal keyboard mode if needed */
    if (original_kb_mode >= 0 && ioctl(current_fd, KDSKBMODE, original_kb_mode) != 0) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not reset terminal keyboard mode");
    }

    close(current_fd);
    current_fd = -1;
}
