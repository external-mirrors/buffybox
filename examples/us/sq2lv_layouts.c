/**
 * Auto-generated with unsqu33kr
 **/

#include "sq2lv_layouts.h"

/**
 * Layout: US English - generated from us
 **/

static const char * const name_us = "US English";

/* Layer: Lowercase letters - generated from base */

static const int num_keys_lower_us = 34;

static const char * const keycaps_lower_us[] = { \
    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "\n", \
    "a", "s", "d", "f", "g", "h", "j", "k", "l", "\n", \
    "ABC", "z", "x", "c", "v", "b", "n", "m", LV_SYMBOL_BACKSPACE, "\n", \
    "1#", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_lower_us[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, \
    LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 2, 2, 2, 2, 2, 2, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 3, \
    LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 7, 2, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 3 \
};

static const int num_modifiers_lower_us = 0;

static const int modifier_idxs_lower_us[] = { \
     \
};

static const int num_switchers_lower_us = 2;

static const int switcher_idxs_lower_us[] = { \
    19, 28 \
};

static const int switcher_dests_lower_us[] = { \
    1, 2 \
};

/* Layer: Uppercase letters - generated from upper */

static const int num_keys_upper_us = 34;

static const char * const keycaps_upper_us[] = { \
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "\n", \
    "A", "S", "D", "F", "G", "H", "J", "K", "L", "\n", \
    "abc", "Z", "X", "C", "V", "B", "N", "M", LV_SYMBOL_BACKSPACE, "\n", \
    "1#", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_upper_us[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, \
    LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 2, 2, 2, 2, 2, 2, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 3, \
    LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 7, 2, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 3 \
};

static const int num_modifiers_upper_us = 0;

static const int modifier_idxs_upper_us[] = { \
     \
};

static const int num_switchers_upper_us = 2;

static const int switcher_idxs_upper_us[] = { \
    19, 28 \
};

static const int switcher_dests_upper_us[] = { \
    0, 2 \
};

/* Layer: Numbers / symbols - generated from numbers */

static const int num_keys_numbers_us = 34;

static const char * const keycaps_numbers_us[] = { \
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\n", \
    "@", "#", "$", "%", "&", "-", "_", "+", "(", ")", "\n", \
    ",", "\"", "'", ":", ";", "!", "?", LV_SYMBOL_BACKSPACE, "\n", \
    "abc", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_numbers_us[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 3, \
    LV_KEYBOARD_CTRL_BTN_FLAGS | 3, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 7, 2, 2, LV_KEYBOARD_CTRL_BTN_FLAGS | 3 \
};

static const int num_modifiers_numbers_us = 0;

static const int modifier_idxs_numbers_us[] = { \
     \
};

static const int num_switchers_numbers_us = 1;

static const int switcher_idxs_numbers_us[] = { \
    28 \
};

static const int switcher_dests_numbers_us[] = { \
    0 \
};

/* Layer array */

static const int num_layers_us = 3;

static const sq2lv_layer_t layers_us[] = {
    {
        .num_keys = num_keys_lower_us,
        .keycaps = keycaps_lower_us,
        .attributes = attributes_lower_us,
        .num_modifiers = num_modifiers_lower_us,
        .modifier_idxs = modifier_idxs_lower_us,
        .num_switchers = num_switchers_lower_us,
        .switcher_idxs = switcher_idxs_lower_us,
        .switcher_dests = switcher_dests_lower_us
    },
    {
        .num_keys = num_keys_upper_us,
        .keycaps = keycaps_upper_us,
        .attributes = attributes_upper_us,
        .num_modifiers = num_modifiers_upper_us,
        .modifier_idxs = modifier_idxs_upper_us,
        .num_switchers = num_switchers_upper_us,
        .switcher_idxs = switcher_idxs_upper_us,
        .switcher_dests = switcher_dests_upper_us
    },
    {
        .num_keys = num_keys_numbers_us,
        .keycaps = keycaps_numbers_us,
        .attributes = attributes_numbers_us,
        .num_modifiers = num_modifiers_numbers_us,
        .modifier_idxs = modifier_idxs_numbers_us,
        .num_switchers = num_switchers_numbers_us,
        .switcher_idxs = switcher_idxs_numbers_us,
        .switcher_dests = switcher_dests_numbers_us
    }
};

/**
 * Public interface
 **/

const int sq2lv_num_layouts = 1;

const sq2lv_layout_t sq2lv_layouts[] = {
    /* US English */
    {
        .name = name_us,
        .num_layers = num_layers_us,
        .layers = layers_us
    }
};

const char * const sq2lv_layout_names = 
    "US English";
