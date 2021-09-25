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


#include "sq2lv.h"


/**
 * Static variables
 */

static sq2lv_layout_id_t current_layout_id = SQ2LV_LAYOUT_NONE;


/**
 * Static prototypes
 */

/**
 * Convert a layer index to an LVGL keyboard mode.
 *
 * @param layer_index layer index to convert
 * @return corresponding LVGL keyboard mode
 */
static lv_keyboard_mode_t layer_index_to_keyboard_mode(int layer_index);

/**
 * Convert an LVGL keyboard mode to a layer index.
 *
 * @param keyboard_mode LVGL keyboard mode to convert
 * @return corresponding layer index
 */
static int keyboard_mode_to_layer_index(lv_keyboard_mode_t keyboard_mode);

/**
 * Get the layer index corresponding to a keyboard's current mode.
 *
 * @param keyboard keyboard widget
 * @return layer index
 */
static int get_layer_index(lv_obj_t *keyboard);

/**
 * Get the index of the destination layer for a layer switcher key in the current layout.
 *
 * @param keyboard keyboard widget
 * @param btn_id button index corresponding to the key
 * @return the destination layer's index or -1 if the key is not a layer switcher
 */
static int get_destination_layer_index_for_layer_switcher(lv_obj_t *keyboard, uint16_t btn_id);


/**
 * Static functions
 */

static lv_keyboard_mode_t layer_index_to_keyboard_mode(int layer_index) {
    switch (layer_index) {
        case 0:
            return LV_KEYBOARD_MODE_TEXT_LOWER;
        case 1:
            return LV_KEYBOARD_MODE_TEXT_UPPER;
        case 2:
            return LV_KEYBOARD_MODE_SPECIAL;
        case 3:
            return LV_KEYBOARD_MODE_NUMBER;
        default:
            return -1;
    }
}

static int keyboard_mode_to_layer_index(lv_keyboard_mode_t keyboard_mode) {
    switch (keyboard_mode) {
        case LV_KEYBOARD_MODE_TEXT_LOWER:
            return 0;
        case LV_KEYBOARD_MODE_TEXT_UPPER:
            return 1;
        case LV_KEYBOARD_MODE_SPECIAL:
            return 2;
        case LV_KEYBOARD_MODE_NUMBER:
            return 3;
        default:
            return -1;
    }
}

static int get_layer_index(lv_obj_t *keyboard) {
    return keyboard_mode_to_layer_index(lv_keyboard_get_mode(keyboard));
}

static int get_destination_layer_index_for_layer_switcher(lv_obj_t *keyboard, uint16_t btn_id) {
    if (current_layout_id < 0 || current_layout_id >= sq2lv_num_layouts) {
        return -1;
    }

    int layer_index = get_layer_index(keyboard);
    if (layer_index < 0 || layer_index >= sq2lv_layouts[current_layout_id].num_layers) {
        return -1;
    }

    for (int i = 0; i < sq2lv_layouts[current_layout_id].layers[layer_index].num_switchers; ++i) {
        if (sq2lv_layouts[current_layout_id].layers[layer_index].switcher_idxs[i] == btn_id) {
            return sq2lv_layouts[current_layout_id].layers[layer_index].switcher_dests[i];
        }
    }

    return -1;
}


/**
 * Public functions
 */

sq2lv_layout_id_t sq2lv_find_layout_with_short_name(const char *name) {
    for (int i = 0; i < sq2lv_num_layouts; ++i) {
        if (strcmp(sq2lv_layouts[i].short_name, name) == 0) {
            return i;
        }
    }
    return SQ2LV_LAYOUT_NONE;
}

