/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_THEME_H
#define BBX_THEME_H

#include "lvgl/lvgl.h"

#include <stdbool.h>
#include <stdint.h>

#define BBX_WIDGET_HEADER LV_OBJ_FLAG_USER_1

/**
 * Theming structs
 */

/* Window theme */
typedef struct {
    uint32_t bg_color;
} bbx_theme_window;

/* Header theme */
typedef struct {
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t pad;
    lv_coord_t gap;
} bbx_theme_header;

/* Key theme for one specific key type and state */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t border_color;
} bbx_theme_key_state;

/* Key theme for one specific key type and all states */
typedef struct {
    bbx_theme_key_state normal;
    bbx_theme_key_state pressed;
} bbx_theme_key;

/* Key theme */
typedef struct {
    lv_coord_t border_width;
    lv_coord_t corner_radius;
    bbx_theme_key key_char;
    bbx_theme_key key_non_char;
    bbx_theme_key key_mod_act;
    bbx_theme_key key_mod_inact;
} bbx_theme_keys;

/* Keyboard theme */
typedef struct {
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t pad;
    lv_coord_t gap;
    bbx_theme_keys keys;
} bbx_theme_keyboard;

/* Button theme for one specific button state */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t border_color;
} bbx_theme_button_state;

/* Button theme */
typedef struct {
    lv_coord_t border_width;
    lv_coord_t corner_radius;
    lv_coord_t pad;
    bbx_theme_button_state normal;
    bbx_theme_button_state pressed;
} bbx_theme_button;

/* Text area cursor theme */
typedef struct {
    lv_coord_t width;
    uint32_t color;
    int period;
} bbx_theme_textarea_cursor;

/* Text area theme */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    lv_coord_t pad;
    uint32_t placeholder_color;
    bbx_theme_textarea_cursor cursor;
} bbx_theme_textarea;

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
} bbx_theme_dropdown_list;

/* Dropdown theme */
typedef struct {
    bbx_theme_button button;
    bbx_theme_dropdown_list list;
} bbx_theme_dropdown;

/* Label */
typedef struct {
    uint32_t fg_color;
} bbx_theme_label;

/* Message box dimming theme */
typedef struct {
    uint32_t color;
    short opacity;
} bbx_theme_msgbox_dimming;

/* Message box theme */
typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    lv_coord_t pad;
    lv_coord_t gap;
    bbx_theme_msgbox_dimming dimming;
} bbx_theme_msgbox;

/* Progress bar indicator theme */
typedef struct {
    uint32_t bg_color;
} bbx_theme_bar_indicator;

/* Progress bar theme */
typedef struct {
    lv_coord_t border_width;
    uint32_t border_color;
    lv_coord_t corner_radius;
    bbx_theme_bar_indicator indicator;
} bbx_theme_bar;

/* Full theme */
typedef struct {
    char *name;
    bbx_theme_window window;
    bbx_theme_header header;
    bbx_theme_keyboard keyboard;
    bbx_theme_button button;
    bbx_theme_textarea textarea;
    bbx_theme_dropdown dropdown;
    bbx_theme_label label;
    bbx_theme_msgbox msgbox;
    bbx_theme_bar bar;
} bbx_theme;

/**
 * Prepare a keyboard widget to be themed with a theme.
 *
 * @param keyboard keyboard widget
 */
void bbx_theme_prepare_keyboard(lv_obj_t *keyboard);

/**
 * Apply a UI theme.
 *
 * @param theme the theme to apply
 */
void bbx_theme_apply(const bbx_theme *theme);

#endif /* BBX_THEME_H */
