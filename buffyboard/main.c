/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "buffyboard.h"
#include "command_line.h"
#include "indev.h"
#include "sq2lv_layouts.h"
#include "terminal.h"
#include "uinput_device.h"

#include "lvgl/lvgl.h"

#include "../squeek2lvgl/sq2lv.h"

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>


/**
 * Static variables
 */

bb_cli_opts cli_opts;

static bool resize_terminals = false;
static lv_obj_t *keyboard = NULL;
static lv_style_t style_text_normal;


/**
 * Static prototypes
 */

/**
 * Compute the denominator of the keyboard height factor. The keyboard height is calculated
 * by dividing the display height by the denominator.
 *
 * @param width display width
 * @param height display height
 * @return denominator
 */
static int keyboard_height_denominator(lv_coord_t width, lv_coord_t height);

/**
 * Handle termination signals sent to the process.
 *
 * @param signum the signal's number
 */
static void sigaction_handler(int signum);

/**
 * Callback for the terminal resizing timer.
 *
 * @param timer the timer object
 */
static void terminal_resize_timer_cb(lv_timer_t *timer);

/**
 * Set the UI theme.
 * 
 * @param is_dark true if the dark theme should be applied, false if the light theme should be applied
 */
static void set_theme(bool is_dark);

/**
 * Handle LV_EVENT_DRAW_TASK_ADDED events from the keyboard widget.
 *
 * @param event the event object
 */
static void keyboard_draw_task_added_cb(lv_event_t *event);

/**
 * Handle LV_EVENT_VALUE_CHANGED events from the keyboard widget.
 * 
 * @param event the event object
 */
static void keyboard_value_changed_cb(lv_event_t *event);

/**
 * Emit key down and up events for a key.
 *
 * @param btn_id button index corresponding to the key
 * @param key_down true if a key down event should be emitted
 * @param key_up true if a key up event should be emitted
 */
static void emit_key_events(uint16_t btn_id, bool key_down, bool key_up);

/**
 * Release any previously pressed modifier keys.
 */
static void pop_checked_modifier_keys(void);


/**
 * Static functions
 */

static int keyboard_height_denominator(lv_coord_t width, lv_coord_t height) {
    return (height > width) ? 3 : 2;
}

static void sigaction_handler(int signum) {
    if (resize_terminals) {
        bb_terminal_reset_all();
    }
    exit(0);
}

static void terminal_resize_timer_cb(lv_timer_t *timer) {
    if (resize_terminals) {
        bb_terminal_shrink_current();
    }
}

static void set_theme(bool is_dark) {
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN), is_dark, &font_32);
}

static void keyboard_draw_task_added_cb(lv_event_t *event) {
    lv_obj_t *obj = lv_event_get_target(event);
    lv_buttonmatrix_t *btnm = (lv_buttonmatrix_t *)obj;
    lv_draw_task_t *draw_task = lv_event_get_draw_task(event);
    lv_draw_dsc_base_t *dsc = draw_task->draw_dsc;

    if (dsc->part != LV_PART_ITEMS) {
        return;
    }

    lv_draw_fill_dsc_t *fill_dsc = lv_draw_task_get_fill_dsc(draw_task);
    if (!fill_dsc) {
        return;
    }

    if (lv_btnmatrix_get_selected_btn(obj) == dsc->id1 && lv_obj_has_state(obj, LV_STATE_PRESSED)) {
        if ((btnm->ctrl_bits[dsc->id1] & SQ2LV_CTRL_MOD_INACTIVE) == SQ2LV_CTRL_MOD_INACTIVE) {
            fill_dsc->color = lv_palette_lighten(LV_PALETTE_TEAL, 1);
        } else if ((btnm->ctrl_bits[dsc->id1] & SQ2LV_CTRL_MOD_ACTIVE) == SQ2LV_CTRL_MOD_ACTIVE) {
            fill_dsc->color = lv_palette_lighten(LV_PALETTE_TEAL, 1);
        } else if ((btnm->ctrl_bits[dsc->id1] & SQ2LV_CTRL_NON_CHAR) == SQ2LV_CTRL_NON_CHAR) {
            fill_dsc->color = lv_palette_darken(LV_PALETTE_BLUE_GREY, 3);
        } else {
            fill_dsc->color = lv_palette_lighten(LV_PALETTE_BLUE_GREY, 1);
        }
    } else {
        if ((btnm->ctrl_bits[dsc->id1] & SQ2LV_CTRL_MOD_INACTIVE) == SQ2LV_CTRL_MOD_INACTIVE) {
            fill_dsc->color = lv_palette_darken(LV_PALETTE_BLUE_GREY, 4);
        } else if ((btnm->ctrl_bits[dsc->id1] & SQ2LV_CTRL_MOD_ACTIVE) == SQ2LV_CTRL_MOD_ACTIVE) {
            fill_dsc->color = lv_palette_main(LV_PALETTE_TEAL);
        } else if ((btnm->ctrl_bits[dsc->id1] & SQ2LV_CTRL_NON_CHAR) == SQ2LV_CTRL_NON_CHAR) {
            fill_dsc->color = lv_palette_darken(LV_PALETTE_BLUE_GREY, 4);
        } else {
            fill_dsc->color = lv_palette_main(LV_PALETTE_BLUE_GREY);
        }
    }
}

