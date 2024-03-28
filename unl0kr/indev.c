/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "indev.h"

#include "../shared/cursor/cursor.h"
#include "../shared/log.h"

#include "lvgl/src/indev/lv_indev_private.h"

#include <libinput.h>
#include <libudev.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include <linux/input.h>

#include <sys/select.h>


/**
 * Defines
 */

#define INPUT_DEVICE_NODE_PREFIX "/dev/input/event"

#define MAX_KEYBOARD_DEVS 4
#define MAX_POINTER_DEVS 4
#define MAX_TOUCHSCREEN_DEVS 1


/**
 * Static variables
 */

static lv_libinput_capability allowed_capability = LV_LIBINPUT_CAPABILITY_NONE;

static struct udev *context = NULL;
static struct udev_monitor *monitor = NULL;
static int monitor_fd = -1;

struct input_device {
  char *node;
  lv_libinput_capability capability;
  lv_indev_t *indev;
};

static struct input_device **devices = NULL;
static int num_devices = 0;
static int num_connected_devices = 0;

lv_group_t *keyboard_input_group = NULL;
lv_obj_t *cursor_obj = NULL;


/**
 * Static prototypes
 */

/**
 * Test whether a device can act as a keyboard device.
 * 
 * @param device the device to test
 * @return true if the device has the keyboard capability
 */
static bool is_keyboard_device(struct input_device *device);

/**
 * Test whether a device can act as a pointer device.
 * 
 * @param device the device to test
 * @return true if the device has the pointer capability
 */
static bool is_pointer_device(struct input_device *device);

/**
 * Test whether a device can act as a touch device.
 *
 * @param device the device to test
 * @return true if the device has the touch capability
 */
static bool is_touch_device(struct input_device *device);

/**
 * Convert a device capability into a descriptive string.
 * 
 * @param capability input device capability
 * @return textual description
 */
static char *capability_to_str(lv_libinput_capability capability);

/**
 * Connect a specific input device using its udev device.
 *
 * @param device udev device
 */
static void connect_udev_device(struct udev_device *device);

/**
 * Connect a specific input device using its device node.
 *
 * @param node device node path
 */
static void connect_devnode(const char *node);

/**
 * Disconnect a specific input device using its device node.
 *
 * @param node device node path
 */
static void disconnect_devnode(const char *node);

/**
 * Disconnect a specific input device using its index in the devices array.
 *
 * @param idx index in devices array
 */
static void disconnect_idx(int idx);

/**
 * Set up the input group for a keyboard device.
 *
 * @param device the input device
 */
static void set_keyboard_input_group(struct input_device *device);

/**
 * Set up the mouse cursor image for a pointer device.
 * 
 * @param device the input device
 */
static void set_mouse_cursor(struct input_device *device);


/**
 * Static functions
 */

static bool is_keyboard_device(struct input_device *device) {
    return (device->capability & LV_LIBINPUT_CAPABILITY_KEYBOARD) != LV_LIBINPUT_CAPABILITY_NONE;
}

static bool is_pointer_device(struct input_device *device) {
    return (device->capability & LV_LIBINPUT_CAPABILITY_POINTER) != LV_LIBINPUT_CAPABILITY_NONE;
}

static bool is_touch_device(struct input_device *device) {
    return (device->capability & LV_LIBINPUT_CAPABILITY_TOUCH) != LV_LIBINPUT_CAPABILITY_NONE;
}

static char *capability_to_str(lv_libinput_capability capability) {
    if (capability == LV_LIBINPUT_CAPABILITY_KEYBOARD) {
        return "keyboard";
    }
    if (capability == LV_LIBINPUT_CAPABILITY_POINTER) {
        return "pointer";
    }
    if (capability == LV_LIBINPUT_CAPABILITY_TOUCH) {
        return "touch";
    }
    return "none";
}

