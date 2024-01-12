/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "indev.h"

#include "cursor.h"

#include "lv_drivers/indev/libinput_drv.h"

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
static lv_indev_drv_t pointer_indev_drvs[MAX_POINTER_DEVS];
static libinput_drv_state_t pointer_drv_states[MAX_POINTER_DEVS];

static int num_touchscreen_devs = 0;
static char *touchscreen_devs[MAX_TOUCHSCREEN_DEVS];
static lv_indev_t *touchscreen_indevs[MAX_TOUCHSCREEN_DEVS];
static lv_indev_drv_t touchscreen_indev_drvs[MAX_TOUCHSCREEN_DEVS];
static libinput_drv_state_t touchscreen_drv_states[MAX_TOUCHSCREEN_DEVS];


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
 * @param indev_drvs array for storing LVGL indev drivers
 * @param drv_states array for storing LVGL libinput driver states
 */
static void auto_connect(libinput_capability capability, int max_num_devs, int *num_devs, char *devs[], lv_indev_t *indevs[],
                  lv_indev_drv_t indev_drvs[], libinput_drv_state_t drv_states[]);

/**
 * Perform an input read on a device using the libinput driver.
 *
 * @param indev_drv input device driver
 * @param data input device data to write into
 */
static void libinput_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);


/**
 * Static functions
 */

static void auto_connect(libinput_capability capability, int max_num_devs, int *num_devs, char *devs[], lv_indev_t *indevs[],
    lv_indev_drv_t indev_drvs[], libinput_drv_state_t drv_states[])
{
    memset(devs, 0, max_num_devs * sizeof(char *));
    memset(indevs, 0, max_num_devs * sizeof(lv_indev_t *));
    memset(indev_drvs, 0, max_num_devs * sizeof(lv_indev_drv_t));
    memset(drv_states, 0, max_num_devs * sizeof(libinput_drv_state_t));

    *num_devs = libinput_find_devs(capability, devs, max_num_devs, false);

    for (int i = 0; i < *num_devs; ++i) {
        libinput_init_state(&(drv_states[i]), devs[i]);
        lv_indev_drv_init(&(indev_drvs[i]));

        indev_drvs[i].read_cb = libinput_read_cb;
        indev_drvs[i].user_data = &(drv_states[i]);

        if (capability == LIBINPUT_CAPABILITY_KEYBOARD) {
            indev_drvs[i].type = LV_INDEV_TYPE_KEYPAD;
        } else {
            indev_drvs[i].type = LV_INDEV_TYPE_POINTER;
        }

        indevs[i] = lv_indev_drv_register(&(indev_drvs[i]));
    }
}

static void libinput_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    libinput_read_state(indev_drv->user_data, indev_drv, data);
}


/**
 * Public functions
 */

void bb_indev_auto_connect() {
    auto_connect(LIBINPUT_CAPABILITY_POINTER, MAX_POINTER_DEVS, &num_pointer_devs, pointer_devs, pointer_indevs,
                 pointer_indev_drvs, pointer_drv_states);
    auto_connect(LIBINPUT_CAPABILITY_TOUCH, MAX_TOUCHSCREEN_DEVS, &num_touchscreen_devs, touchscreen_devs, touchscreen_indevs,
                 touchscreen_indev_drvs, touchscreen_drv_states);
}

void bb_indev_set_up_mouse_cursor() {
    lv_obj_t *cursor_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor_obj, &bb_cursor_img_dsc);
    for (int i = 0; i < num_pointer_devs; ++i) {
        lv_indev_set_cursor(pointer_indevs[i], cursor_obj);
    }
}
