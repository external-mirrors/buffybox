/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "indev.h"

#include "cursor/cursor.h"
#include "log.h"

#include "lvgl/lvgl.h"

#ifndef BBX_APP_BUFFYBOARD
#include <linux/input-event-codes.h>
#endif
#include <sys/epoll.h>
#include <sys/stat.h>
#ifndef BBX_APP_BUFFYBOARD
#include <xkbcommon/xkbcommon.h>
#endif
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef BBX_APP_BUFFYBOARD
/* All keyboard devices are connected to this input group */
lv_group_t *keyboard_input_group;
#endif

enum input_device_type {
    BBX_INDEV_NONE,
    BBX_INDEV_KEY,
    BBX_INDEV_KEYBOARD,
    BBX_INDEV_MOUSE,
    BBX_INDEV_TOUCHPAD,
    BBX_INDEV_TOUCHSCREEN,
    BBX_INDEV_OTHER
};

struct input_device_ext {
    /* The second pointer is used to represent the second finger on touchscreens.
       This makes typing with thumbs convenient */
    lv_indev_t* pointer1;
    lv_indev_t* pointer2;
#ifndef BBX_APP_BUFFYBOARD
    lv_indev_t* keypad;
    struct xkb_state* xkb_state;
#endif
    enum input_device_type type;
};

/* The analog of lv_indev_data_t, but without unused fields */
struct indev_ext {
    union {
        lv_point_t point;
        uint32_t key;
    };
    lv_indev_state_t state;
};

/**
 * Static variables
 */

static struct libinput *context_libinput;
static struct udev *context_udev;
static struct udev_monitor *monitor;
#ifndef BBX_APP_BUFFYBOARD
static struct xkb_keymap *keymap;
#endif
static lv_obj_t *mouse_cursor;
static bool libinput_is_suspended = false;

#ifndef BBX_APP_BUFFYBOARD
static uint8_t num_keyboards = 0;
#endif
static uint8_t num_mouses = 0;
static uint8_t num_touchpads = 0;

#ifndef BBX_APP_BUFFYBOARD
static void (*on_key_power_cb)() = NULL;
#endif

static struct {
#ifndef BBX_APP_BUFFYBOARD
    uint8_t keyboard : 1;
#endif
    uint8_t pointer : 1;
    uint8_t touchscreen : 1;
} options;

/**
 * Static functions
 */

static int open_restricted(const char *path, int flags, void *user_data) {
    LV_UNUSED(user_data);
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) {
    LV_UNUSED(user_data);
    close(fd);
}

#ifndef BBX_APP_BUFFYBOARD
static void read_keypad(lv_indev_t *indev, lv_indev_data_t *data) {
    struct indev_ext* ext = lv_indev_get_user_data(indev);

    data->key = ext->key;
    data->state = ext->state;
}
#endif

static void read_pointer(lv_indev_t * indev, lv_indev_data_t * data) {
    struct indev_ext* ext = lv_indev_get_user_data(indev);

    data->point = ext->point;
    data->state = ext->state;
}

static enum input_device_type identify_input_device(struct udev_device* device) {
    bool is_input = false;
    enum input_device_type ret = BBX_INDEV_OTHER;

    struct udev_list_entry *entry;
    struct udev_list_entry *properties = udev_device_get_properties_list_entry(device);
    udev_list_entry_foreach(entry, properties) {
        const char* name = udev_list_entry_get_name(entry);
        const char* value = udev_list_entry_get_value(entry);

        if (strcmp(name, "ID_INPUT") == 0 && strcmp(value, "1") == 0)
            is_input = true;
        else if (strcmp(name, "ID_INPUT_KEY") == 0 && strcmp(value, "1") == 0)
            ret = BBX_INDEV_KEY;
        else if (strcmp(name, "ID_INPUT_KEYBOARD") == 0 && strcmp(value, "1") == 0)
            ret = BBX_INDEV_KEYBOARD;
        else if (strcmp(name, "ID_INPUT_MOUSE") == 0 && strcmp(value, "1") == 0)
            ret = BBX_INDEV_MOUSE;
        else if (strcmp(name, "ID_INPUT_TOUCHPAD") == 0 && strcmp(value, "1") == 0)
            ret = BBX_INDEV_TOUCHPAD;
        else if (strcmp(name, "ID_INPUT_TOUCHSCREEN") == 0 && strcmp(value, "1") == 0)
            ret = BBX_INDEV_TOUCHSCREEN;
    }