static void connect_udev_device(struct udev_device *device) {
    /* Obtain and verify device node */
    const char *node = udev_device_get_devnode(device);
    if (!node || strncmp(node, INPUT_DEVICE_NODE_PREFIX, strlen(INPUT_DEVICE_NODE_PREFIX)) != 0) {
        bb_log(BB_LOG_LEVEL_VERBOSE, "Ignoring unsupported input device %s", udev_device_get_syspath(device));
        return;
    }

    /* Connect device using its node */
    connect_devnode(node);
}

static void connect_devnode(const char *node) {
    /* Check if the device is already connected */
    for (int i = 0; i < num_connected_devices; ++i) {
        if (strcmp(devices[i]->node, node) == 0) {
            bb_log(BB_LOG_LEVEL_WARNING, "Ignoring already connected input device %s", node);
            return;
        }
    }

    /* Double array size every time it's filled */
    if (num_connected_devices == num_devices) {
        /* Re-allocate array */
        struct input_device **tmp = realloc(devices, (2 * num_devices + 1) * sizeof(struct input_device *));
        if (!tmp) {
            bb_log(BB_LOG_LEVEL_ERROR, "Could not reallocate memory for input device array");
            return;
        }
        devices = tmp;

        /* Update size counter */
        num_devices = 2 * num_devices + 1;

        /* Fill empty space with zeros */
        lv_memzero(devices + num_connected_devices, (num_devices - num_connected_devices) * sizeof(struct input_device *));
    }

    /* Allocate memory for new input device and insert it */
    struct input_device *device = malloc(sizeof(struct input_device));
    lv_memzero(device, sizeof(struct input_device));
    devices[num_connected_devices] = device;

    /* Copy the node path so that it can be used beyond the caller's scope */
    device->node = strdup(node);

    /* Initialise the indev and obtain the libinput device */
    device->indev = lv_libinput_create(LV_INDEV_TYPE_NONE, device->node);
    if (!device->indev) {
        bb_log(BB_LOG_LEVEL_WARNING, "Aborting connection of input device %s because libinput failed to connect it", node);
        disconnect_idx(num_connected_devices);
        return;
    }
    lv_libinput_t *dsc = lv_indev_get_driver_data(device->indev);
    struct libinput_device *device_libinput = dsc->libinput_device;
    if (!device_libinput) {
        bb_log(BB_LOG_LEVEL_WARNING, "Aborting connection of input device %s because libinput failed to connect it", node);
        disconnect_idx(num_connected_devices);
        return;
    }

    /* Obtain device capabilities */
    device->capability = lv_libinput_query_capability(device_libinput);

    /* If the device doesn't have any supported capabilities, exit */
    if ((device->capability & allowed_capability) == LV_LIBINPUT_CAPABILITY_NONE)  {
        bb_log(BB_LOG_LEVEL_WARNING, "Aborting connection of input device %s because it has no allowed capabilities", node);
        disconnect_idx(num_connected_devices);
        return;
    }

    /*
     * Set up indev type and related properties
     *
     * FIXME: Some libinput devices (notably, certain hid-i2c keyboards)
     * report both keyboard and pointer capability. Since lvgl expects
     * every input device to have only one type, we register those devices
     * as keyboards, considering this capability more useful. However
     * we must also assume that keyboard+touch devices are touch.
     */

    if (is_touch_device(device)) {
        device->indev->type = LV_INDEV_TYPE_POINTER;
        device->indev->long_press_repeat_time = USHRT_MAX;
    } else if (is_keyboard_device(device)) {
        device->indev->type = LV_INDEV_TYPE_KEYPAD;
    } else if (is_pointer_device(device)) {
        device->indev->type = LV_INDEV_TYPE_POINTER;
        device->indev->long_press_repeat_time = USHRT_MAX;
    }

    /* Set the input group for keyboard devices */
    if (is_keyboard_device(device)) {
        set_keyboard_input_group(device);
    }

    /* Set the mouse cursor for pointer devices */
    if (is_pointer_device(device)) {
        set_mouse_cursor(device);
    }

    /* Increment connected device count */
    num_connected_devices++;

    bb_log(BB_LOG_LEVEL_VERBOSE, "Connected input device %s (%s)", node, capability_to_str(device->capability));
}

