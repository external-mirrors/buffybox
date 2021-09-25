/**
 * Copyright 2021 Johannes Marbach
 *
 * This file is part of squeek2lvgl, hereafter referred to as the program.
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


#ifndef SQ2LV_H
#define SQ2LV_H

#include "../sq2lv_layouts.h"

/**
 * Find the first layout with a given short name.
 *
 * @param name layout short name
 * @return ID of the first matching layout or SQ2LV_LAYOUT_NONE if no layout matched
 */
sq2lv_layout_id_t find_layout_with_short_name(const char *name);

/**
 * Apply a layout to a keyboard.
 *
 * @param keyboard keyboard widget
 * @param layout_id layout ID
 */
void sq2lv_switch_layout(lv_obj_t *keyboard, sq2lv_layout_id_t layout_id);

/**
 * Check if a key acts as a layer switcher in the current layer.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the key
 * @return true if a layer switch would happen if the key is pressed, false otherwise
 */
bool sq2lv_is_layer_switcher(lv_obj_t *keyboard, uint16_t btn_id);

/**
 * Attempt to perform a layer switch after pressing a key in the current layer.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the pressed key
 * @return true if a layer switch was performed, false otherwise
 */
bool sq2lv_switch_layer(lv_obj_t *keyboard, uint16_t btn_id);

/**
 * Check if a key is a modifier in the current layer.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the key
 * @return true if the key is a modifier, false otherwise
 */
bool sq2lv_is_modifier(lv_obj_t *keyboard, uint16_t btn_id);

/**
 * Get the button indexes for all modifier keys in the current layer.
 *
 * @param keyboard keyboard widget
 * @param num_modifiers pointer to an integer into which the number of modifiers will be written
 * @return pointer to the array of button indexes corresponding to modifier keys
 */
const int * const sq2lv_get_modifier_indexes(lv_obj_t *keyboard, int *num_modifiers);

#if SQ2LV_SCANCODES_ENABLED
/**
 * Get scancodes associated with a key in the current layer.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the key
 * @param num_scancodes pointer to an integer into which the number of scancodes will be written
 * @return pointer into an array of scancodes at the appropriate index
 */
const int * const sq2lv_get_scancodes(lv_obj_t *keyboard, uint16_t btn_id, int *num_scancodes);
#endif /* SQ2LV_SCANCODES_ENABLED */

#endif /* SQ2LV_H */
