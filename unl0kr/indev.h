/**
 * Copyright 2021 Johannes Marbach
 *
 * This file is part of buffybox, hereafter referred to as the program.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