    if (!is_input) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "%s is not an input device", udev_device_get_devnode(device));
        ret = BBX_INDEV_NONE;
    }

    return ret;
}

static void add_device_extension(struct libinput_device* device) {
    enum input_device_type type = (enum input_device_type) libinput_device_get_user_data(device);
    libinput_device_set_user_data(device, NULL);
    if (type == BBX_INDEV_NONE) {
        struct udev_device *dev = libinput_device_get_udev_device(device);
        if (!dev) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "libinput_device_get_udev_device(%s) is failed", libinput_device_get_sysname(device));
            goto failure1;
        }
        type = identify_input_device(dev);
        udev_device_unref(dev);
        if (type == BBX_INDEV_NONE)
            goto failure1;
    }

    struct input_device_ext* ext = malloc(sizeof(struct input_device_ext));
    if (!ext) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
        goto failure1;
    }

    ext->pointer1 = NULL;
    ext->pointer2 = NULL;
#ifndef BBX_APP_BUFFYBOARD
    ext->keypad = NULL;
    ext->xkb_state = NULL;
#endif
    ext->type = type;

#ifndef BBX_APP_BUFFYBOARD
    if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_KEYBOARD)) {
        ext->keypad = lv_indev_create();
        if (!ext->keypad) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
            goto failure2;
        }

        struct indev_ext* data = malloc(sizeof(struct indev_ext));
        if (!data) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
            lv_indev_delete(ext->keypad);
            goto failure2;
        }

        data->key = 0;
        data->state = LV_INDEV_STATE_RELEASED;

        lv_indev_set_type(ext->keypad, LV_INDEV_TYPE_KEYPAD);
        lv_indev_set_mode(ext->keypad, LV_INDEV_MODE_EVENT);
        lv_indev_set_user_data(ext->keypad, data);
        lv_indev_set_read_cb(ext->keypad, read_keypad);
        lv_indev_set_group(ext->keypad, keyboard_input_group);

        if (ext->type == BBX_INDEV_KEYBOARD) {
            ext->xkb_state = xkb_state_new(keymap);
            if (!ext->xkb_state)
                bbx_log(BBX_LOG_LEVEL_WARNING, "Can't create xkb_state for %s", libinput_device_get_sysname(device));
        }
    }
#endif

    if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_POINTER) ||
        libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_TOUCH)) {
        ext->pointer1 = lv_indev_create();
        if (!ext->pointer1) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
            goto failure3;
        }

        struct indev_ext* data1 = malloc(sizeof(struct indev_ext));
        if (!data1) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
            lv_indev_delete(ext->pointer1);
            goto failure3;
        }

        lv_display_t* display = lv_indev_get_display(ext->pointer1);
        data1->point.x = display->hor_res / 2;
        data1->point.y = display->ver_res / 2;
        data1->state = LV_INDEV_STATE_RELEASED;

        lv_indev_set_type(ext->pointer1, LV_INDEV_TYPE_POINTER);
        lv_indev_set_mode(ext->pointer1, LV_INDEV_MODE_EVENT);
        lv_indev_set_user_data(ext->pointer1, data1);
        lv_indev_set_read_cb(ext->pointer1, read_pointer);

        if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_TOUCH) &&
            libinput_device_touch_get_touch_count(device) != 1) {
            ext->pointer2 = lv_indev_create();
            if (!ext->pointer2) {
                bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
                goto failure3;
            }

            struct indev_ext* data2 = malloc(sizeof(struct indev_ext));
            if (!data2) {
                bbx_log(BBX_LOG_LEVEL_ERROR, "Out of memory");
                lv_indev_delete(ext->pointer2);
                goto failure3;
            }

            data2->point.x = display->hor_res / 2;
            data2->point.y = display->ver_res / 2;
            data2->state = LV_INDEV_STATE_RELEASED;

            lv_indev_set_type(ext->pointer2, LV_INDEV_TYPE_POINTER);
            lv_indev_set_mode(ext->pointer2, LV_INDEV_MODE_EVENT);
            lv_indev_set_user_data(ext->pointer2, data2);
            lv_indev_set_read_cb(ext->pointer2, read_pointer);
        }

        if (ext->type == BBX_INDEV_MOUSE || ext->type == BBX_INDEV_TOUCHPAD) {
            lv_indev_read(ext->pointer1);
            lv_indev_set_cursor(ext->pointer1, mouse_cursor);
        }
    }

