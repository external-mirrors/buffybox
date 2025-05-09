/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "backends.h"
#include "command_line.h"
#include "config.h"
#include "unl0kr.h"
#include "terminal.h"

#include "../shared/indev.h"
#include "../shared/log.h"
#include "../shared/theme.h"
#include "../shared/themes.h"
#include "../squeek2lvgl/sq2lv.h"

#include "lvgl/lvgl.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/time.h>


/**
 * Static variables
 */

ul_cli_opts cli_opts;
ul_config_opts conf_opts;

bool is_alternate_theme = false;
bool is_password_obscured = true;
bool is_keyboard_hidden = false;

lv_obj_t *container = NULL;
lv_obj_t *keyboard = NULL;

int32_t content_height_with_kb;
int32_t content_height_without_kb;
int32_t content_pad_bottom_with_kb;
int32_t content_pad_bottom_without_kb;

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
 * Handle LV_EVENT_CLICKED events from the show/hide password toggle button.
 *
 * @param event the event object
 */
static void toggle_pw_btn_clicked_cb(lv_event_t *event);

/**
 * Toggle between showing and hiding the password.
 */
static void toggle_password_obscured(void);

/**
 * Show / hide the password.
 *
 * @param is_hidden true if the password should be hidden, false if it should be shown
 */
static void set_password_obscured(bool is_obscured);

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
 * Show / hide the keyboard
 *
 * @param is_hidden true if the keyboard should be hidden, false if it should be shown
 */
static void set_keyboard_hidden(bool is_hidden);

/**
 * Callback for the pad animation.
 *
 * @param obj container widget
 * @param value the current value of the pad
 */

static void pad_anim_cb(void *obj, int32_t value);

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
 * Handle LV_EVENT_READY events from the textarea widget.
 *
 * @param event the event object
 */
static void textarea_ready_cb(lv_event_t *event);

/**
 * Print out the entered password and exit.
 *
 * @param textarea the textarea widget
 */
static void print_password_and_exit(lv_obj_t *textarea);

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
 * Static functions
 */

static void toggle_theme_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    toggle_theme();
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

static void toggle_pw_btn_clicked_cb(lv_event_t *event) {
    LV_UNUSED(event);
    toggle_password_obscured();
}

static void toggle_password_obscured(void) {
    is_password_obscured = !is_password_obscured;
    set_password_obscured(is_password_obscured);
}

static void set_password_obscured(bool is_obscured) {
    lv_obj_t *textarea = lv_keyboard_get_textarea(keyboard);
    lv_textarea_set_password_mode(textarea, is_obscured);
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
    if (!conf_opts.general.animations) {
        lv_obj_set_height(container, is_hidden? content_height_without_kb : content_height_with_kb);
        lv_obj_set_style_pad_bottom(container,
            is_hidden? content_pad_bottom_without_kb : content_pad_bottom_with_kb, LV_PART_MAIN);
        return;
    }

    lv_anim_t keyboard_anim;
    lv_anim_init(&keyboard_anim);
    lv_anim_set_var(&keyboard_anim, container);
    lv_anim_set_exec_cb(&keyboard_anim, (lv_anim_exec_xcb_t) lv_obj_set_height);
    lv_anim_set_path_cb(&keyboard_anim, lv_anim_path_ease_out);
    lv_anim_set_time(&keyboard_anim, 500);

    lv_anim_set_values(&keyboard_anim,
        is_hidden? content_height_with_kb : content_height_without_kb,
        is_hidden? content_height_without_kb : content_height_with_kb);

    lv_anim_start(&keyboard_anim);

    if (content_pad_bottom_with_kb != content_pad_bottom_without_kb) {
        lv_anim_t pad_anim;
        lv_anim_init(&pad_anim);
        lv_anim_set_var(&pad_anim, container);
        lv_anim_set_exec_cb(&pad_anim, pad_anim_cb);
        lv_anim_set_path_cb(&pad_anim, lv_anim_path_ease_out);
        lv_anim_set_time(&pad_anim, 500);

        lv_anim_set_values(&pad_anim,
            is_hidden? content_pad_bottom_with_kb : content_pad_bottom_without_kb,
            is_hidden? content_pad_bottom_without_kb : content_pad_bottom_with_kb);

        lv_anim_start(&pad_anim);
    }
}

static void pad_anim_cb(void *obj, int32_t value) {
    lv_obj_set_style_pad_bottom(obj, value, LV_PART_MAIN);
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
    print_password_and_exit(lv_keyboard_get_textarea(lv_event_get_target(event)));
}

static void textarea_ready_cb(lv_event_t *event) {
    print_password_and_exit(lv_event_get_target(event));
}

