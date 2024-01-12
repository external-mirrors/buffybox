/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BB_INDEV_H
#define BB_INDEV_H

/**
 * Auto-connect currently available keyboard, pointer and touchscreen input devices.
 */
void bb_indev_auto_connect();

/**
 * Set up the mouse cursor image for currently connected pointer devices.
 */
void bb_indev_set_up_mouse_cursor();

#endif /* BB_INDEV_H */
