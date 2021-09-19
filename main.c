/**
 * Copyright 2021 Johannes Marbach
 *
 * This file is part of buffyboard, hereafter referred to as the program.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "cursor.h"
#include "libinput_multi.h"
#include "sq2lv_layouts.h"
#include "terminal.h"
#include "uinput_device.h"

#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/libinput_drv.h"

#include "lvgl/lvgl.h"

#include "squeek2lvgl/sq2lv.h"

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>


/**
 * Custom fonts
 */

LV_FONT_DECLARE(montserrat_extended_32);


/**
 * Static variables
 */

static bool resize_terminals = false;
static lv_obj_t *keyboard = NULL;
static lv_style_t style_text_normal;


/**
 * Static prototypes
 */

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
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN), is_dark, &montserrat_extended_32);
}

static void keyboard_value_changed_cb(lv_event_t *event) {
    lv_obj_t *kb = lv_event_get_target(event);

    uint16_t btn_id = lv_btnmatrix_get_selected_btn(kb);
    if (btn_id == LV_BTNMATRIX_BTN_NONE) {
        return;
    }

    if (sq2lv_is_layer_switcher(kb, btn_id)) {
        pop_checked_modifier_keys();
        sq2lv_switch_layer(kb, btn_id);
        return;
    }

    /* Note that the LV_BTNMATRIX_CTRL_CHECKED logic is inverted because LV_KEYBOARD_CTRL_BTN_FLAGS already
     * contains LV_BTNMATRIX_CTRL_CHECKED. As a result, pressing e.g. CTRL will _un_check the key. To account
     * for this, we invert the meaning of "checked" here and elsewhere in the code. */

    bool is_modifier = sq2lv_is_modifier(keyboard, btn_id);
    bool is_checked = !lv_btnmatrix_has_btn_ctrl(keyboard, btn_id, LV_BTNMATRIX_CTRL_CHECKED);

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
    int *scancodes = sq2lv_get_scancodes(keyboard, btn_id, &num_scancodes);

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
    int *modifier_idxs = sq2lv_get_modifier_indexes(keyboard, &num_modifiers);

    for (int i = 0; i < num_modifiers; ++i) {
        if (!lv_btnmatrix_has_btn_ctrl(keyboard, modifier_idxs[i], LV_BTNMATRIX_CTRL_CHECKED)) {
            emit_key_events(modifier_idxs[i], false, true);
            lv_btnmatrix_set_btn_ctrl(keyboard, modifier_idxs[i], LV_BTNMATRIX_CTRL_CHECKED);
        }
    }
}

static void keyboard_draw_part_begin_cb(lv_event_t *event) {
    lv_obj_t *obj = lv_event_get_target(event);
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(event);

    if (dsc->part != LV_PART_ITEMS) {
        return;
    }

    if (lv_btnmatrix_get_selected_btn(obj) == dsc->id) { /* key is held down */
        if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKED)) {
            if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKABLE)) { /* inactive modifiers */
                dsc->rect_dsc->bg_color = lv_palette_lighten(LV_PALETTE_TEAL, 1);
            } else { /* non-letters */
                dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_BLUE_GREY, 3);
            }
        } else {
            if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKABLE)) { /* active modifiers */
                dsc->rect_dsc->bg_color = lv_palette_lighten(LV_PALETTE_TEAL, 1);
            } else { /* letters */
                dsc->rect_dsc->bg_color = lv_palette_lighten(LV_PALETTE_BLUE_GREY, 1);
            }
        }
    } else { /* key is not held down */
        if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKED)) { /* inactive modifiers & non-letters */
            dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_BLUE_GREY, 4);
        } else if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKABLE)) { /* active modifiers */
            dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_TEAL);
        } else { /* letters */
            dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_BLUE_GREY);
        }
    }
}


/**
 * Main
 */