#ifndef BBX_APP_BUFFYBOARD
    if (!ext->pointer1 && !ext->keypad) {
#else
    if (!ext->pointer1) {
#endif
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "%s does not have required capabilities, ignoring.",
            libinput_device_get_sysname(device));
        goto failure2;
    }

    libinput_device_set_user_data(device, ext);

#ifndef BBX_APP_BUFFYBOARD
    if (ext->type == BBX_INDEV_KEYBOARD)
        num_keyboards++;
    else
#endif
    if (ext->type == BBX_INDEV_MOUSE)
        num_mouses++;
    else if (ext->type == BBX_INDEV_TOUCHPAD)
        num_touchpads++;
    if (num_mouses != 0 || num_touchpads != 0)
        lv_obj_remove_flag(mouse_cursor, LV_OBJ_FLAG_HIDDEN);

    bbx_log(BBX_LOG_LEVEL_VERBOSE, "New input device: %s", libinput_device_get_name(device));
    return;


failure3:
    if (ext->pointer1) {
        free(lv_indev_get_user_data(ext->pointer1));
        lv_indev_delete(ext->pointer1);
    }
    if (ext->pointer2) {
        free(lv_indev_get_user_data(ext->pointer2));
        lv_indev_delete(ext->pointer2);
    }
#ifndef BBX_APP_BUFFYBOARD
    if (ext->keypad) {
        free(lv_indev_get_user_data(ext->keypad));
        lv_indev_delete(ext->keypad);
    }
    if (ext->xkb_state) {
        xkb_state_unref(ext->xkb_state);
    }
#endif
failure2:
    free(ext);
failure1:
    libinput_path_remove_device(device);
    return;
}

