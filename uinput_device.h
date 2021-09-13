/**
 * Copyright 2021 Johannes Marbach
 *
 * This file is part of buffyboard, hereafter referred to as the program.
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


#ifndef BB_UINPUT_DEVICE_H
#define BB_UINPUT_DEVICE_H

#include <stdbool.h>

/**
 * Initialise the uinput keyboard device
 * 
 * @param scancodes array of scancodes the device can emit
 * @param num_scancodes number of scancodes the device can emit
 * @return true if creating the device was successful, false otherwise
 */
bool bb_uinput_device_init(int * const scancodes, int num_scancodes);

/**
 * Emit a key down event
 * 
 * @param scancode the key's scancode
 * @return true if emitting the event was successful, false otherwise
 */
bool bb_uinput_device_emit_key_down(int scancode);

/**
 * Emit a key up event
 * 
 * @param scancode the key's scancode
 * @return true if emitting the event was successful, false otherwise
 */
bool bb_uinput_device_emit_key_up(int scancode);

#endif /* BB_UINPUT_DEVICE_H */
