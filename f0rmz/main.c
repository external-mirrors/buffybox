/**
 * Copyright 2025 Clayton Craft
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "command_line.h"
#include "config.h"

#include "../shared/backends.h"
#include "../shared/display.h"
#include "../shared/header.h"
#include "../shared/indev.h"
#include "../shared/keyboard.h"
#include "../shared/log.h"
#include "../shared/theme.h"
#include "../shared/themes.h"
#include "../shared/terminal.h"
#include "../squeek2lvgl/sq2lv.h"

#include "lvgl/lvgl.h"

#include <sys/epoll.h>
#include <sys/reboot.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define F0_PASSWORD_HIDDEN_DOTS  "••••••••"

/**
 * Static variables
 */

f0_cli_opts cli_opts;
f0_config_opts conf_opts;

bool is_alternate_theme = false;
bool is_keyboard_hidden = false;

static char **field_values = NULL;

int current_field_index = 0;

lv_obj_t *form_container = NULL;
lv_obj_t *form_textarea = NULL;
lv_obj_t *keyboard = NULL;

int32_t content_height_with_kb;
int32_t content_height_without_kb;

/**
 * Static prototypes
 */

/**
 * Handle LV_EVENT_CLICKED events from the theme toggle button.
 *
 * @param event the event object
 */
static void toggle_theme_btn_clicked_cb(lv_event_t *event);

/**
 * Toggle between the light and dark theme.
 */
static void toggle_theme(void);

/**
 * Set the UI theme.
 *
 * @param is_alternate true if the alternate theme should be applied, false if the default theme should be applied
 */
static void set_theme(bool is_alternate);

/**
 * Get the UI theme.
 *
 * @param is_alternate true if the alternate theme should be selected, false if the default theme should be selected
 */
static const bbx_theme * get_theme(bool is_alternate);

/**
 * Show the intro screen.
 *
 */
static void show_intro_screen(void);

/**
 * Handle LV_EVENT_KEY events from the intro 'get started' button.
 *
 * @param event the event object
 */
static void get_started_btn_clicked_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_KEY events on intro screen.
 *
 * @param event the event object
 */
static void intro_key_cb(lv_event_t *event);

/**
 * Show form screen with a field
 */
static void show_form_screen(void);

/**
 * Handle LV_EVENT_CLICKED events from the form field submit.
 *
 * @param event the event object
 */
static void field_submit_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_CLICKED events from the form 'previous' button.
 *
 * @param event the event object
 */
static void prev_btn_clicked_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_CLICKED events from the form 'next' button.
 *
 * @param event the event object
 */
 static void next_btn_clicked_cb(lv_event_t *event);

 /**
 * Validate a form field according to its requirements.
 *
 * @param field_index index of the field to validate
 * @return true if field is valid, false otherwise
 */
static bool validate_field(int field_index);

/**
 * Set visual validation error state on the current textarea.
 *
 * @param has_error true to show error styling, false to clear it
 */
static void set_field_validation_error(bool has_error);

/**
 * Save current textarea content and validate the field.
 *
 * @return true if field is valid, false if validation failed
 */
static bool save_and_validate_current_field(void);

/**
 * Handle LV_EVENT_CLICKED events from the show/hide keyboard toggle button.
 *
 * @param event the event object
 */
static void toggle_kb_btn_clicked_cb(lv_event_t *event);

/**
 * Toggle between showing and hiding the keyboard.
 */
static void toggle_keyboard_hidden(void);

/**
 * Show / hide the keyboard.
 *
 * @param is_hidden true if the keyboard should be hidden, false if it should be shown
 */
static void set_keyboard_hidden(bool is_hidden);

/**
 * Handle LV_EVENT_CLICKED events from password visibility toggle button.
 *
 * @param event the event object
 */
static void toggle_password_btn_clicked_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_CLICKED events from password visibility toggle button in
 * summary.
 *
 * @param event the event object
 */
static void toggle_password_summary_btn_clicked_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_VALUE_CHANGED events from the keyboard layout dropdown.
 *
 * @param event the event object
 */
static void layout_dropdown_value_changed_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_CLICKED events from the shutdown button.
 *
 * @param event the event object
 */
static void shutdown_btn_clicked_cb(lv_event_t *event);

/**
 * Handle confirmation events from the shutdown message box.
 *
 * @param event the event object
 */