void sq2lv_switch_layout(lv_obj_t *keyboard, sq2lv_layout_id_t layout_id) {
    if (layout_id < 0 || layout_id >= sq2lv_num_layouts) {
        return;
    }

    /* Assign layers */
    for (int i = 0; i < sq2lv_layouts[layout_id].num_layers; ++i) {
        lv_keyboard_set_map(keyboard, layer_index_to_keyboard_mode(i),
            (const char **)sq2lv_layouts[layout_id].layers[i].keycaps,
            sq2lv_layouts[layout_id].layers[i].attributes);
    }

    /* Switch to default layer if current layer doesn't exist in new layout */
    int layer_index = get_layer_index(keyboard);
    if (layer_index < 0 || layer_index >= sq2lv_layouts[layout_id].num_layers) {
        lv_keyboard_set_mode(keyboard, layer_index_to_keyboard_mode(0));
    }

    current_layout_id = layout_id;
}

bool sq2lv_is_layer_switcher(lv_obj_t *keyboard, uint16_t btn_id) {
    return get_destination_layer_index_for_layer_switcher(keyboard, btn_id) >= 0;
}

bool sq2lv_switch_layer(lv_obj_t *keyboard, uint16_t btn_id) {
    int destination_layer_index = get_destination_layer_index_for_layer_switcher(keyboard, btn_id);
    if (destination_layer_index < 0 || destination_layer_index >= sq2lv_layouts[current_layout_id].num_layers) {
        return false;
    }

    lv_keyboard_set_mode(keyboard, layer_index_to_keyboard_mode(destination_layer_index));
    return true;
}

bool sq2lv_is_modifier(lv_obj_t *keyboard, uint16_t btn_id) {
    if (current_layout_id < 0 || current_layout_id >= sq2lv_num_layouts) {
        return false;
    }

    int layer_index = get_layer_index(keyboard);
    if (layer_index < 0 || layer_index >= sq2lv_layouts[current_layout_id].num_layers) {
        return false;
    }

    for (int i = 0; i < sq2lv_layouts[current_layout_id].layers[layer_index].num_modifiers; ++i) {
        if (sq2lv_layouts[current_layout_id].layers[layer_index].modifier_idxs[i] == btn_id) {
            return true;
        }
    }

    return false;
}

const int * const sq2lv_get_modifier_indexes(lv_obj_t *keyboard, int *num_modifiers) {
    if (current_layout_id < 0 || current_layout_id >= sq2lv_num_layouts) {
        *num_modifiers = 0;
        return NULL;
    }

    int layer_index = get_layer_index(keyboard);
    if (layer_index < 0 || layer_index >= sq2lv_layouts[current_layout_id].num_layers) {
        *num_modifiers = 0;
        return NULL;
    }

    *num_modifiers = sq2lv_layouts[current_layout_id].layers[layer_index].num_modifiers;
    if (*num_modifiers == 0) {
        return NULL;
    }

    return &(sq2lv_layouts[current_layout_id].layers[layer_index].modifier_idxs[0]);
}

#if SQ2LV_SCANCODES_ENABLED
const int * const sq2lv_get_scancodes(lv_obj_t *keyboard, uint16_t btn_id, int *num_scancodes) {
    if (current_layout_id < 0 || current_layout_id >= sq2lv_num_layouts) {
        *num_scancodes = 0;
        return NULL;
    }

    int layer_index = get_layer_index(keyboard);
    if (layer_index < 0 || layer_index >= sq2lv_layouts[current_layout_id].num_layers) {
        *num_scancodes = 0;
        return NULL;
    }

    if (btn_id >= sq2lv_layouts[current_layout_id].layers[layer_index].num_keys) {
        *num_scancodes = 0;
        return NULL;
    }

    *num_scancodes = sq2lv_layouts[current_layout_id].layers[layer_index].scancode_nums[btn_id];
    if (*num_scancodes == 0) {
        return NULL;
    }

    const int index = sq2lv_layouts[current_layout_id].layers[layer_index].scancode_idxs[btn_id];
    return &(sq2lv_layouts[current_layout_id].layers[layer_index].scancodes[index]);
}
#endif /* SQ2LV_SCANCODES_ENABLED */
