/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BB_THEME_H
#define BB_THEME_H

#include "lvgl/lvgl.h"

#include <stdbool.h>
#include <stdint.h>

#define BB_WIDGET_HEADER LV_OBJ_FLAG_USER_1

/**
 * Theming structs
 */

/* Window theme */
typedef struct {
    uint32_t bg_color;
} bb_theme_window;

/* Header theme */
typedef struct {
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t pad;
    lv_coord_t gap;
} bb_theme_header;

/* Key theme for one specific key type and state */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t border_color;
} bb_theme_key_state;

/* Key theme for one specific key type and all states */
typedef struct {
    bb_theme_key_state normal;
    bb_theme_key_state pressed;
} bb_theme_key;

/* Key theme */
typedef struct {
    lv_coord_t border_width;
    lv_coord_t corner_radius;
    bb_theme_key key_char;
    bb_theme_key key_non_char;
    bb_theme_key key_mod_act;
    bb_theme_key key_mod_inact;
} bb_theme_keys;

/* Keyboard theme */
typedef struct {
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t pad;
    lv_coord_t gap;
    bb_theme_keys keys;
} bb_theme_keyboard;

/* Button theme for one specific button state */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t border_color;
} bb_theme_button_state;

/* Button theme */
typedef struct {
    lv_coord_t border_width;
    lv_coord_t corner_radius;
    lv_coord_t pad;
    bb_theme_button_state normal;
    bb_theme_button_state pressed;
} bb_theme_button;

/* Text area cursor theme */
typedef struct {
    lv_coord_t width;
    uint32_t color;
    int period;
} bb_theme_textarea_cursor;

/* Text area theme */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    lv_coord_t pad;
    uint32_t placeholder_color;
    bb_theme_textarea_cursor cursor;
} bb_theme_textarea;

/* Dropdown list theme */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t selection_fg_color;
    uint32_t selection_bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    lv_coord_t pad;
} bb_theme_dropdown_list;

/* Dropdown theme */
typedef struct {
    bb_theme_button button;
    bb_theme_dropdown_list list;
} bb_theme_dropdown;

/* Label */
typedef struct {
    uint32_t fg_color;
} bb_theme_label;

/* Message box buttons theme */
typedef struct {
    lv_coord_t gap;
} bb_theme_msgbox_buttons;

/* Message box dimming theme */
typedef struct {
    uint32_t color;
    short opacity;
} bb_theme_msgbox_dimming;

/* Message box theme */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    lv_coord_t pad;
    lv_coord_t gap;
    bb_theme_msgbox_buttons buttons;
    bb_theme_msgbox_dimming dimming;
} bb_theme_msgbox;

/* Progress bar indicator theme */
typedef struct {
    uint32_t bg_color;
} bb_theme_bar_indicator;

/* Progress bar theme */
typedef struct {
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    bb_theme_bar_indicator indicator;
} bb_theme_bar;

/* Full theme */
typedef struct {
    char *name;
    bb_theme_window window;
    bb_theme_header header;
    bb_theme_keyboard keyboard;
    bb_theme_button button;
    bb_theme_textarea textarea;
    bb_theme_dropdown dropdown;
    bb_theme_label label;
    bb_theme_msgbox msgbox;
    bb_theme_bar bar;
} ul_theme;

/**
 * Prepare a keyboard widget to be themed with a theme.
 *
 * @param keyboard keyboard widget
 */
void bb_theme_prepare_keyboard(lv_obj_t *keyboard);

/**
 * Apply a UI theme.
 *
 * @param theme the theme to apply
 */
void bb_theme_apply(const ul_theme *theme);

#endif /* BB_THEME_H */