static void shutdown_mbox_confirmed_cb(lv_event_t *event);

/**
 * Handle declination events from the shutdown message box.
 *
 * @param event the event object
 */
static void shutdown_mbox_declined_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_VALUE_CHANGED events from the keyboard widget.
 *
 * @param event the event object
 */
static void keyboard_value_changed_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_READY events from the keyboard widget.
 *
 * @param event the event object
 */
static void keyboard_ready_cb(lv_event_t *event);

/**
 * Show summary screen with all field values.
 */
static void show_summary_screen(void);

/**
 * Handle LV_EVENT_CLICKED events from summary field items.
 *
 * @param event the event object
 */
static void summary_field_clicked_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_CLICKED events from finish button.
 *
 * @param event the event object
 */
static void finish_cb(lv_event_t *event);

/**
 * Shuts down the device.
 */
static void shutdown(void);

/**
 * Handle termination signals sent to the process.
 *
 * @param signum the signal's number
 */
static void sigaction_handler(int signum);

/**
 * Restore the terminal and exit from the program with EXIT_FAILURE.
 */
static void exit_failure();

/**
 * Static functions
 */

 static void intro_key_cb(lv_event_t *event) {
    uint32_t key = lv_indev_get_key(lv_indev_active());
    if (key == LV_KEY_ENTER) {
        get_started_btn_clicked_cb(event);
    }
}

static void toggle_theme_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    toggle_theme();
}

static void shutdown(void) {
    sync();
    reboot(RB_POWER_OFF);
}

static void sigaction_handler(int signum) {
    bbx_terminal_reset_current_terminal();
    exit(signum);
}

static void exit_failure() {
    bbx_terminal_reset_current_terminal();
    exit(EXIT_FAILURE);
}


static void toggle_theme(void) {
    is_alternate_theme = !is_alternate_theme;
    set_theme(is_alternate_theme);
}

static void set_theme(bool is_alternate) {
    bbx_theme_apply(get_theme(is_alternate));
}

static const bbx_theme * get_theme(bool is_alternate) {
    return bbx_themes_themes[is_alternate ? conf_opts.theme.alternate_id : conf_opts.theme.default_id];
}

static void toggle_kb_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    toggle_keyboard_hidden();
}

static void toggle_keyboard_hidden(void) {
    is_keyboard_hidden = !is_keyboard_hidden;
    set_keyboard_hidden(is_keyboard_hidden);
}

static void set_keyboard_hidden(bool is_hidden) {
    if (!form_container) return;

    lv_anim_t container_anim;
    lv_anim_init(&container_anim);
    lv_anim_set_var(&container_anim, form_container);
    lv_anim_set_exec_cb(&container_anim, (lv_anim_exec_xcb_t) lv_obj_set_height);
    lv_anim_set_path_cb(&container_anim, lv_anim_path_ease_out);
    lv_anim_set_time(&container_anim, 500);

    lv_anim_set_values(&container_anim,
        is_hidden? content_height_with_kb : content_height_without_kb,
        is_hidden? content_height_without_kb : content_height_with_kb);

    lv_anim_start(&container_anim);
}

static void toggle_password_btn_clicked_cb(lv_event_t *event) {
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *textarea = (lv_obj_t *)lv_obj_get_user_data(btn);
    lv_obj_t *btn_label = lv_obj_get_child(btn, 0);

    bool is_obscured = lv_textarea_get_password_mode(textarea);
    lv_textarea_set_password_mode(textarea, !is_obscured);
    lv_label_set_text(btn_label, is_obscured ? LV_SYMBOL_EYE_CLOSE : LV_SYMBOL_EYE_OPEN);
}

static void toggle_password_summary_btn_clicked_cb(lv_event_t *event) {
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *value_label = (lv_obj_t *)lv_obj_get_user_data(btn);
    lv_obj_t *btn_label = lv_obj_get_child(btn, 0);

    const char *current_text = lv_label_get_text(value_label);
    bool is_obscured = (strcmp(current_text, F0_PASSWORD_HIDDEN_DOTS) == 0);

    if (is_obscured) {
        /* Show actual password - need field index to get real value */
        int field_index = (int)(intptr_t)lv_obj_get_user_data(value_label);
        const char *real_value = field_values[field_index] ? field_values[field_index] : "";
        lv_label_set_text(value_label, real_value);
        lv_label_set_text(btn_label, LV_SYMBOL_EYE_CLOSE);
    } else {
        lv_label_set_text(value_label, F0_PASSWORD_HIDDEN_DOTS);
        lv_label_set_text(btn_label, LV_SYMBOL_EYE_OPEN);
    }
}