static void keyboard_value_changed_cb(lv_event_t *event) {
    lv_obj_t *kb = lv_event_get_target(event);

    uint16_t btn_id = lv_buttonmatrix_get_selected_button(kb);
    if (btn_id == LV_BUTTONMATRIX_BUTTON_NONE) {
        return;
    }

    if (sq2lv_is_layer_switcher(kb, btn_id)) {
        pop_checked_modifier_keys();
        sq2lv_switch_layer(kb, btn_id);
        return;
    }

    /* Note that the LV_BUTTONMATRIX_CTRL_CHECKED logic is inverted because LV_KEYBOARD_CTRL_BTN_FLAGS already
     * contains LV_BUTTONMATRIX_CTRL_CHECKED. As a result, pressing e.g. CTRL will _un_check the key. To account
     * for this, we invert the meaning of "checked" here and elsewhere in the code. */

    bool is_modifier = sq2lv_is_modifier(keyboard, btn_id);
    bool is_checked = !lv_buttonmatrix_has_button_ctrl(keyboard, btn_id, LV_BUTTONMATRIX_CTRL_CHECKED);

    /* Emit key events. Suppress key up events for modifiers unless they were unchecked. For checked modifiers
     * the key up events are sent with the next non-modifier key press. */
    emit_key_events(btn_id, true, !is_modifier || !is_checked);

    /* Pop any previously checked modifiers when a non-modifier key was pressed */
    if (!is_modifier) {
        pop_checked_modifier_keys();
    }
}

static void emit_key_events(uint16_t btn_id, bool key_down, bool key_up) {
    int num_scancodes = 0;
    const int *scancodes = sq2lv_get_scancodes(keyboard, btn_id, &num_scancodes);

    if (key_down) {
        /* Emit key down events in forward order */
        for (int i = 0; i < num_scancodes; ++i) {
            bb_uinput_device_emit_key_down(scancodes[i]);
        }
    }

    if (key_up) {
        /* Emit key up events in backward order */
        for (int i = num_scancodes - 1; i >= 0; --i) {
            bb_uinput_device_emit_key_up(scancodes[i]);
        }
    }
}

static void pop_checked_modifier_keys(void) {
    int num_modifiers = 0;
    const int *modifier_idxs = sq2lv_get_modifier_indexes(keyboard, &num_modifiers);

    for (int i = 0; i < num_modifiers; ++i) {
        if (!lv_buttonmatrix_has_button_ctrl(keyboard, modifier_idxs[i], LV_BUTTONMATRIX_CTRL_CHECKED)) {
            emit_key_events(modifier_idxs[i], false, true);
            lv_buttonmatrix_set_button_ctrl(keyboard, modifier_idxs[i], LV_BUTTONMATRIX_CTRL_CHECKED);
        }
    }
}


/**
 * Main
 */

