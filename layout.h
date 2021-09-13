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


#ifndef BB_LAYOUT_H
#define BB_LAYOUT_H

#include "sq2lv_layouts.h"

/**
 * Apply a layout to a keyboard.
 *
 * @param keyboard keyboard widget
 * @param layout_id layout ID
 */
void bb_layout_switch_layout(lv_obj_t *keyboard, sq2lv_layout_id_t layout_id);

/**
 * Attempt to perform a layer switch after pressing a key.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the pressed key
 * @return true if a layer switch was performed, false otherwise
 */
bool bb_layout_switch_layer(lv_obj_t *keyboard, uint16_t btn_id);

/**
 * Get scancodes associated with a key.
 * 
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the key
 * @param num_scancodes pointer to an integer into which the number of scancodes will be written
 * @return pointer into an array of scancodes at the appropriate index
 */
const int * const bb_layout_get_scancodes(lv_obj_t *keyboard, uint16_t btn_id, int *num_scancodes);

#endif /* BB_LAYOUT_H */