static void layout_dropdown_value_changed_cb(lv_event_t *event) {
    lv_obj_t *dropdown = lv_event_get_target(event);
    uint16_t idx = lv_dropdown_get_selected(dropdown);
    sq2lv_switch_layout(keyboard, idx);
}

static void shutdown_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    lv_obj_t *mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "Shutdown device?");
    lv_obj_t *confirm_btn = lv_msgbox_add_footer_button(mbox, "Yes");
    lv_obj_add_event_cb(confirm_btn, shutdown_mbox_confirmed_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *decline_btn = lv_msgbox_add_footer_button(mbox, "No");
    lv_obj_add_event_cb(decline_btn, shutdown_mbox_declined_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(mbox, 400, LV_SIZE_CONTENT);
    lv_obj_center(mbox);
}

static void shutdown_mbox_confirmed_cb(lv_event_t *event) {
    LV_UNUSED(event);
    shutdown();
}

static void shutdown_mbox_declined_cb(lv_event_t *event) {
    /* Find the containing message box for the clicked button */
    lv_obj_t *obj = lv_event_get_target(event);
    while (obj && !lv_obj_check_type(obj, &lv_msgbox_class)) {
        obj = lv_obj_get_parent(obj);
    }

    if (!obj) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not find containing message box for clicked button");
        return;
    }

    lv_msgbox_close(obj);
}

static void keyboard_value_changed_cb(lv_event_t *event) {
    lv_obj_t *kb = lv_event_get_target(event);

    uint16_t btn_id = lv_buttonmatrix_get_selected_button(kb);
    if (btn_id == LV_BUTTONMATRIX_BUTTON_NONE) {
        return;
    }

    if (sq2lv_is_layer_switcher(kb, btn_id)) {
        sq2lv_switch_layer(kb, btn_id);
        return;
    }

    lv_keyboard_def_event_cb(event);
}

static void keyboard_ready_cb(lv_event_t *event) {
    field_submit_cb(event);
}

static void get_started_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    current_field_index = 0;
    show_form_screen();
}

static void field_submit_cb(lv_event_t *event) {
    LV_UNUSED(event);
    if (save_and_validate_current_field()) {
        current_field_index++;
        if (current_field_index >= conf_opts.num_fields) {
            show_summary_screen();
        } else {
            show_form_screen();
        }
    }
}

static void prev_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    if (current_field_index > 0 && save_and_validate_current_field()) {
        current_field_index--;
        show_form_screen();
    }
}

static void next_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    if (save_and_validate_current_field()) {
        current_field_index++;
        if (current_field_index >= conf_opts.num_fields) {
            show_summary_screen();
        } else {
            show_form_screen();
        }
    }
}

static void summary_field_clicked_cb(lv_event_t *event) {
    lv_obj_t *target = lv_event_get_target(event);
    int field_index = (int)(intptr_t)lv_obj_get_user_data(target);

    current_field_index = field_index;
    show_form_screen();
}

static void finish_cb(lv_event_t *event) {
    LV_UNUSED(event);

    /* Output key=value pairs */
    for (int i = 0; i < conf_opts.num_fields; i++) {
        const f0_form_field_t *field = &conf_opts.fields[i];
        const char *value = field_values[i] ? field_values[i] : "";
        printf("%s=%s\n", field->name, value);
    }

    /* Clear the screen so that form data cannot leak via stale display buffers */
    lv_obj_t *rect = lv_obj_create(lv_screen_active());
    lv_obj_set_size(rect, LV_PCT(100), LV_PCT(100));
    lv_obj_set_pos(rect, 0, 0);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(rect, lv_color_hex(get_theme(is_alternate_theme)->window.bg_color), LV_PART_MAIN);
    lv_refr_now(lv_display_get_default()); /* Force the screen to be drawn */

    /* Trigger SIGTERM to exit */
    sigaction_handler(SIGTERM);

    exit(0);
}