static void on_input_event() {
    libinput_dispatch(context_libinput);

    struct libinput_event * event;
    while ((event = libinput_get_event(context_libinput))) {
        struct libinput_device* device = libinput_event_get_device(event);
        struct input_device_ext* ext = libinput_device_get_user_data(device);

        switch (libinput_event_get_type(event)) {
        case LIBINPUT_EVENT_DEVICE_ADDED: {
            add_device_extension(device);
            break;
        }

        case LIBINPUT_EVENT_DEVICE_REMOVED: {
            if (!ext)
                break;

            if (ext->pointer1) {
                free(lv_indev_get_user_data(ext->pointer1));
                lv_indev_delete(ext->pointer1);
            }
            if (ext->pointer2) {
                free(lv_indev_get_user_data(ext->pointer2));
                lv_indev_delete(ext->pointer2);
            }
#ifndef BBX_APP_BUFFYBOARD
            if (ext->keypad) {
                free(lv_indev_get_user_data(ext->keypad));
                lv_indev_delete(ext->keypad);
            }
            if (ext->xkb_state) {
                xkb_state_unref(ext->xkb_state);
            }

            if (ext->type == BBX_INDEV_KEYBOARD) {
                assert(num_keyboards > 0);
                num_keyboards--;
            } else
#endif
            if (ext->type == BBX_INDEV_MOUSE) {
                assert(num_mouses > 0);
                num_mouses--;
            } else if (ext->type == BBX_INDEV_TOUCHPAD) {
                assert(num_touchpads > 0);
                num_touchpads--;
            }
            if (num_mouses == 0 && num_touchpads == 0)
                lv_obj_add_flag(mouse_cursor, LV_OBJ_FLAG_HIDDEN);

            free(ext);
            break;
        }

#ifndef BBX_APP_BUFFYBOARD
        case LIBINPUT_EVENT_KEYBOARD_KEY: {
            struct libinput_event_keyboard *kb_event = libinput_event_get_keyboard_event(event);

            struct indev_ext *data = lv_indev_get_user_data(ext->keypad);
            uint32_t key = libinput_event_keyboard_get_key(kb_event);
            bool pressed = libinput_event_keyboard_get_key_state(kb_event) == LIBINPUT_KEY_STATE_PRESSED;
            xkb_keycode_t keycode = key + 8;
            bool ignore = false;

            switch (key) {
            case KEY_UP:
                data->key = LV_KEY_UP;
                break;
            case KEY_DOWN:
                data->key = LV_KEY_DOWN;
                break;
            case KEY_RIGHT:
                data->key = LV_KEY_RIGHT;
                break;
            case KEY_LEFT:
                data->key = LV_KEY_LEFT;
                break;
            case KEY_ESC:
                data->key = LV_KEY_ESC;
                break;
            case KEY_DELETE:
                data->key = LV_KEY_DEL;
                break;
            case KEY_BACKSPACE:
                data->key = LV_KEY_BACKSPACE;
                break;
            case KEY_ENTER:
                data->key = LV_KEY_ENTER;
                break;
            case KEY_NEXT:
            case KEY_TAB:
                data->key = LV_KEY_NEXT;
                break;
            case KEY_PREVIOUS:
                data->key = LV_KEY_PREV;
                break;
            case KEY_HOME:
                data->key = LV_KEY_HOME;
                break;
            case KEY_END:
                data->key = LV_KEY_END;
                break;
            case KEY_POWER:
                if (pressed && on_key_power_cb)
                    on_key_power_cb();
                ignore = true;
                break;
            default: {
                if (!ext->xkb_state) {
                    ignore = true;
                    break;
                }
                char buffer[] = { 0, 0, 0, 0, 0 };
                int size = xkb_state_key_get_utf8(ext->xkb_state, keycode, buffer, sizeof(buffer));
                if (size == 0) {
                    ignore = true;
                    break;
                }
                data->key = *((uint32_t*) buffer);
                break;
            }}

            if (ext->xkb_state)
                xkb_state_update_key(ext->xkb_state, keycode, pressed ? XKB_KEY_DOWN : XKB_KEY_UP);

            if (ignore)
                break;

            data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

            lv_indev_read(ext->keypad);
            break;
        }
#endif

        case LIBINPUT_EVENT_POINTER_MOTION: {
            struct libinput_event_pointer *pointer_event = libinput_event_get_pointer_event(event);

            struct indev_ext *data = lv_indev_get_user_data(ext->pointer1);
            lv_display_t* display = lv_indev_get_display(ext->pointer1);

            int32_t x = data->point.x + (int32_t) libinput_event_pointer_get_dx(pointer_event);
            data->point.x = LV_CLAMP(0, x, display->hor_res - 1);
            int32_t y = data->point.y + (int32_t) libinput_event_pointer_get_dy(pointer_event);
            data->point.y = LV_CLAMP(0, y, display->ver_res - 1);

            lv_indev_read(ext->pointer1);
            break;
        }

        case LIBINPUT_EVENT_POINTER_BUTTON: {
            struct libinput_event_pointer *pointer_event = libinput_event_get_pointer_event(event);

            struct indev_ext *data = lv_indev_get_user_data(ext->pointer1);
            data->state = libinput_event_pointer_get_button_state(pointer_event) == LIBINPUT_BUTTON_STATE_PRESSED ?
                LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

            lv_indev_read(ext->pointer1);
            break;
        }

        case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE: {
            struct libinput_event_pointer *pointer_event = libinput_event_get_pointer_event(event);

            struct indev_ext *data = lv_indev_get_user_data(ext->pointer1);
            lv_display_t* display = lv_indev_get_display(ext->pointer1);

            int32_t x = libinput_event_pointer_get_absolute_x_transformed(pointer_event, display->physical_hor_res);
            int32_t y = libinput_event_pointer_get_absolute_y_transformed(pointer_event, display->physical_ver_res);
            x -= display->offset_x;
            y -= display->offset_y;
            if (x < 0 || x >= display->hor_res || y < 0 || y >= display->ver_res)
                break;
            data->point.x = x;
            data->point.y = y;

            lv_indev_read(ext->pointer1);
            break;
        }

        case LIBINPUT_EVENT_TOUCH_MOTION: {
            struct libinput_event_touch *touch_event = libinput_event_get_touch_event(event);
            int32_t slot = libinput_event_touch_get_slot(touch_event);

            lv_indev_t *pointer;
            if (slot <= 0)
                pointer = ext->pointer1;
            else if (slot == 1)
                pointer = ext->pointer2;
            else
                break;

            struct indev_ext *data = lv_indev_get_user_data(pointer);
            lv_display_t* display = lv_indev_get_display(pointer);

            int32_t x = libinput_event_touch_get_x_transformed(touch_event, display->physical_hor_res);
            int32_t y = libinput_event_touch_get_y_transformed(touch_event, display->physical_ver_res);
            x -= display->offset_x;
            y -= display->offset_y;
            if (x < 0 || x >= display->hor_res || y < 0 || y >= display->ver_res)
                break;
            data->point.x = x;
            data->point.y = y;

            lv_indev_read(pointer);
            break;
        }

        case LIBINPUT_EVENT_TOUCH_DOWN: {
            struct libinput_event_touch *touch_event = libinput_event_get_touch_event(event);
            int32_t slot = libinput_event_touch_get_slot(touch_event);

            lv_indev_t *pointer;
            if (slot <= 0)
                pointer = ext->pointer1;
            else if (slot == 1)
                pointer = ext->pointer2;
            else
                break;

            struct indev_ext *data = lv_indev_get_user_data(pointer);
            lv_display_t* display = lv_indev_get_display(pointer);

            int32_t x = libinput_event_touch_get_x_transformed(touch_event, display->physical_hor_res);
            int32_t y = libinput_event_touch_get_y_transformed(touch_event, display->physical_ver_res);
            x -= display->offset_x;
            y -= display->offset_y;
            if (x < 0 || x >= display->hor_res || y < 0 || y >= display->ver_res)
                break;
            data->point.x = x;
            data->point.y = y;
            data->state = LV_INDEV_STATE_PRESSED;

            lv_indev_read(pointer);
            break;
        }

        case LIBINPUT_EVENT_TOUCH_UP:
        case LIBINPUT_EVENT_TOUCH_CANCEL: {
            struct libinput_event_touch *touch_event = libinput_event_get_touch_event(event);
            int32_t slot = libinput_event_touch_get_slot(touch_event);

            lv_indev_t *pointer;
            if (slot <= 0)
                pointer = ext->pointer1;
            else if (slot == 1)
                pointer = ext->pointer2;
            else
                break;

            struct indev_ext *data = lv_indev_get_user_data(pointer);
            data->state = LV_INDEV_STATE_RELEASED;

            lv_indev_read(pointer);
            break;
        }

        default:
            break;
        }

        libinput_event_destroy(event);
    }
}


