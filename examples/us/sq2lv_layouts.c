/**
 * Auto-generated with squeek2lvgl
 **/

#include "sq2lv_layouts.h"

#define SQ2LV_SYMBOL_SHIFT "Shift"

/**
 * Layout: US English - generated from us
 **/

static const char * const name_us = "US English";
static const char * const short_name_us = "us";

/* Layer: Lowercase letters - generated from base */

static const int num_keys_lower_us = 34;

static const char * const keycaps_lower_us[] = { \
    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "\n", \
    "a", "s", "d", "f", "g", "h", "j", "k", "l", "\n", \
    SQ2LV_SYMBOL_SHIFT, "z", "x", "c", "v", "b", "n", "m", LV_SYMBOL_BACKSPACE, "\n", \
    "123", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_lower_us[] = { \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    SQ2LV_CTRL_MOD_INACTIVE | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_lower_us = 0;

static const int * const modifier_idxs_lower_us = NULL;

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
    SQ2LV_SYMBOL_SHIFT, "Z", "X", "C", "V", "B", "N", "M", LV_SYMBOL_BACKSPACE, "\n", \
    "123", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_upper_us[] = { \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    SQ2LV_CTRL_MOD_ACTIVE | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_upper_us = 0;

static const int * const modifier_idxs_upper_us = NULL;

static const int num_switchers_upper_us = 2;

static const int switcher_idxs_upper_us[] = { \
    19, 28 \
};

static const int switcher_dests_upper_us[] = { \
    0, 2 \
};

/* Layer: Numbers / symbols - generated from numbers */

static const int num_keys_numbers_us = 35;

static const char * const keycaps_numbers_us[] = { \
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\n", \
    "@", "#", "$", "%", "&", "-", "_", "+", "(", ")", "\n", \
    "*/=", ",", "\"", "'", ":", ";", "!", "?", LV_SYMBOL_BACKSPACE, "\n", \
    "ABC", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_numbers_us[] = { \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    SQ2LV_CTRL_NON_CHAR | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_numbers_us = 0;

static const int * const modifier_idxs_numbers_us = NULL;

static const int num_switchers_numbers_us = 2;

static const int switcher_idxs_numbers_us[] = { \
    20, 29 \
};

static const int switcher_dests_numbers_us[] = { \
    3, 0 \
};

/* Layer: Symbols - generated from symbols */

static const int num_keys_symbols_us = 35;

static const char * const keycaps_symbols_us[] = { \
    "~", "`", "|", "·", "√", "π", "τ", "÷", "×", "¶", "\n", \
    "©", "®", "£", "€", "¥", "^", "°", "*", "{", "}", "\n", \
    "123", "\\", "/", "<", ">", "=", "[", "]", LV_SYMBOL_BACKSPACE, "\n", \
    "ABC", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_symbols_us[] = { \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, \
    SQ2LV_CTRL_NON_CHAR | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | LV_BTNMATRIX_CTRL_NO_REPEAT | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, LV_BTNMATRIX_CTRL_POPOVER | LV_BTNMATRIX_CTRL_NO_REPEAT | 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_symbols_us = 0;

static const int * const modifier_idxs_symbols_us = NULL;

static const int num_switchers_symbols_us = 2;

static const int switcher_idxs_symbols_us[] = { \
    20, 29 \
};

static const int switcher_dests_symbols_us[] = { \
    2, 0 \
};

/* Layer array */

static const int num_layers_us = 4;

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
    },
    {
        .num_keys = num_keys_symbols_us,
        .keycaps = keycaps_symbols_us,
        .attributes = attributes_symbols_us,
        .num_modifiers = num_modifiers_symbols_us,
        .modifier_idxs = modifier_idxs_symbols_us,
        .num_switchers = num_switchers_symbols_us,
        .switcher_idxs = switcher_idxs_symbols_us,
        .switcher_dests = switcher_dests_symbols_us
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
        .short_name = short_name_us,
        .num_layers = num_layers_us,
        .layers = layers_us
    }
};

const char * const sq2lv_layout_names =
    "US English";

const char * const sq2lv_layout_short_names =
    "us";