static bool validate_field(int field_index) {
    if (field_index >= conf_opts.num_fields) return true;

    const f0_form_field_t *field = &conf_opts.fields[field_index];
    const char *value = field_values[field_index] ? field_values[field_index] : "";

    // Required field validation
    if (field->required && strlen(value) == 0) {
        return false;
    }

    // Future validation rules here, e.g.:
    // if (field->min_length > 0 && strlen(value) < field->min_length) return false;

    return true;
}

static void set_field_validation_error(bool has_error) {
    if (!form_textarea) return;

    if (has_error) {
        lv_obj_set_style_border_color(form_textarea, lv_color_hex(0xFF0000), LV_PART_MAIN);
        lv_obj_set_style_border_width(form_textarea, 2, LV_PART_MAIN);
    } else {
        lv_obj_set_style_border_color(form_textarea, lv_color_hex(0x000000), LV_PART_MAIN);
        lv_obj_set_style_border_width(form_textarea, 1, LV_PART_MAIN);
    }
}

static bool save_and_validate_current_field() {
    if (!form_textarea || !field_values) return true;

    // Save current content
    char *new_value = strdup(lv_textarea_get_text(form_textarea));
    if (new_value) {
        if (field_values[current_field_index]) {
            free(field_values[current_field_index]);
        }
        field_values[current_field_index] = new_value;
    }

    // Validate and show/clear error
    bool is_valid = validate_field(current_field_index);
    set_field_validation_error(!is_valid);

    return is_valid;
}

