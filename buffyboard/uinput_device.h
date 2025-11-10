/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BB_UINPUT_DEVICE_H
#define BB_UINPUT_DEVICE_H

#include <stdbool.h>

/**
 * Initialise the uinput keyboard device
 * 
 * @param keycodes array of keycodes the device can emit
 * @param num_keycodes number of keycodes the device can emit
 * @return true if creating the device was successful, false otherwise
 */
bool bb_uinput_device_init(const int * const keycodes, int num_keycodes);

/**
 * Emit a key down event
 * 
 * @param keycode the key's keycode
 * @return true if emitting the event was successful, false otherwise
 */
bool bb_uinput_device_emit_key_down(int keycode);

/**
 * Emit a key up event
 * 
 * @param keycode the key's keycode
 * @return true if emitting the event was successful, false otherwise
 */
bool bb_uinput_device_emit_key_up(int keycode);

#endif /* BB_UINPUT_DEVICE_H */