static void disconnect_udev_device(struct udev_device *device) {
    /* Obtain and verify device node */
    const char *node = udev_device_get_devnode(device);
    if (!node || strncmp(node, INPUT_DEVICE_NODE_PREFIX, strlen(INPUT_DEVICE_NODE_PREFIX)) != 0) {
        bb_log(BB_LOG_LEVEL_VERBOSE, "Ignoring unsupported input device %s", udev_device_get_syspath(device));
        return;
    }

    /* Disconnect device using its node */
    disconnect_devnode(node);
}

static void disconnect_devnode(const char *node) {
    /* Find connected device matching the specified node */
    int idx = -1;
    for (int i = 0; i < num_connected_devices; ++i) {
        if (strcmp(devices[i]->node, node) == 0) {
            idx = i;
            break;
        }
    }

    /* If no matching device was found, exit */
    if (idx < 0) {
        bb_log(BB_LOG_LEVEL_WARNING, "Ignoring already disconnected input device %s", node);
        return;
    }

    /* Disconnect device using its index */
    disconnect_idx(idx);

    /* Shift subsequent devices forward */
    for (int i = idx + 1; i < num_connected_devices; ++i) {
        devices[i - 1] = devices[i];

        /* Zero out the last element after shifting it forward */
        if (i == num_connected_devices - 1) {
            lv_memzero(devices + i, sizeof(struct input_device *));
        }
    }
    
    /* Decrement connected device count */
    --num_connected_devices;

    bb_log(BB_LOG_LEVEL_VERBOSE, "Disconnected input device %s", node);
}

static void disconnect_idx(int idx) {
    /* Delete LVGL indev */
    if (devices[idx]->indev) {
        lv_libinput_delete(devices[idx]->indev);
    }

    /* Free previously copied node path */
    if (devices[idx]->node) {
        free(devices[idx]->node);
    }

    /* Deallocate memory and zero out freed array element */
    free(devices[idx]);
    lv_memzero(devices + idx, sizeof(struct input_device *));
}

static void set_keyboard_input_group(struct input_device *device) {
    /* Ignore non-keyboard devices */
    if (!is_keyboard_device(device)) {
        return;
    }

    /* Apply the input group */
    lv_indev_set_group(device->indev, keyboard_input_group);
}

static void set_mouse_cursor(struct input_device *device) {
    /* Ignore non-pointer devices */
    if (!is_pointer_device(device)) {
        return;
    }

    /* Initialise cursor image if needed */
    if (!cursor_obj) {
        cursor_obj = lv_img_create(lv_scr_act());
        lv_img_set_src(cursor_obj, &bb_cursor_img_dsc);
    }

    /* Apply the cursor image */
    lv_indev_set_cursor(device->indev, cursor_obj);
}


/**
 * Public functions
 */

void ul_indev_set_allowed_device_capability(bool keyboard, bool pointer, bool touchscreen) {
    allowed_capability = LV_LIBINPUT_CAPABILITY_NONE;
    if (keyboard) {
        allowed_capability |= LV_LIBINPUT_CAPABILITY_KEYBOARD;
    }
    if (pointer) {
        allowed_capability |= LV_LIBINPUT_CAPABILITY_POINTER;
    }
    if (touchscreen) {
        allowed_capability |= LV_LIBINPUT_CAPABILITY_TOUCH;
    }
}

void ul_indev_set_keyboard_input_group(lv_group_t *group) {
    /* Store the group */
    keyboard_input_group = group;

    /* Apply the group on all connected keyboard devices */
    for (int i = 0; i < num_connected_devices; ++i) {
        if (is_keyboard_device(devices[i])) {
            set_keyboard_input_group(devices[i]);
        }
    }
}

