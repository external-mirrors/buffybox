/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
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
sq2lv_layout_id_t sq2lv_find_layout_with_short_name(const char *name);

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
int *sq2lv_get_modifier_indexes(lv_obj_t *keyboard, int *num_modifiers);

#if SQ2LV_SCANCODES_ENABLED
/**
 * Get scancodes associated with a key in the current layer.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the key
 * @param num_scancodes pointer to an integer into which the number of scancodes will be written
 * @return pointer into an array of scancodes at the appropriate index
 */
int *sq2lv_get_scancodes(lv_obj_t *keyboard, uint16_t btn_id, int *num_scancodes);
#endif /* SQ2LV_SCANCODES_ENABLED */

#endif /* SQ2LV_H */