static void attach_input_device(struct udev_device* device) {
    const char* node = udev_device_get_devnode(device);

    enum input_device_type type = identify_input_device(device);
    switch (type) {
    case BBX_INDEV_NONE:
        return;
    case BBX_INDEV_KEY:
#ifndef BBX_APP_BUFFYBOARD
        break;
#else
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Key %s is ignored", node);
        return;
#endif
    case BBX_INDEV_KEYBOARD:
#ifndef BBX_APP_BUFFYBOARD
        if (!options.keyboard) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Keyboard %s is ignored", node);
            return;
        }
        break;
#else
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Keyboard %s is ignored", node);
        return;
#endif
    case BBX_INDEV_MOUSE:
        if (!options.pointer) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Mouse %s is ignored", node);
            return;
        }
        break;
    case BBX_INDEV_TOUCHPAD:
        if (!options.pointer) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Touchpad %s is ignored", node);
            return;
        }
        break;
    case BBX_INDEV_TOUCHSCREEN:
        if (!options.touchscreen) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Touchscreen %s is ignored", node);
            return;
        }
        break;
    default:
        break;
    }

    struct libinput_device* dev = libinput_path_add_device(context_libinput, node);
    if (!dev) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "libinput can't use %s", node);
        return;
    }

    static_assert(sizeof(type) <= sizeof(void*));
    libinput_device_set_user_data(dev, (void*) type);

    on_input_event(); /* Process LIBINPUT_EVENT_DEVICE_ADDED immediately */
}

static void attach_input_devices() {
    DIR* dir = opendir("/dev/input");
    if (!dir) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Can't open /dev/input: %s", strerror(errno));
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (strncmp(entry->d_name, "event", 5) != 0)
            continue;

        char node[sizeof("/dev/input") + strlen(entry->d_name) + 1];
        sprintf(node, "/dev/input/%s", entry->d_name);

        struct stat buffer;
        int r = stat(node, &buffer);
        if (r != 0) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "stat(%s) is failed: %s", node, strerror(errno));
            continue;
        }

        assert(S_ISCHR(buffer.st_mode));

        struct udev_device *device = udev_device_new_from_devnum(context_udev, 'c', buffer.st_rdev);
        if (!device) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Can't create udev device for %s", node);
            continue;
        }

        attach_input_device(device);

        udev_device_unref(device);
    }

    closedir(dir);
    return;
}

