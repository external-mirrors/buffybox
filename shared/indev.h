/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_INDEV_H
#define BBX_INDEV_H

#include "lvgl/lvgl.h"

#include <stdbool.h>

/**
 * Set the required capabilities for input devices.
 *
 * @param keyboard if true, allow connection of keyboard devices
 * @param pointer if true, allow connection of pointer devices
 * @param touchscreen if true, allow connection of touchscreen devices
 */
void bbx_indev_set_allowed_device_capability(bool keyboard, bool pointer, bool touchscreen);

/**
 * Set the group for receiving input from keyboard devices.
 * 
 * @param group group that should receive input
 */
void bbx_indev_set_keyboard_input_group(lv_group_t *group);

/**
 * Start the udev device monitor and auto-connect currently available devices.
 *
 * @param keyboard if true, allow connection of keyboard devices
 * @param pointer if true, allow connection of pointer devices
 * @param touchscreen if true, allow connection of touchscreen devices
 */
void bbx_indev_start_monitor_and_autoconnect(bool keyboard, bool pointer, bool touchscreen);

/**
 * Auto-connect currently available keyboard, pointer and touchscreen input devices.
 */
void bbx_indev_auto_connect();

/**
 * Start the udev device monitor.
 */
void bbx_indev_start_monitor();

/**
 * Stop the udev device monitor.
 */
void bbx_indev_stop_monitor();

/**
 * Query the udev device monitor and (dis)connect added or removed devices
 */
void bbx_indev_query_monitor();

/**
 * Check if any keyboard devices are connected.
 *
 * @return true if at least one keyboard device is connected, false otherwise
 */
bool bbx_indev_is_keyboard_connected();

#endif /* BBX_INDEV_H */