int main(void) {
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

    /* Initialise lvgl and framebuffer driver */
    lv_init(); 
    fbdev_init();

    /* Query display size */
    uint32_t hor_res;
    uint32_t ver_res;
	fbdev_get_sizes(&hor_res, &ver_res);

    /* Make sure to not cover the whole screen */
    ver_res /= 3;
    // fbdev_set_offset(0, 2 * ver_res);

    /* Prepare display buffer */
    const size_t buf_size = hor_res * ver_res / 10; /* At least 1/10 of the display size is recommended */
    lv_disp_draw_buf_t disp_buf;
    lv_color_t *buf = (lv_color_t *)malloc(buf_size * sizeof(lv_color_t));
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, buf_size);    

    /* Initialise display driver */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = hor_res;
    disp_drv.ver_res = ver_res;
    lv_disp_drv_register(&disp_drv);

    /* Connect mice and trackpads */
    #define MAX_POINTER_DEVICES 4
    char *pointer_devices[MAX_POINTER_DEVICES] = { NULL, NULL, NULL, NULL };
    lv_indev_drv_t pointer_indev_drvs[MAX_POINTER_DEVICES];
    lv_indev_t *pointer_indevs[MAX_POINTER_DEVICES] = { NULL, NULL, NULL, NULL };
    size_t num_pointer_devices = libinput_find_devs(LIBINPUT_CAPABILITY_POINTER, pointer_devices, MAX_POINTER_DEVICES, false);
    for (int i = 0; i < num_pointer_devices; ++i) {
        printf("Connecting pointer device %s\n", pointer_devices[i]);
        lv_indev_drv_init(&pointer_indev_drvs[i]);
        pointer_indev_drvs[i].type = LV_INDEV_TYPE_POINTER;
        pointer_indev_drvs[i].read_cb = libinput_multi_read;
        pointer_indev_drvs[i].long_press_repeat_time = USHRT_MAX;
        libinput_multi_init_driver(&pointer_indev_drvs[i]);
        libinput_multi_set_file(&pointer_indev_drvs[i], pointer_devices[i]);
        pointer_indevs[i] = lv_indev_drv_register(&pointer_indev_drvs[i]);
    }

    /* Set mouse cursor */
    if (num_pointer_devices > 0) {
        lv_obj_t *cursor_obj = lv_img_create(lv_scr_act());
        lv_img_set_src(cursor_obj, &bb_cursor_img_dsc);
        for (int i = 0; i < num_pointer_devices; ++i) {
            lv_indev_set_cursor(pointer_indevs[i], cursor_obj);
        }
    }

    /* Connect touchscreens */
    #define MAX_TOUCHSCREENS 1
    char *touchscreens[MAX_TOUCHSCREENS] = { NULL };
    lv_indev_drv_t touchscreen_indev_drvs[MAX_TOUCHSCREENS];
    lv_indev_t *touchscreen_indevs[MAX_TOUCHSCREENS] = { NULL };
    size_t num_touchscreens = libinput_find_devs(LIBINPUT_CAPABILITY_TOUCH, touchscreens, MAX_TOUCHSCREENS, false);
    for (int i = 0; i < num_touchscreens; ++i) {
        printf("Connecting touchscreen device %s\n", touchscreens[i]);
        lv_indev_drv_init(&touchscreen_indev_drvs[i]);
        touchscreen_indev_drvs[i].type = LV_INDEV_TYPE_POINTER;
        touchscreen_indev_drvs[i].read_cb = libinput_multi_read;
        touchscreen_indev_drvs[i].long_press_repeat_time = USHRT_MAX;
        libinput_multi_init_driver(&touchscreen_indev_drvs[i]);
        libinput_multi_set_file(&touchscreen_indev_drvs[i], touchscreens[i]);
        touchscreen_indevs[i] = lv_indev_drv_register(&touchscreen_indev_drvs[i]);
    }

    /* Initialise theme and styles */
    set_theme(true);
    lv_style_init(&style_text_normal);
    lv_style_set_text_font(&style_text_normal, &montserrat_extended_32);

    /* Add keyboard */
    keyboard = lv_keyboard_create(lv_scr_act());
    // lv_btnmatrix_set_popovers(keyboard, true);
    lv_obj_set_pos(keyboard, 0, 0);
    lv_obj_set_size(keyboard, hor_res, ver_res);
    lv_obj_add_style(keyboard, &style_text_normal, 0);

    /* Set up keyboard event handlers */
    lv_obj_remove_event_cb(keyboard, lv_keyboard_def_event_cb);
    lv_obj_add_event_cb(keyboard, keyboard_value_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(keyboard, keyboard_draw_part_begin_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

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
