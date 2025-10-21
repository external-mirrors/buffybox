/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef UL_CONFIG_H
#define UL_CONFIG_H


#include "../shared/backends.h"
#include "../shared/themes.h"

#include "sq2lv_layouts.h"

#include <stdbool.h>
#include <stdint.h>
#include <xkbcommon/xkbcommon.h>

/**
 * General options
 */
typedef struct {
    /* If true, use animations */
    bool animations;
    /* Backend to use */
    bbx_backends_backend_id_t backend;
    /* Timeout (in seconds) - once elapsed, the device will shutdown. 0 (default) to disable */
    uint16_t timeout;
} ul_config_opts_general;

/**
 * Options related to the keyboard
 */
typedef struct {
    /* If true, hide the keyboard on launch when a hardware keyboard is connected */
    bool autohide;
    /* Keyboard layout */
    sq2lv_layout_id_t layout_id;
    /* If true, display key popovers on press */
    bool popovers;
} ul_config_opts_keyboard;

/**
 * Options related to the password textarea
 */
typedef struct {
    /* If true, disguise the entered text with dots */
    bool obscured;
    /* The character to use for disguising the entered text */
    const char *bullet;
} ul_config_opts_textarea;

/**
 * Options related to the theme
 */
typedef struct {
    /* Default theme */
    bbx_themes_theme_id_t default_id;
    /* Alternate theme */
    bbx_themes_theme_id_t alternate_id;
} ul_config_opts_theme;

/**
 * Options related to input devices
 */
typedef struct {
    /* If true and a keyboard device is connected, use it for input */
    bool keyboard;
    /* If true and a pointer device is connected, use it for input */
    bool pointer;
    /* If true and a touchscreen device is connected, use it for input */
    bool touchscreen;
} ul_config_opts_input;

/**
 * (Normally unneeded) quirky options
 */
typedef struct {
    /* If true and using the framebuffer backend, force a refresh on every draw operation */
    bool fbdev_force_refresh;
    /* If true, do *not* switch terminal into graphics mode (will show terminal command prompt) */
    bool terminal_prevent_graphics_mode;
    /* If true, do *not* turn off terminal keyboard input (will show entered characters) */
    bool terminal_allow_keyboard_input;
} ul_config_opts_quirks;

/**
 * Options parsed from config file(s)
 */
typedef struct {
    /* General options */
    ul_config_opts_general general;
    /* Options related to the keyboard */
    ul_config_opts_keyboard keyboard;
    /* Options related to the password textarea */
    ul_config_opts_textarea textarea;
    /* Options related to the theme */
    ul_config_opts_theme theme;
    /* Options related to input devices */
    ul_config_opts_input input;
    /* Options to create a keymap for hardware keyboards */
    struct xkb_rule_names hw_keyboard;
    /* Options related to (normally unneeded) quirks */
    ul_config_opts_quirks quirks;
} ul_config_opts;

/**
 * Initialise a config options struct with default values.
 * 
 * @param opts pointer to the options struct
 */
void ul_config_init_opts(ul_config_opts *opts);

/**
 * Find configuration files in a directory and parse them in alphabetic order.
 * 
 * @param path directory path
 * @param opts pointer for writing the parsed options into
 */
void ul_config_parse_directory(const char *path, ul_config_opts *opts);

/**
 * Parse one or more configuration files.
 * 
 * @param files paths to configuration files
 * @param num_files number of configuration files
 * @param opts pointer for writing the parsed options into
 */
void ul_config_parse_files(const char **files, int num_files, ul_config_opts *opts);

/**
 * Parse a configuration file.
 * 
 * @param path path to configuration file
 * @param opts pointer for writing the parsed options into
 */
void ul_config_parse_file(const char *path, ul_config_opts *opts);

#endif /* UL_CONFIG_H */
