/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BBX_INDEV_H
#define BBX_INDEV_H

#ifndef BBX_APP_BUFFYBOARD
#include "lvgl/lvgl.h"
#endif

#include <stdbool.h>
#include <stdint.h>

struct bbx_indev_opts {
#ifndef BBX_APP_BUFFYBOARD
    struct xkb_rule_names* keymap;
    uint8_t keyboard : 1;
#endif
    uint8_t pointer : 1;
    uint8_t touchscreen : 1;
};

#ifndef BBX_APP_BUFFYBOARD
/* All keyboard devices are added to this input group */
extern lv_group_t *keyboard_input_group;
#endif

/**
 * Attach input devices from /dev/input/event* and set up monitoring for new devices.
 *
 * @param fd_epoll epoll descriptor to add monitored events to
 * @param opts options for filtering input devices and setting a keyboard keymap
 * @return the number of file descriptors added to fd_epoll
 */
uint8_t bbx_indev_init(int fd_epoll, const struct bbx_indev_opts* opts);

/**
 * Stop input processing.
 */
void bbx_indev_suspend();

/**
 * Resume input processing.
 */
void bbx_indev_resume();

#ifndef BBX_APP_BUFFYBOARD
/**
 * Check if any keyboard devices are connected.
 *
 * @return true if at least one keyboard device is connected, false otherwise
 */
bool bbx_indev_is_keyboard_connected();

/**
 * Set a function that will be called on pressing KEY_POWER.
 */
void bbx_indev_set_key_power_cb(void (*callback)());
#endif

#endif /* BBX_INDEV_H */
