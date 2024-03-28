/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "indev.h"

#include "../shared/cursor/cursor.h"

#include "lvgl/lvgl.h"

#include <limits.h>


/**
 * Defines
 */

#define MAX_POINTER_DEVS 4
#define MAX_TOUCHSCREEN_DEVS 1


/**
 * Static variables
 */

static int num_pointer_devs = 0;
static char *pointer_devs[MAX_POINTER_DEVS];
static lv_indev_t *pointer_indevs[MAX_POINTER_DEVS];

static int num_touchscreen_devs = 0;
static char *touchscreen_devs[MAX_TOUCHSCREEN_DEVS];
static lv_indev_t *touchscreen_indevs[MAX_TOUCHSCREEN_DEVS];


/**
 * Static prototypes
 */

/**
 * Auto-connect available input devices having a specific capability.
 *
 * @param capability capability to filter devices by
 * @param max_num_devs maximum number of devices to connect
 * @param num_devs pointer for writing the actual number of connected devices into
 * @param devs array for storing device paths
 * @param indevs array for storing LVGL indevs
 */
static void auto_connect(lv_libinput_capability capability, int max_num_devs, int *num_devs, char *devs[], lv_indev_t *indevs[]);


/**
 * Static functions
 */

static void auto_connect(lv_libinput_capability capability, int max_num_devs, int *num_devs, char *devs[], lv_indev_t *indevs[]) {
    lv_memset(devs, 0, max_num_devs * sizeof(char *));
    lv_memset(indevs, 0, max_num_devs * sizeof(lv_indev_t *));

    *num_devs = lv_libinput_find_devs(capability, devs, max_num_devs, false);

    for (int i = 0; i < *num_devs; ++i) {
        indevs[i] = lv_libinput_create(capability & LV_LIBINPUT_CAPABILITY_KEYBOARD ? LV_INDEV_TYPE_KEYPAD : LV_INDEV_TYPE_POINTER, devs[i]);
    }
}


/**
 * Public functions
 */

void bb_indev_auto_connect() {
    auto_connect(LV_LIBINPUT_CAPABILITY_POINTER, MAX_POINTER_DEVS, &num_pointer_devs, pointer_devs, pointer_indevs);
    auto_connect(LV_LIBINPUT_CAPABILITY_TOUCH, MAX_TOUCHSCREEN_DEVS, &num_touchscreen_devs, touchscreen_devs, touchscreen_indevs);
}

void bb_indev_set_up_mouse_cursor() {
    lv_obj_t *cursor_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor_obj, &bb_cursor_img_dsc);
    for (int i = 0; i < num_pointer_devs; ++i) {
        lv_indev_set_cursor(pointer_indevs[i], cursor_obj);
    }
}
