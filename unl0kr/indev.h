/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef UL_INDEV_H
#define UL_INDEV_H

#include "lvgl/lvgl.h"

#include <stdbool.h>

/**
 * Set the required capabilities for input devices.
 *
 * @param keyboard if true, allow connection of keyboard devices
 * @param pointer if true, allow connection of pointer devices
 * @param touchscreen if true, allow connection of touchscreen devices
 */
void ul_indev_set_allowed_device_capability(bool keyboard, bool pointer, bool touchscreen);

/**
 * Set the group for receiving input from keyboard devices.
 * 
 * @param group group that should receive input
 */
void ul_indev_set_keyboard_input_group(lv_group_t *group);

/**
 * Auto-connect currently available keyboard, pointer and touchscreen input devices.
 */
void ul_indev_auto_connect();

/**
 * Start the udev device monitor.
 */
void ul_indev_start_monitor();

/**
 * Stop the udev device monitor.
 */
void ul_indev_stop_monitor();

/**
 * Query the udev device monitor and (dis)connect added or removed devices
 */
void ul_indev_query_monitor();

/**
 * Check if any keyboard devices are connected.
 *
 * @return true if at least one keyboard device is connected, false otherwise
 */
bool ul_indev_is_keyboard_connected();

#endif /* UL_INDEV_H */
