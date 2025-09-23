/**
 * Copyright 2025 Clayton Craft
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef F0_CONFIG_H
#define F0_CONFIG_H

#include "../shared/backends.h"
#include "../shared/config.h"
#include "../shared/themes.h"

#include "sq2lv_layouts.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * General options
 */
typedef struct {
    /* Backend to use */
    bbx_backends_backend_id_t backend;
} f0_config_opts_general;

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
} f0_config_opts_keyboard;

/**
 * Options related to the theme
 */
typedef struct {
    /* Default theme */
    bbx_themes_theme_id_t default_id;
    /* Alternate theme */
    bbx_themes_theme_id_t alternate_id;
} f0_config_opts_theme;

/**
 * Options related to the password textarea
 */
typedef struct {
    /* If true, disguise the entered text with dots */
    bool obscured;
    /* The character to use for disguising the entered text */
    const char *bullet;
} f0_config_opts_textarea;

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
} f0_config_opts_input;

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
} f0_config_opts_quirks;

/**
 * Form configuration options
*/

/**
 * Intro section configuration
 */
typedef struct {
    /* Welcome screen title */
    const char *title;
    /* Welcome screen body text */
    const char *body;
} f0_config_intro;

/**
 * Form field types
 */
typedef enum {
    F0_FIELD_TYPE_TEXT,
    F0_FIELD_TYPE_PASSWORD
} f0_field_type_t;

/**
 * Form field definition
 */
typedef struct {
    char *name;
    f0_field_type_t type;
    char *label;
    bool required;
} f0_form_field_t;

typedef struct {
    /* General options */
    f0_config_opts_general general;
    /* Options related to the keyboard */
    f0_config_opts_keyboard keyboard;
    /* Options related to the theme */
    f0_config_opts_theme theme;
    /* Options related to the password textarea */
    f0_config_opts_textarea textarea;
    /* Options related to input devices */
    f0_config_opts_input input;
    /* Options related to (normally unneeded) quirks */
    f0_config_opts_quirks quirks;
    /* Intro section */
    f0_config_intro intro;
    /* Form fields */
    f0_form_field_t *fields;
    /* Number of form fields/pages */
    int num_fields;
} f0_config_opts;

/**
 * Initialize config options with default values.
 *
 * @param opts pointer to the options struct
 */
void f0_config_init_opts(f0_config_opts *opts);

/**
 * Find configuration files in a directory and parse them in alphabetic order.
 *
 * @param path directory path
 * @param opts pointer for writing the parsed options into
 */
void f0_config_parse_directory(const char *path, f0_config_opts *opts);

/**
 * Parse one or more configuration files.
 *
 * @param files paths to configuration files
 * @param num_files number of configuration files
 * @param opts pointer for writing the parsed options into
 */
void f0_config_parse_files(const char **files, int num_files, f0_config_opts *opts);

/**
 * Parse a configuration file.
 *
 * @param path path to configuration file
 * @param opts pointer for writing the parsed options into
 */
void f0_config_parse_file(const char *path, f0_config_opts *opts);

#endif /* F0_CONFIG_H */