static void on_input_device_event() {
    bool attached = false;

    struct udev_device *device;
    while ((device = udev_monitor_receive_device(monitor))) {
        const char *action = udev_device_get_action(device);
        const char *devnode = udev_device_get_devnode(device);

        if (strcmp(action, "add") != 0)
            goto skip;

        if (!devnode || strncmp(devnode, "/dev/input/event", sizeof("/dev/input/event") - 1) != 0)
            goto skip;

        attach_input_device(device);
        attached = true;
skip:
        udev_device_unref(device);
    }

    /* Libinput does not store the state, and activates added devices immediately.
       So we have to suspend it again. */
    if (attached && libinput_is_suspended)
        libinput_suspend(context_libinput);
}


/**
 * Public functions
 */

uint8_t bbx_indev_init(int fd_epoll, const struct bbx_indev_opts* opts) {
    static const struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
    };

    context_libinput = libinput_path_create_context(&interface, NULL);
    if (!context_libinput) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not create libinput context");
        return 0;
    }

    context_udev = udev_new();
    if (!context_udev) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not create udev context");
        goto failure1;
    }

    monitor = udev_monitor_new_from_netlink(context_udev, "udev");
    if (!monitor) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not create udev monitor");
        goto failure2;
    }

    if (udev_monitor_filter_add_match_subsystem_devtype(monitor, "input", NULL) != 0)
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not add a filter for udev monitor");

    if (udev_monitor_filter_update(monitor) != 0)
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not update a filter for udev monitor");

#ifndef BBX_APP_BUFFYBOARD
    struct xkb_context *context_xkb = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!context_xkb) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Could not create xkb_context");
        goto failure3;
    }

    keymap = xkb_keymap_new_from_names(context_xkb, opts->keymap, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!keymap) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Can't compile xkb_rule_names: %s:%s:%s:%s:%s",
            opts->keymap->rules,
            opts->keymap->model,
            opts->keymap->layout,
            opts->keymap->variant,
            opts->keymap->options);
        goto failure4;
    }
#endif

    lv_display_t *display = lv_display_get_default();
    assert(display->physical_hor_res > 0);
    assert(display->physical_ver_res > 0);

    mouse_cursor = lv_image_create(lv_display_get_layer_sys(display));
    if (!mouse_cursor) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "lv_image_create() is failed");
        goto failure5;
    }
    lv_obj_add_flag(mouse_cursor, LV_OBJ_FLAG_HIDDEN);
    lv_image_set_src(mouse_cursor, &cursor);

#ifndef BBX_APP_BUFFYBOARD
    keyboard_input_group = lv_group_create();
    if (!keyboard_input_group) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "lv_group_create() is failed");
        goto failure6;
    }

    options.keyboard = opts->keyboard;
#endif
    options.pointer = opts->pointer;
    options.touchscreen = opts->touchscreen;

    attach_input_devices();

    uint8_t ret = 0;

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.ptr = __extension__ (void*) on_input_event;

    int r = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, libinput_get_fd(context_libinput), &event);
    if (r == -1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "EPOLL_CTL_ADD for libinput context is failed");
        return ret;
    }
    ret++;

    event.events = EPOLLIN;
    event.data.ptr = __extension__ (void*) on_input_device_event;

    r = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, udev_monitor_get_fd(monitor), &event);
    if (r == -1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "EPOLL_CTL_ADD for udev_monitor is failed");
        return ret;
    }
    ret++;

    return ret;


failure6:
    lv_obj_delete(mouse_cursor);
failure5:
#ifndef BBX_APP_BUFFYBOARD
    xkb_keymap_unref(keymap);
failure4:
    xkb_context_unref(context_xkb);
#endif
failure3:
    udev_monitor_unref(monitor);
failure2:
    udev_unref(context_udev);
failure1:
    libinput_unref(context_libinput);
    return 0;
}

void bbx_indev_suspend() {
    if (!libinput_is_suspended) {
        libinput_suspend(context_libinput);
        libinput_is_suspended = true;
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "libinput is suspended");
    }
}

void bbx_indev_resume() {
    if (libinput_is_suspended) {
        libinput_resume(context_libinput);
        libinput_is_suspended = false;
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "libinput is resumed");
    }
}

#ifndef BBX_APP_BUFFYBOARD
bool bbx_indev_is_keyboard_connected() {
    return num_keyboards != 0;
}

void bbx_indev_set_key_power_cb(void (*callback)()) {
    on_key_power_cb = callback;
}
#endif
