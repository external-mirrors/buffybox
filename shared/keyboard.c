/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "keyboard.h"
#include "theme.h"
#include "../squeek2lvgl/sq2lv.h"

void bbx_keyboard_init_config(bbx_keyboard_config_t *config) {
    config->mode = LV_KEYBOARD_MODE_TEXT_LOWER;
    config->layout_id = 0;  /* Default to first layout */
    config->popovers = false;
    config->height = -1; /* Auto-calculate */
    config->value_changed_callback = NULL;
    config->ready_callback = NULL;
}

lv_obj_t *bbx_keyboard_create(lv_obj_t *parent, lv_obj_t *textarea, bbx_keyboard_config_t *config) {
    lv_obj_t *keyboard = lv_keyboard_create(parent);

    lv_keyboard_set_mode(keyboard, config->mode);
    lv_keyboard_set_textarea(keyboard, textarea);

    /* Remove default event callback and add custom ones */
    uint32_t num_events = lv_obj_get_event_count(keyboard);
    for(uint32_t i = 0; i < num_events; ++i) {
        if(lv_event_dsc_get_cb(lv_obj_get_event_dsc(keyboard, i)) == lv_keyboard_def_event_cb) {
            lv_obj_remove_event(keyboard, i);
            break;
        }
    }

    if (config->value_changed_callback) {
        lv_obj_add_event_cb(keyboard, config->value_changed_callback, LV_EVENT_VALUE_CHANGED, NULL);
    }

    if (config->ready_callback) {
        lv_obj_add_event_cb(keyboard, config->ready_callback, LV_EVENT_READY, NULL);
    }

    /* Set size */
    if (config->height > 0) {
        lv_obj_set_size(keyboard, LV_PCT(100), config->height);
    }

    /* Apply theme preparation */
    bbx_theme_prepare_keyboard(keyboard);

    /* Set layout */
    sq2lv_switch_layout(keyboard, config->layout_id);

    /* Enable popovers if requested */
    if (config->popovers) {
        lv_keyboard_set_popovers(keyboard, true);
    }

    return keyboard;
}