static void print_password_and_exit(lv_obj_t *textarea) {
    /* Print the password to STDOUT */
    printf(cli_opts.newline? "%s\n" : "%s", lv_textarea_get_text(textarea));

    /* Clear the screen so that when the password field was unobscured, it cannot
     * leak via stale display buffers after we've exited */
    lv_obj_t *rect = lv_obj_create(lv_screen_active());
    lv_obj_set_size(rect, LV_PCT(100), LV_PCT(100));
    lv_obj_set_pos(rect, 0, 0);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(rect , lv_color_hex(get_theme(is_alternate_theme)->window.bg_color), LV_PART_MAIN);
    lv_refr_now(lv_display_get_default()); /* Force the screen to be drawn */

    /* Trigger SIGTERM to exit */
    sigaction_handler(SIGTERM);
}

static void shutdown(void) {
    sync();
    reboot(RB_POWER_OFF);
}

static void sigaction_handler(int signum) {
    LV_UNUSED(signum);
    ul_terminal_reset_current_terminal();
    exit(0);
}


/**
 * Main
 */

int main(int argc, char *argv[]) {
    /* Parse command line options */
    ul_cli_parse_opts(argc, argv, &cli_opts);

    /* Set up log level */
    if (cli_opts.verbose) {
        bbx_log_set_level(BBX_LOG_LEVEL_VERBOSE);
    }

    /* Announce ourselves */
    bbx_log(BBX_LOG_LEVEL_VERBOSE, "unl0kr %s", PROJECT_VERSION);

    /* Check that we have access to the clock */
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Unable to read the clock");
        exit(EXIT_FAILURE);
    }

    /* Parse config files */
    ul_config_init_opts(&conf_opts);
    ul_config_parse_file("/usr/share/unl0kr/unl0kr.conf", &conf_opts);
    ul_config_parse_directory("/usr/share/unl0kr/unl0kr.conf.d", &conf_opts);
    ul_config_parse_file("/etc/unl0kr.conf", &conf_opts);
    ul_config_parse_directory("/etc/unl0kr.conf.d", &conf_opts);
    ul_config_parse_files(cli_opts.config_files, cli_opts.num_config_files, &conf_opts);

    /* Prepare current TTY and clean up on termination */
    ul_terminal_prepare_current_terminal(!conf_opts.quirks.terminal_prevent_graphics_mode, !conf_opts.quirks.terminal_allow_keyboard_input);
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sigaction_handler;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    /* Initialise LVGL and set up logging callback */
    lv_init();
    lv_log_register_print_cb(bbx_log_print_cb);

    /* Initialise display */
    lv_display_t *disp = NULL;
    switch (conf_opts.general.backend) {
#if LV_USE_LINUX_FBDEV
    case UL_BACKENDS_BACKEND_FBDEV:
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Using framebuffer backend");
        disp = lv_linux_fbdev_create();
        lv_linux_fbdev_set_file(disp, "/dev/fb0");
        if (conf_opts.quirks.fbdev_force_refresh) {
            lv_linux_fbdev_set_force_refresh(disp, true);
        }
        break;
#endif /* LV_USE_LINUX_FBDEV */
#if LV_USE_LINUX_DRM
    case UL_BACKENDS_BACKEND_DRM:
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Using DRM backend");
        disp = lv_linux_drm_create();

        char *format_string = "/dev/dri/card%d";
        char drm_path[16] = {0};
        struct stat buffer;

        for (size_t i = 0; i < 9; i++) {
            sprintf(drm_path, format_string, i);

            if (stat(drm_path, &buffer) != 0) {
                continue;
            }

            lv_linux_drm_set_file(disp, drm_path, -1);

            break;
        }

        break;
#endif /* LV_USE_LINUX_DRM */
    default:
        bbx_log(BBX_LOG_LEVEL_ERROR, "Unable to find suitable backend");
        exit(EXIT_FAILURE);
    }

    /* Override display properties with command line options if necessary */
    lv_display_set_offset(disp, cli_opts.x_offset, cli_opts.y_offset);
    if (cli_opts.hor_res > 0 || cli_opts.ver_res > 0) {
        lv_display_set_physical_resolution(disp, lv_display_get_horizontal_resolution(disp), lv_display_get_vertical_resolution(disp));
        lv_display_set_resolution(disp, cli_opts.hor_res, cli_opts.ver_res);
    }
    if (cli_opts.dpi > 0) {
        lv_display_set_dpi(disp, cli_opts.dpi);
    }

    /* Prepare for routing physical keyboard input into the textarea */
    lv_group_t *keyboard_input_group = lv_group_create();
    bbx_indev_set_keyboard_input_group(keyboard_input_group);

    /* Start input device monitor and auto-connect available devices */
    bbx_indev_start_monitor_and_autoconnect(conf_opts.input.keyboard, conf_opts.input.pointer, conf_opts.input.touchscreen);

    /* Hide the on-screen keyboard by default if a physical keyboard is connected */
    if (conf_opts.keyboard.autohide && bbx_indev_is_keyboard_connected()) {
        is_keyboard_hidden = true;
    }

    /* Initialise theme */
    set_theme(is_alternate_theme);

    /* Figure out a few numbers for sizing and positioning */
    const int32_t hor_res = lv_display_get_horizontal_resolution(disp);
    const int32_t ver_res = lv_display_get_vertical_resolution(disp);
    const int32_t keyboard_height = ver_res > hor_res ? ver_res / 2.5 : ver_res / 1.8; /* Height for 5 rows */

    /* Prevent scrolling when keyboard is off-screen */
    lv_obj_t *screen = lv_screen_active();
    lv_theme_apply(screen);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* Header flexbox */
    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_add_flag(header, BBX_WIDGET_HEADER);
    lv_theme_apply(header); /* Force re-apply theme after setting flag so that the widget can be identified */
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_size(header, LV_PCT(100), LV_SIZE_CONTENT);

    /* Theme switcher button */
    lv_obj_t *toggle_theme_btn = lv_button_create(header);
    lv_obj_add_event_cb(toggle_theme_btn, toggle_theme_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *toggle_theme_btn_label = lv_label_create(toggle_theme_btn);
    lv_label_set_text(toggle_theme_btn_label, UL_SYMBOL_ADJUST);
    lv_obj_center(toggle_theme_btn_label);

    /* Show / hide keyboard button */
    lv_obj_t *toggle_kb_btn = lv_button_create(header);
    lv_obj_add_event_cb(toggle_kb_btn, toggle_kb_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *toggle_kb_btn_label = lv_label_create(toggle_kb_btn);
    lv_label_set_text(toggle_kb_btn_label, LV_SYMBOL_KEYBOARD);
    lv_obj_center(toggle_kb_btn_label);

    /* Keyboard layout dropdown */
    lv_obj_t *layout_dropdown = lv_dropdown_create(header);
    lv_dropdown_set_options(layout_dropdown, sq2lv_layout_short_names);
    lv_obj_add_event_cb(layout_dropdown, layout_dropdown_value_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_width(layout_dropdown, 90);

    /* Spacer */
    lv_obj_t *spacer = lv_obj_create(header);
    lv_obj_set_height(spacer, 0);
    lv_obj_set_flex_grow(spacer, 1);

    /* Shutdown button */
    lv_obj_t *shutdown_btn = lv_button_create(header);
    lv_obj_add_event_cb(shutdown_btn, shutdown_btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *shutdown_btn_label = lv_label_create(shutdown_btn);
    lv_label_set_text(shutdown_btn_label, LV_SYMBOL_POWER);
    lv_obj_center(shutdown_btn_label);

    lv_obj_update_layout(layout_dropdown);
    const int32_t dropwdown_height = lv_obj_get_height(layout_dropdown);
    lv_obj_set_size(toggle_theme_btn, dropwdown_height, dropwdown_height);
    lv_obj_set_size(toggle_kb_btn, dropwdown_height, dropwdown_height);
    lv_obj_set_size(shutdown_btn, dropwdown_height, dropwdown_height);

    lv_obj_update_layout(header);
    content_height_without_kb = ver_res - lv_obj_get_height(header);
    content_height_with_kb = content_height_without_kb - keyboard_height;

    /* Container for a message and an input field */
    container = lv_obj_create(screen);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_size(container, LV_PCT(100), is_keyboard_hidden? content_height_without_kb : content_height_with_kb);
    lv_obj_set_style_pad_top(container, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_left(container, 20, LV_PART_MAIN);
    lv_obj_set_style_pad_right(container, 20, LV_PART_MAIN);

    int32_t content_pad_row = 10;

    /* Message for a user */
    lv_obj_t *message_label = NULL;
    if (cli_opts.message) {
        lv_obj_set_style_pad_row(container, content_pad_row, LV_PART_MAIN);

        /* lv_label does not support wrapping and scrolling simultaneously,
           so we place it in a scrollable container */
        lv_obj_t *message_container = lv_obj_create(container);
        lv_obj_set_width(message_container, LV_PCT(100));
        lv_obj_set_flex_flow(message_container, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_grow(message_container, 1);

        lv_obj_t *message_spacer = lv_obj_create(message_container);
        lv_obj_set_width(message_spacer, 0);
        lv_obj_set_flex_grow(message_spacer, 1);

        message_label = lv_label_create(message_container);
        lv_obj_set_size(message_label, LV_PCT(100), LV_SIZE_CONTENT);
        lv_label_set_text(message_label, cli_opts.message);
    }

    /* Textarea flexbox */
    lv_obj_t *textarea_container = lv_obj_create(container);
    lv_obj_set_size(textarea_container, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(textarea_container, LV_FLEX_FLOW_ROW);

    /* Textarea */
    lv_obj_t *textarea = lv_textarea_create(textarea_container);
    lv_textarea_set_one_line(textarea, true);
    lv_textarea_set_password_mode(textarea, true);
    lv_textarea_set_password_bullet(textarea, conf_opts.textarea.bullet);
    lv_textarea_set_placeholder_text(textarea, "Enter password...");
    lv_obj_add_event_cb(textarea, textarea_ready_cb, LV_EVENT_READY, NULL);
    lv_obj_set_flex_grow(textarea, 1);
    lv_obj_add_state(textarea, LV_STATE_FOCUSED);

    /* Route physical keyboard input into textarea */
    lv_group_add_obj(keyboard_input_group, textarea);

    lv_obj_update_layout(textarea);
    const int32_t textarea_height = lv_obj_get_height(textarea);

    /* Reveal / obscure password button */
    lv_obj_t *toggle_pw_btn = lv_button_create(textarea_container);
    lv_obj_set_size(toggle_pw_btn, textarea_height, textarea_height);
    lv_obj_t *toggle_pw_btn_label = lv_label_create(toggle_pw_btn);
    lv_obj_center(toggle_pw_btn_label);
    lv_label_set_text(toggle_pw_btn_label, LV_SYMBOL_EYE_OPEN);
    lv_obj_add_event_cb(toggle_pw_btn, toggle_pw_btn_clicked_cb, LV_EVENT_CLICKED, NULL);

    /* The bottom pad is used to center content when the keyboard is hidden */
    content_pad_bottom_with_kb = 20;

    int32_t content_native_height = textarea_height;
    if (cli_opts.message) {
        lv_obj_update_layout(message_label);
        content_native_height += content_pad_row + lv_obj_get_height(message_label);
    }

    content_pad_bottom_without_kb = (content_height_without_kb - content_native_height) / 2;
    if (content_pad_bottom_without_kb < content_pad_bottom_with_kb)
        content_pad_bottom_without_kb = content_pad_bottom_with_kb;

    lv_obj_set_style_pad_bottom(container, is_keyboard_hidden? content_pad_bottom_without_kb : content_pad_bottom_with_kb, LV_PART_MAIN);

    /* Keyboard (after textarea / label so that key popovers are not drawn over) */
    keyboard = lv_keyboard_create(screen);
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_keyboard_set_textarea(keyboard, textarea);
    uint32_t num_keyboard_events = lv_obj_get_event_count(keyboard);
    for(uint32_t i = 0; i < num_keyboard_events; ++i) {
        if(lv_event_dsc_get_cb(lv_obj_get_event_dsc(keyboard, i)) == lv_keyboard_def_event_cb) {
            lv_obj_remove_event(keyboard, i);
            break;
        }
    }
    lv_obj_add_event_cb(keyboard, keyboard_value_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(keyboard, keyboard_ready_cb, LV_EVENT_READY, NULL);
    lv_obj_set_size(keyboard, LV_PCT(100), keyboard_height);
    bbx_theme_prepare_keyboard(keyboard);

    /* Apply textarea options */
    set_password_obscured(conf_opts.textarea.obscured);

    /* Apply keyboard options */
    sq2lv_switch_layout(keyboard, conf_opts.keyboard.layout_id);
    lv_dropdown_set_selected(layout_dropdown, conf_opts.keyboard.layout_id);
    if (conf_opts.keyboard.popovers) {
        lv_keyboard_set_popovers(keyboard, true);
    }

    /* Periodically run timer / task handler */
    uint32_t timeout = conf_opts.general.timeout * 1000; /* ms */
    while(1) {
        uint32_t time_till_next = lv_timer_handler();

        if (timeout != 0) {
            uint32_t time_idle = lv_display_get_inactive_time(NULL);
            if (time_idle >= timeout)
                shutdown();

            uint32_t time_till_shutdown = timeout - time_idle;
            if (time_till_shutdown < time_till_next)
                time_till_next = time_till_shutdown;
        }

        usleep(time_till_next * 1000);
    }

    return 0;
}
