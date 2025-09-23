/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "header.h"

void bbx_header_init_config(bbx_header_config_t *config) {
    config->show_theme_toggle = true;
    config->show_keyboard_toggle = true;
    config->show_layout_dropdown = true;
    config->show_shutdown_button = true;
    config->dropdown_options = "Option 1\nOption 2\nOption 3";
    config->dropdown_width = 90;
    config->theme_symbol = LV_SYMBOL_SETTINGS;
    config->keyboard_symbol = LV_SYMBOL_KEYBOARD;
    config->shutdown_symbol = LV_SYMBOL_POWER;
}

lv_obj_t *bbx_header_create(lv_obj_t *parent, bbx_header_config_t *config, bbx_header_widgets_t *widgets) {
    lv_memzero(widgets, sizeof(bbx_header_widgets_t));

    /* Create header flexbox */
    lv_obj_t *header = lv_obj_create(parent);
    widgets->header = header;

    /* Theme toggle button */
    if (config->show_theme_toggle) {
        lv_obj_t *btn = lv_button_create(header);
        widgets->theme_toggle_btn = btn;

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, config->theme_symbol);
        lv_obj_center(label);
    }

    /* Keyboard toggle button */
    if (config->show_keyboard_toggle) {
        lv_obj_t *btn = lv_button_create(header);
        widgets->keyboard_toggle_btn = btn;

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, config->keyboard_symbol);
        lv_obj_center(label);
    }

    /* Layout dropdown */
    if (config->show_layout_dropdown) {
        lv_obj_t *dropdown = lv_dropdown_create(header);
        widgets->layout_dropdown = dropdown;

        lv_dropdown_set_options(dropdown, config->dropdown_options);
        lv_obj_set_width(dropdown, config->dropdown_width);
    }

    /* Spacer */
    lv_obj_t *spacer = lv_obj_create(header);
    lv_obj_set_height(spacer, 0);
    lv_obj_set_flex_grow(spacer, 1);

    /* Shutdown button */
    if (config->show_shutdown_button) {
        lv_obj_t *btn = lv_button_create(header);
        widgets->shutdown_btn = btn;

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, config->shutdown_symbol);
        lv_obj_center(label);
    }

    /* Size buttons to match dropdown height */
    if (widgets->layout_dropdown) {
        lv_obj_update_layout(widgets->layout_dropdown);
        const int32_t dropdown_height = lv_obj_get_height(widgets->layout_dropdown);

        if (widgets->theme_toggle_btn) {
            lv_obj_set_size(widgets->theme_toggle_btn, dropdown_height, dropdown_height);
        }
        if (widgets->keyboard_toggle_btn) {
            lv_obj_set_size(widgets->keyboard_toggle_btn, dropdown_height, dropdown_height);
        }
        if (widgets->shutdown_btn) {
            lv_obj_set_size(widgets->shutdown_btn, dropdown_height, dropdown_height);
        }
    }

    return header;
}