void ul_indev_auto_connect() {
    bb_log(BB_LOG_LEVEL_VERBOSE, "Auto-connecting supported input devices");

    /* Make sure udev context is initialised */
    if (!context) {
        context = udev_new();
        if (!context) {
            bb_log(BB_LOG_LEVEL_WARNING, "Could not create udev context");
            return;
        }
    }

    /* Scan for available input devices */
    struct udev_enumerate *enumerate = udev_enumerate_new(context);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);

    /* Prepare for enumerating found devices */
    struct udev_list_entry *first_entry = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, first_entry) {
        /* Obtain system path */
        const char *path = udev_list_entry_get_name(entry);

        /* Create udev device */
        struct udev_device *device = udev_device_new_from_syspath(context, path);
        if (!device) {
            bb_log(BB_LOG_LEVEL_WARNING, "Could not create udev device for %s", path);
            continue;
        }

        /* Connect device */
        connect_udev_device(device);

        /* Unreference udev device */
        udev_device_unref(device);
    }

    /* Unreference enumeration */
    udev_enumerate_unref(enumerate);
}

void ul_indev_start_monitor() {
    /* Make sure udev context is initialised */
    if (!context) {
        context = udev_new();
        if (!context) {
            bb_log(BB_LOG_LEVEL_WARNING, "Could not create udev context");
            return;
        }
    }

    /* Check if monitor is already running */
    if (monitor) {
        bb_log(BB_LOG_LEVEL_WARNING, "Not starting udev monitor because it is already running");
        return;
    }

    /* Create new monitor */
    monitor = udev_monitor_new_from_netlink(context, "udev");
    if (!monitor) {
        bb_log(BB_LOG_LEVEL_WARNING, "Could not create udev monitor");
        ul_indev_stop_monitor();
        return;
    }

    /* Apply input subsystem filter */
    if (udev_monitor_filter_add_match_subsystem_devtype(monitor, "input", NULL) < 0) {
        bb_log(BB_LOG_LEVEL_WARNING, "Could not add input subsystem filter for udev monitor");
    }

    /* Start monitor */
    if (udev_monitor_enable_receiving(monitor) < 0) {
        bb_log(BB_LOG_LEVEL_WARNING, "Could not enable udev monitor");
        ul_indev_stop_monitor();
        return;
    }

    /* Obtain monitor file descriptor */
    if ((monitor_fd = udev_monitor_get_fd(monitor)) < 0) {
        bb_log(BB_LOG_LEVEL_WARNING, "Could not acquire file descriptor for udev monitor");
        ul_indev_stop_monitor();
        return;
    }
}

void ul_indev_stop_monitor() {
    /* Unreference monitor */
    if (monitor) {
        udev_monitor_unref(monitor);
        monitor = NULL;
    }

    /* Reset monitor file descriptor */
    if (monitor_fd >= 0) {
        monitor_fd = -1;
    }

    /* Unreference udev context */
    if (context) {
        udev_unref(context);
        context = NULL;
    }
}

void ul_indev_query_monitor() {
    /* Make sure the monitor is running */
    if (!monitor) {
        bb_log(BB_LOG_LEVEL_ERROR, "Cannot query udev monitor because it is not running");
        return;
    }

    /* Prepare file descriptor set for reading */
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(monitor_fd, &fds);

    /* Set up timeval to not block when no updates are available */
    struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };

    /* Read and process all updates */
    while (select(monitor_fd + 1, &fds, NULL, NULL, &tv) > 0 && FD_ISSET(monitor_fd, &fds)) {
        /* Obtain udev device */
        struct udev_device *device = udev_monitor_receive_device(monitor);
        if (!device) {
            continue;
        }

        /* Obtain action */
        const char *action = udev_device_get_action(device);

        /* Connect or disconnect the device */
        if (strcmp(action, "add") == 0) {
            connect_udev_device(device);
        } else if (strcmp(action, "remove") == 0) {
            disconnect_udev_device(device);
        }

        /* Unreference udev device */
        udev_device_unref(device);
    }
}

bool ul_indev_is_keyboard_connected() {
    for (int i = 0; i < num_connected_devices; ++i) {
        if (is_keyboard_device(devices[i])) {
            return true;
        }
    }
    return false;
}
