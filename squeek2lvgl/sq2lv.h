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

#include "lvgl.h"

#define SQ2LV_SCANCODES_ENABLED 0

/* Key attributes */
#define SQ2LV_CTRL_NON_CHAR     (LV_BTNMATRIX_CTRL_CLICK_TRIG | LV_BTNMATRIX_CTRL_CHECKED)
#define SQ2LV_CTRL_MOD_ACTIVE   (LV_BTNMATRIX_CTRL_CLICK_TRIG | LV_BTNMATRIX_CTRL_CHECKABLE)
#define SQ2LV_CTRL_MOD_INACTIVE (LV_BTNMATRIX_CTRL_CLICK_TRIG | LV_BTNMATRIX_CTRL_CHECKABLE | LV_BTNMATRIX_CTRL_CHECKED)

/* Layout IDs, values can be used as indexes into the sq2lv_layouts array */
typedef enum {
    SQ2LV_LAYOUT_NONE = -1,
    SQ2LV_LAYOUT_US = 0,
    SQ2LV_LAYOUT_DE = 1,
    SQ2LV_LAYOUT_ES = 2,
    SQ2LV_LAYOUT_FR = 3
} sq2lv_layout_id_t;

/* Layer type */
typedef struct {
    /* Number of keys */
    const int num_keys;
    /* Key caps */
    const char * const * const keycaps;
    /* Key attributes */
    const lv_btnmatrix_ctrl_t * const attributes;
    /* Number of modifier keys */
    const int num_modifiers;
    /* Button indexes of modifier keys */
    const int * const modifier_idxs;
    /* Number of buttons that trigger a layer switch */
    const int num_switchers;
    /* Button indexes that trigger a layer switch */
    const int * const switcher_idxs;
    /* Indexes of layers to jump to when triggering layer switch buttons */
    const int * const switcher_dests;
} sq2lv_layer_t;

/* Layout type */
typedef struct {
    /* Layout name */
    const char * const name;
    /* Layout short name */
    const char * const short_name;
    /* Total number of layers */
    const int num_layers;
    /* Layers array */
    const sq2lv_layer_t * const layers;
} sq2lv_layout_t;

/**
 * Configure the layouts to be used by the keyboard.
 * MUST be called during initialisation!
 *
 * @param num_layouts number of layouts
 * @param layouts array of layouts
 * @return 0 on success, -1 on failure
 */
int sq2lv_configure_layouts(unsigned int num_layouts, const sq2lv_layout_t *layouts);

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