int main(int argc, char *argv[]) {
    /* Parse command line options */
    bb_cli_parse_opts(argc, argv, &cli_opts);

    /* Prepare for terminal resizing and reset */
    resize_terminals = bb_terminal_init(2.0f / 3.0f);
    if (resize_terminals) {
        /* Clean up on termination */
        struct sigaction action;
        memset(&action, 0, sizeof(action));
        action.sa_handler = sigaction_handler;
        sigaction(SIGINT, &action, NULL);
        sigaction(SIGTERM, &action, NULL);

        /* Resize current terminal */
        bb_terminal_shrink_current();
    }

    /* Set up uinput device */
    if (!bb_uinput_device_init(sq2lv_unique_scancodes, sq2lv_num_unique_scancodes)) {
        return 1;
    }

    /* Initialise lvgl */
    lv_init();

    /* Initialise display */
    lv_display_t *disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, "/dev/fb0");
    int32_t hor_res_phys = lv_display_get_horizontal_resolution(disp);
    int32_t ver_res_phys = lv_display_get_vertical_resolution(disp);
    lv_display_set_physical_resolution(disp, hor_res_phys, ver_res_phys);
    // TODO: Sofware-rotation is currently broken in the framebuffer driver (https://gitlab.com/postmarketOS/buffybox/-/issues/26)
    lv_coord_t denom = keyboard_height_denominator(hor_res_phys, ver_res_phys);
    lv_display_set_resolution(disp, hor_res_phys, ver_res_phys / denom);
    lv_display_set_offset(disp, 0, (denom - 1) * ver_res_phys / denom);
    // lv_display_set_rotation(disp, cli_opts.rotation);
    // switch (cli_opts.rotation) {
    //     case LV_DISPLAY_ROTATION_0:
    //     case LV_DISPLAY_ROTATION_180: {
    //         lv_coord_t denom = keyboard_height_denominator(hor_res_phys, ver_res_phys);
    //         lv_display_set_resolution(disp, hor_res_phys, ver_res_phys / denom);
    //         lv_display_set_offset(disp, 0, (cli_opts.rotation == LV_DISPLAY_ROTATION_0) ? (denom - 1) * ver_res_phys / denom : 0);
    //         break;
    //     }
    //     case LV_DISPLAY_ROTATION_90:
    //     case LV_DISPLAY_ROTATION_270: {
    //         lv_coord_t denom = keyboard_height_denominator(ver_res_phys, hor_res_phys);
    //         lv_display_set_resolution(disp, hor_res_phys / denom, ver_res_phys);
    //         lv_display_set_offset(disp, (cli_opts.rotation == LV_DISPLAY_ROTATION_90) ? (denom - 1) * hor_res_phys / denom : 0, 0);
    //         break;
    //     }
    // }

    /* Connect input devices */
    bb_indev_auto_connect();
    bb_indev_set_up_mouse_cursor();

    /* Initialise theme and styles */
    set_theme(true);
    lv_style_init(&style_text_normal);
    lv_style_set_text_font(&style_text_normal, &font_32);

    /* Add keyboard */
    keyboard = lv_keyboard_create(lv_scr_act());
    // lv_buttonmatrix_set_popovers(keyboard, true);
    lv_obj_set_pos(keyboard, 0, 0);
    lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_style(keyboard, &style_text_normal, 0);

    /* Set up keyboard event handlers */
    lv_obj_remove_event_cb(keyboard, lv_keyboard_def_event_cb);
    lv_obj_add_event_cb(keyboard, keyboard_value_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(keyboard, keyboard_draw_task_added_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    /* Apply default keyboard layout */
    sq2lv_switch_layout(keyboard, SQ2LV_LAYOUT_TERMINAL_US);

    /* Start timer for periodically resizing terminals */
    lv_timer_create(terminal_resize_timer_cb, 1000,  NULL);

    /* Run lvgl in "tickless" mode */
    while(1) {
        lv_task_handler();
        usleep(5000);
    }

    return 0;
}


/**
 * Tick generation
 */

/**
 * Generate tick for LVGL.
 * 
 * @return tick in ms
 */
uint32_t bb_get_tick(void) {
    static uint64_t start_ms = 0;
    if (start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
