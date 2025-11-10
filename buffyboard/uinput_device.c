/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "uinput_device.h"

#include "../shared/log.h"

#include <linux/uinput.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if UINPUT_VERSION < 5
#error Buffyboard does not have support for uinput < 5
#endif

/**
 * Static variables
 */

static int fd = -1;
struct input_event event;


/**
 * Static prototypes
 */

/**
 * Emit an event on the device.
 * @param type event type
 * @param code event code
 * @param value event value
 * @return true if emitting the event was succesful, false otherwise
 */
static bool uinput_device_emit(int type, int code, int value);

/**
 * Emit a synchronisation event on the device
 * @return true if emitting the event was succesful, false otherwise
 */
static bool uinput_device_synchronise();


/**
 * Static functions
 */

static bool uinput_device_emit(int type, int code, int value) {
    event.type = type;
    event.code = code;
    event.value = value;
    event.input_event_sec = 0;
    event.input_event_usec = 0;

    if (write(fd, &event, sizeof(event)) != sizeof(event)) {
        perror("Could not emit event");
        return false;
    }

    return true;
}

static bool uinput_device_synchronise() {
    return uinput_device_emit(EV_SYN, SYN_REPORT, 0);
}


/**
 * Public functions
 */

bool bb_uinput_device_init(const int * const keycodes, int num_keycodes) {
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not open /dev/uinput");
        return false;
    }

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not set EVBIT for EV_KEY");
        return false;
    }

    for (int i = 0; i < num_keycodes; ++i) {
        if (ioctl(fd, UI_SET_KEYBIT, keycodes[i]) < 0) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Could not set KEYBIT");
            return false;
        }
    }

    static_assert(sizeof("buffyboard") <= UINPUT_MAX_NAME_SIZE);

    struct uinput_setup usetup = { 0 };
    usetup.id.bustype = BUS_VIRTUAL;
    strcpy(usetup.name, "buffyboard");

    if (ioctl(fd, UI_DEV_SETUP, &usetup) < 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not set up uinput device");
        return false;
    }

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not create uinput device");
        return false;
    }

    memset(&event, 0, sizeof(event));

    return true;
}

bool bb_uinput_device_emit_key_down(int keycode) {
    return uinput_device_emit(EV_KEY, keycode, 1) && uinput_device_synchronise();
}

bool bb_uinput_device_emit_key_up(int keycode) {
    return uinput_device_emit(EV_KEY, keycode, 0) && uinput_device_synchronise();
}