static void show_intro_screen(void) {
    lv_obj_t *screen = lv_screen_active();

    /* Header config */
    bbx_header_config_t header_config;
    bbx_header_init_config(&header_config);
    /* No OSK needed on intro */
    header_config.show_keyboard_toggle = false;
    header_config.show_layout_dropdown = false;

    bbx_header_widgets_t header_widgets;
    lv_obj_t *header = bbx_header_create(screen, &header_config, &header_widgets);
    lv_obj_add_flag(header, BBX_WIDGET_HEADER);
    lv_theme_apply(header);

    /* Attach callbacks */
    lv_obj_add_event_cb(header_widgets.theme_toggle_btn, toggle_theme_btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    /* Container for intro content */
    lv_obj_t *container = lv_obj_create(screen);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(container, 40, LV_PART_MAIN);

    /* Title */
    lv_obj_t *title = lv_label_create(container);
    lv_label_set_text(title, conf_opts.intro.title);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(title, 20, LV_PART_MAIN);

    /* Body */
    lv_obj_t *body = lv_label_create(container);
    lv_label_set_text(body, conf_opts.intro.body);
    lv_label_set_long_mode(body, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(body, LV_PCT(80));
    lv_obj_set_style_text_align(body, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(body, 30, LV_PART_MAIN);

    /* Get Started button */
    lv_obj_t *btn = lv_button_create(container);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Get Started");
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, get_started_btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    /* Set up keyboard input for intro screen */
    lv_group_remove_all_objs(keyboard_input_group);
    lv_group_add_obj(keyboard_input_group, btn);
    lv_obj_add_event_cb(btn, intro_key_cb, LV_EVENT_KEY, NULL);
  }

static void show_form_screen(void) {
    if (current_field_index >= conf_opts.num_fields) {
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Form complete");
        return;
    }

    if (field_values == NULL) {
        field_values = calloc(conf_opts.num_fields, sizeof(char*));
    }

    f0_form_field_t *field = &conf_opts.fields[current_field_index];

    /* Clear screen */
    lv_obj_clean(lv_screen_active());

    /* Recreate header */
    bbx_header_config_t header_config;
    bbx_header_init_config(&header_config);
    header_config.show_keyboard_toggle = true;
    header_config.show_layout_dropdown = true;
    header_config.dropdown_options = sq2lv_layout_short_names;

    bbx_header_widgets_t header_widgets;
    lv_obj_t *header = bbx_header_create(lv_screen_active(), &header_config, &header_widgets);
    lv_obj_add_flag(header, BBX_WIDGET_HEADER);
    lv_theme_apply(header);

    /* Attach callbacks */
    lv_obj_add_event_cb(header_widgets.theme_toggle_btn, toggle_theme_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(header_widgets.keyboard_toggle_btn, toggle_kb_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(header_widgets.layout_dropdown, layout_dropdown_value_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(header_widgets.shutdown_btn, shutdown_btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    /* Calculate keyboard dimensions and container heights */
    lv_obj_update_layout(header);
    const int32_t hor_res = lv_display_get_horizontal_resolution(lv_display_get_default());
    const int32_t ver_res = lv_display_get_vertical_resolution(lv_display_get_default());
    const int32_t keyboard_height = ver_res > hor_res ? ver_res / 2.5 : ver_res / 1.8;

    content_height_without_kb = ver_res - lv_obj_get_height(header);
    content_height_with_kb = content_height_without_kb - keyboard_height;

    /* Form container - vertical layout */
    form_container = lv_obj_create(lv_screen_active());
    lv_obj_set_flex_flow(form_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(form_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(form_container, 40, LV_PART_MAIN);
    lv_obj_remove_flag(form_container, LV_OBJ_FLAG_SCROLLABLE);

    /* Field label */
    lv_obj_t *label = lv_label_create(form_container);
    lv_label_set_text(label, field->label ? field->label : field->name);
    lv_obj_set_style_pad_bottom(label, 20, LV_PART_MAIN);

    /* Textarea container */
    lv_obj_t *textarea_container = lv_obj_create(form_container);
    lv_obj_set_size(textarea_container, LV_PCT(80), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(textarea_container, LV_FLEX_FLOW_ROW);

    /* Create textarea and restore content */
    form_textarea = lv_textarea_create(textarea_container);
    lv_textarea_set_one_line(form_textarea, true);
    if (field->type == F0_FIELD_TYPE_PASSWORD) {
        lv_textarea_set_password_mode(form_textarea, true);
        lv_textarea_set_password_bullet(form_textarea, conf_opts.textarea.bullet);
    }
    lv_obj_add_event_cb(form_textarea, field_submit_cb, LV_EVENT_READY, NULL);
    lv_obj_set_flex_grow(form_textarea, 1);
    lv_obj_add_state(form_textarea, LV_STATE_FOCUSED);

    /* Reveal / obscure password button */
    if (field->type == F0_FIELD_TYPE_PASSWORD) {
        lv_obj_update_layout(form_textarea);
        const int32_t textarea_height = lv_obj_get_height(form_textarea);

        lv_obj_t *toggle_pw_btn = lv_button_create(textarea_container);
        lv_obj_set_size(toggle_pw_btn, textarea_height, textarea_height);
        lv_obj_t *toggle_pw_btn_label = lv_label_create(toggle_pw_btn);
        lv_obj_center(toggle_pw_btn_label);
        lv_label_set_text(toggle_pw_btn_label, LV_SYMBOL_EYE_OPEN);
        lv_obj_set_user_data(toggle_pw_btn, form_textarea);
        lv_obj_add_event_cb(toggle_pw_btn, toggle_password_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    }

    /* Restore saved content */
    if (field_values[current_field_index]) {
        lv_textarea_set_text(form_textarea, field_values[current_field_index]);
        /* Prevent text from being scrolled off to the left of the textarea */
        lv_obj_update_layout(form_textarea);
        lv_obj_scroll_to_x(form_textarea, 0, LV_ANIM_OFF);
    }

    /* Navigation bar at bottom */
    lv_obj_t *nav_container = lv_obj_create(lv_screen_active());
    lv_obj_set_size(nav_container, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(nav_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(nav_container, 10, LV_PART_MAIN);

    /* Previous button */
    lv_obj_t *prev_btn = lv_button_create(nav_container);
    lv_obj_t *prev_label = lv_label_create(prev_btn);
    lv_label_set_text(prev_label, "Previous");
    lv_obj_center(prev_label);
    lv_obj_add_event_cb(prev_btn, prev_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    if (current_field_index == 0) {
        lv_obj_add_flag(prev_btn, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_remove_flag(prev_btn, LV_OBJ_FLAG_HIDDEN);
    }

    /* Spacer to push Next to the right */
    lv_obj_t *spacer = lv_obj_create(nav_container);
    lv_obj_set_flex_grow(spacer, 1);
    lv_obj_set_height(spacer, 0);

    /* Next button */
    lv_obj_t *next_btn = lv_button_create(nav_container);
    lv_obj_t *next_label = lv_label_create(next_btn);
    lv_label_set_text(next_label, current_field_index >= conf_opts.num_fields - 1 ? "Submit" : "Next");
    lv_obj_center(next_label);
    lv_obj_add_event_cb(next_btn, next_btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    /* Update layout to get nav bar height */
    lv_obj_update_layout(nav_container);
    const int32_t nav_height = lv_obj_get_height(nav_container);

    /* Adjust content heights for nav bar */
    content_height_without_kb -= nav_height;
    content_height_with_kb -= nav_height;

    /* Update form container size to account for nav bar */
    lv_obj_set_size(form_container, LV_PCT(100), is_keyboard_hidden ? content_height_without_kb : content_height_with_kb);

    /* Create keyboard widget */
    bbx_keyboard_config_t keyboard_config;
    bbx_keyboard_init_config(&keyboard_config);
    keyboard_config.layout_id = (int)conf_opts.keyboard.layout_id;
    keyboard_config.height = keyboard_height;
    keyboard_config.popovers = conf_opts.keyboard.popovers;
    keyboard_config.value_changed_callback = keyboard_value_changed_cb;
    keyboard_config.ready_callback = keyboard_ready_cb;
    keyboard = bbx_keyboard_create(lv_screen_active(), form_textarea, &keyboard_config);

    /* Configuring routing for physical keyboard input into the textarea */
    lv_group_remove_all_objs(keyboard_input_group);
    lv_group_add_obj(keyboard_input_group, form_textarea);
}

static void show_summary_screen(void) {
    /* Clear screen */
    lv_obj_clean(lv_screen_active());

    /* Header */
    bbx_header_config_t header_config;
    bbx_header_init_config(&header_config);
    header_config.show_keyboard_toggle = false;
    header_config.show_layout_dropdown = false;

    bbx_header_widgets_t header_widgets;
    lv_obj_t *header = bbx_header_create(lv_screen_active(), &header_config, &header_widgets);
    lv_obj_add_flag(header, BBX_WIDGET_HEADER);
    lv_theme_apply(header);
    lv_obj_add_event_cb(header_widgets.theme_toggle_btn, toggle_theme_btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    /* Summary container */
    lv_obj_t *container = lv_obj_create(lv_screen_active());
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(80));
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(container, 20, LV_PART_MAIN);

    /* Title */
    lv_obj_t *title = lv_label_create(container);
    lv_label_set_text(title, "Summary");
    lv_obj_set_style_pad_bottom(title, 20, LV_PART_MAIN);

    /* Field list */
    for (int i = 0; i < conf_opts.num_fields; i++) {
        f0_form_field_t *field = &conf_opts.fields[i];
        const char *value = field_values[i] ? field_values[i] : "";

        lv_obj_t *item_btn = lv_button_create(container);
        lv_obj_set_width(item_btn, LV_PCT(100));
        lv_obj_set_user_data(item_btn, (void*)(intptr_t)i);
        lv_obj_add_event_cb(item_btn, summary_field_clicked_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *item_container = lv_obj_create(item_btn);
        lv_obj_set_flex_flow(item_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_width(item_container, LV_PCT(100));
        lv_obj_add_flag(item_container, LV_OBJ_FLAG_EVENT_BUBBLE);

        lv_obj_t *label = lv_label_create(item_container);
        lv_label_set_text(label, field->label ? field->label : field->name);
        lv_obj_set_style_text_decor(label, LV_TEXT_DECOR_UNDERLINE, LV_PART_MAIN);

        lv_obj_t *value_label = lv_label_create(item_container);
        if (strlen(value) > 0) {
            if (field->type == F0_FIELD_TYPE_PASSWORD) {
                lv_label_set_text(value_label, F0_PASSWORD_HIDDEN_DOTS);

                /* Add toggle button for password */
                lv_obj_t *toggle_btn = lv_button_create(item_container);
                lv_obj_set_size(toggle_btn, 40, 30);
                lv_obj_set_user_data(toggle_btn, value_label);

                lv_obj_t *toggle_label = lv_label_create(toggle_btn);
                lv_label_set_text(toggle_label, LV_SYMBOL_EYE_OPEN);
                lv_obj_center(toggle_label);
                lv_obj_add_event_cb(toggle_btn, toggle_password_summary_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
            } else {
                lv_label_set_text(value_label, value);
            }
        } else {
            lv_label_set_text(value_label, "(empty)");
        }

        /* Save field index to obj to reference later in summary_field_click_cb */
        lv_obj_set_user_data(item_container, (void*)(intptr_t)i);
        lv_obj_set_user_data(label, (void*)(intptr_t)i);
        lv_obj_set_user_data(value_label, (void*)(intptr_t)i);
    }

    /* Spacer for button positioning */
    lv_obj_t *spacer = lv_obj_create(container);
    lv_obj_set_height(spacer, 20);
    lv_obj_set_width(spacer, 0);

    /* Button container for centering */
    lv_obj_t *button_container = lv_obj_create(container);
    lv_obj_set_size(button_container, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(button_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(button_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Finish button */
    lv_obj_t *finish_btn = lv_button_create(button_container);
    lv_obj_t *finish_label = lv_label_create(finish_btn);
    lv_label_set_text(finish_label, "Finish");
    lv_obj_center(finish_label);
    lv_obj_add_event_cb(finish_btn, finish_cb, LV_EVENT_CLICKED, NULL);

    /* Set up keyboard input for summary screen */
    lv_group_remove_all_objs(keyboard_input_group);
    lv_group_add_obj(keyboard_input_group, finish_btn);
}

int main(int argc, char *argv[]) {
    f0_cli_parse_opts(argc, argv, &cli_opts);

    if (cli_opts.common.verbose) {
        bbx_log_set_level(BBX_LOG_LEVEL_VERBOSE);
    }

    /* Parse config files */
    f0_config_init_opts(&conf_opts);
    f0_config_parse_file("/usr/share/f0rmz/f0rmz.conf", &conf_opts);
    f0_config_parse_directory("/usr/share/f0rmz/f0rmz.conf.d", &conf_opts);
    f0_config_parse_file("/etc/f0rmz.conf", &conf_opts);
    f0_config_parse_directory("/etc/f0rmz.conf.d", &conf_opts);
    f0_config_parse_file(cli_opts.config_file, &conf_opts);

    /* Prepare current TTY and clean up on termination */
    bbx_terminal_prepare_current_terminal(!conf_opts.quirks.terminal_prevent_graphics_mode, !conf_opts.quirks.terminal_allow_keyboard_input);
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sigaction_handler;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    /* Initialize LVGL */
    lv_init();
    lv_log_register_print_cb(bbx_log_print_cb);

    /* Populate display config */
    bbx_display_config_t display_config = {
        .hor_res = cli_opts.common.hor_res,
        .ver_res = cli_opts.common.ver_res,
        .x_offset = cli_opts.common.x_offset,
        .y_offset = cli_opts.common.y_offset,
        .dpi = cli_opts.common.dpi,
        .fbdev_force_refresh = conf_opts.quirks.fbdev_force_refresh
    };

    /* Initialize display */
    lv_display_t *disp = bbx_display_create(conf_opts.general.backend, &display_config);
    if (!disp) {
        exit_failure();
    }

    int fd_epoll = epoll_create1(EPOLL_CLOEXEC);
    if (fd_epoll == -1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "epoll_create1() is failed");
        exit_failure();
    }

    /* Attach input devices and start monitoring for new ones */
    struct bbx_indev_opts input_config = {
        .keymap = &conf_opts.hw_keyboard,
        .keyboard = conf_opts.input.keyboard,
        .pointer = conf_opts.input.pointer,
        .touchscreen = conf_opts.input.touchscreen
    };
    if (bbx_indev_init(fd_epoll, &input_config) == 0)
        exit_failure();

    bbx_indev_set_key_power_cb(shutdown);

    /* Hide the on-screen keyboard by default if a physical keyboard is connected */
    if (conf_opts.keyboard.autohide && bbx_indev_is_keyboard_connected()) {
        is_keyboard_hidden = true;
    }

    /* Prevent scrolling when keyboard is off-screen */
    lv_obj_t *screen = lv_screen_active();
    lv_theme_apply(screen);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* Initialise theme */
    set_theme(is_alternate_theme);

    /* Create header */
    show_intro_screen();

    /* Main loop */
    while(1) {
        uint32_t time_till_next = lv_timer_handler();

        struct epoll_event event;
        int r = epoll_wait(fd_epoll, &event, 1, time_till_next);
        if (r == 0)
            continue;
        if (r > 0) {
            __extension__ void (*handler)() = event.data.ptr;
            handler();
            continue;
        }
        if (errno == EINTR)
            continue;

        bbx_log(BBX_LOG_LEVEL_ERROR, "epoll_wait() is failed");
        exit_failure();
    }

    return 0;
}
