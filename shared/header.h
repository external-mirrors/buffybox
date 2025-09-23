/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BBX_HEADER_H
#define BBX_HEADER_H

#include "lvgl/lvgl.h"
#include <stdbool.h>

/**
 * Header configuration options
 */
typedef struct {
    /* Which elements to include */
    bool show_theme_toggle;
    bool show_keyboard_toggle;
    bool show_layout_dropdown;
    bool show_shutdown_button;

    /* Dropdown options (if enabled) */
    const char *dropdown_options;
    int dropdown_width;

    /* Button symbols */
    const char *theme_symbol;
    const char *keyboard_symbol;
    const char *shutdown_symbol;
} bbx_header_config_t;

/**
 * Header widget references for attaching callbacks
 */
typedef struct {
    lv_obj_t *header;
    lv_obj_t *theme_toggle_btn;
    lv_obj_t *keyboard_toggle_btn;
    lv_obj_t *layout_dropdown;
    lv_obj_t *shutdown_btn;
} bbx_header_widgets_t;

/**
 * Create a header bar with configurable elements.
 *
 * @param parent parent object (usually screen)
 * @param config header configuration
 * @param widgets output struct for widget references
 * @return header object
 */
lv_obj_t *bbx_header_create(lv_obj_t *parent, bbx_header_config_t *config, bbx_header_widgets_t *widgets);

/**
 * Initialize header config with default values.
 *
 * @param config config struct to initialize
 */
void bbx_header_init_config(bbx_header_config_t *config);

#endif /* BBX_HEADER_H */
