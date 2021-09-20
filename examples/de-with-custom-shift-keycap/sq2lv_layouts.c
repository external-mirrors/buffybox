/**
 * Auto-generated with squeek2lvgl
 **/

#include "sq2lv_layouts.h"

#define SQ2LV_SYMBOL_SHIFT "ABC"

/**
 * Layout: German - generated from de
 **/

static const char * const name_de = "German";
static const char * const short_name_de = "de";

/* Layer: Lowercase letters - generated from base */

static const int num_keys_lower_de = 36;

static const char * const keycaps_lower_de[] = { \
    "q", "w", "e", "r", "t", "z", "u", "i", "o", "p", "\n", \
    "a", "s", "d", "f", "g", "h", "j", "k", "l", "\n", \
    SQ2LV_SYMBOL_SHIFT, "y", "x", "c", "v", "b", "n", "m", LV_SYMBOL_BACKSPACE, "\n", \
    "123", "äÄ", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ",", ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_lower_de[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, \
    SQ2LV_CTRL_MOD_INACTIVE | 3, 2, 2, 2, 2, 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | 3, SQ2LV_CTRL_MOD_INACTIVE | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_lower_de = 0;

static const int modifier_idxs_lower_de[] = { \
     \
};

static const int num_switchers_lower_de = 3;

static const int switcher_idxs_lower_de[] = { \
    19, 28, 29 \
};

static const int switcher_dests_lower_de[] = { \
    1, 2, 3 \
};

/* Layer: Uppercase letters - generated from upper */

static const int num_keys_upper_de = 36;

static const char * const keycaps_upper_de[] = { \
    "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "\n", \
    "A", "S", "D", "F", "G", "H", "J", "K", "L", "\n", \
    SQ2LV_SYMBOL_SHIFT, "Y", "X", "C", "V", "B", "N", "M", LV_SYMBOL_BACKSPACE, "\n", \
    "123", "äÄ", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, "!", "?", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_upper_de[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, \
    SQ2LV_CTRL_MOD_ACTIVE | 3, 2, 2, 2, 2, 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | 3, SQ2LV_CTRL_MOD_INACTIVE | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_upper_de = 0;

static const int modifier_idxs_upper_de[] = { \
     \
};

static const int num_switchers_upper_de = 3;

static const int switcher_idxs_upper_de[] = { \
    19, 28, 29 \
};

static const int switcher_dests_upper_de[] = { \
    0, 2, 3 \
};

/* Layer: Numbers / symbols - generated from numbers */

static const int num_keys_numbers_de = 36;

static const char * const keycaps_numbers_de[] = { \
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\n", \
    "@", "#", "€", "%", "&", "-", "_", "+", "(", ")", "\n", \
    ";", "\"", "'", ":", "=", "<", ">", LV_SYMBOL_BACKSPACE, "\n", \
    "abc", "äÄ", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, ",", ".", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_numbers_de[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | 3, SQ2LV_CTRL_MOD_INACTIVE | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_numbers_de = 0;

static const int modifier_idxs_numbers_de[] = { \
     \
};

static const int num_switchers_numbers_de = 2;

static const int switcher_idxs_numbers_de[] = { \
    28, 29 \
};

static const int switcher_dests_numbers_de[] = { \
    0, 3 \
};

/* Layer: Special characters - generated from eschars */

static const int num_keys_special_de = 37;

static const char * const keycaps_special_de[] = { \
    "ä", "è", "é", "ö", "ü", "Ä", "È", "É", "Ö", "Ü", "\n", \
    "à", "â", "ê", "î", "ô", "À", "Â", "È", "Î", "Ô", "\n", \
    "123", "«", "»", "ç", "Ç", "æ", "œ", "ß", LV_SYMBOL_BACKSPACE, "\n", \
    "abc", "äÄ", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, "„", "“", LV_SYMBOL_OK, "" \
};

static const lv_btnmatrix_ctrl_t attributes_special_de[] = { \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    SQ2LV_CTRL_NON_CHAR | 3, 2, 2, 2, 2, 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3, \
    SQ2LV_CTRL_NON_CHAR | 3, SQ2LV_CTRL_MOD_ACTIVE | 3, SQ2LV_CTRL_NON_CHAR | 2, 7, SQ2LV_CTRL_NON_CHAR | 2, 2, 2, SQ2LV_CTRL_NON_CHAR | 3 \
};

static const int num_modifiers_special_de = 0;

static const int modifier_idxs_special_de[] = { \
     \
};

static const int num_switchers_special_de = 3;

static const int switcher_idxs_special_de[] = { \
    20, 29, 30 \
};

static const int switcher_dests_special_de[] = { \
    2, 0, 0 \
};

/* Layer array */

static const int num_layers_de = 4;

static const sq2lv_layer_t layers_de[] = {
    {
        .num_keys = num_keys_lower_de,
        .keycaps = keycaps_lower_de,
        .attributes = attributes_lower_de,
        .num_modifiers = num_modifiers_lower_de,
        .modifier_idxs = modifier_idxs_lower_de,
        .num_switchers = num_switchers_lower_de,
        .switcher_idxs = switcher_idxs_lower_de,
        .switcher_dests = switcher_dests_lower_de
    },
    {
        .num_keys = num_keys_upper_de,
        .keycaps = keycaps_upper_de,
        .attributes = attributes_upper_de,
        .num_modifiers = num_modifiers_upper_de,
        .modifier_idxs = modifier_idxs_upper_de,
        .num_switchers = num_switchers_upper_de,
        .switcher_idxs = switcher_idxs_upper_de,
        .switcher_dests = switcher_dests_upper_de
    },
    {
        .num_keys = num_keys_numbers_de,
        .keycaps = keycaps_numbers_de,
        .attributes = attributes_numbers_de,
        .num_modifiers = num_modifiers_numbers_de,
        .modifier_idxs = modifier_idxs_numbers_de,
        .num_switchers = num_switchers_numbers_de,
        .switcher_idxs = switcher_idxs_numbers_de,
        .switcher_dests = switcher_dests_numbers_de
    },
    {
        .num_keys = num_keys_special_de,
        .keycaps = keycaps_special_de,
        .attributes = attributes_special_de,
        .num_modifiers = num_modifiers_special_de,
        .modifier_idxs = modifier_idxs_special_de,
        .num_switchers = num_switchers_special_de,
        .switcher_idxs = switcher_idxs_special_de,
        .switcher_dests = switcher_dests_special_de
    }
};

/**
 * Public interface
 **/

const int sq2lv_num_layouts = 1;

const sq2lv_layout_t sq2lv_layouts[] = {
    /* German */
    {
        .name = name_de,
        .short_name = short_name_de,
        .num_layers = num_layers_de,
        .layers = layers_de
    }
};

const char * const sq2lv_layout_names =
    "German";

const char * const sq2lv_layout_short_names =
    "de";
