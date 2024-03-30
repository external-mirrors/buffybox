/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "config.h"

#include "../shared/config.h"
#include "../shared/log.h"
#include "../squeek2lvgl/sq2lv.h"

#include "lvgl/lvgl.h"

#include <ini.h>
#include <stdlib.h>
#include <string.h>


/**
 * Static prototypes
 */

/**
 * Handle parsing events from INIH.
 *
 * @param user_data pointer to user data
 * @param section current section name
 * @param key option key
 * @param value option value
 * @return 0 on error, non-0 otherwise
 */
static int parsing_handler(void* user_data, const char* section, const char* key, const char* value);


/**
 * Static functions
 */

static int parsing_handler(void* user_data, const char* section, const char* key, const char* value) {
    ul_config_opts *opts = (ul_config_opts *)user_data;

    if (strcmp(section, "general") == 0) {
        if (strcmp(key, "animations") == 0) {
            if (bbx_config_parse_bool(value, &(opts->general.animations))) {
                return 1;
            }
        } else if (strcmp(key, "backend") == 0) {
            ul_backends_backend_id_t id = ul_backends_find_backend_with_name(value);
            if (id != UL_BACKENDS_BACKEND_NONE) {
                opts->general.backend = id;
                return 1;
            }
        } else if (strcmp(key, "timeout") == 0) {
            /* Use a max ceiling of 60 minutes (3600 secs) */
            opts->general.timeout = (uint16_t)LV_MIN(strtoul(value, (char **)NULL, 10), 3600);
            return 1;
        }
    } else if (strcmp(section, "keyboard") == 0) {
        if (strcmp(key, "autohide") == 0) {
            if (bbx_config_parse_bool(value, &(opts->keyboard.autohide))) {
                return 1;
            }
        } else if (strcmp(key, "layout") == 0) {
            sq2lv_layout_id_t id = sq2lv_find_layout_with_short_name(value);
            if (id != SQ2LV_LAYOUT_NONE) {
                opts->keyboard.layout_id = id;
                return 1;
            }
        } else if (strcmp(key, "popovers") == 0) {
            if (bbx_config_parse_bool(value, &(opts->keyboard.popovers))) {
                return 1;
            }
        }
    } else if (strcmp(section, "textarea") == 0) {
        if (strcmp(key, "obscured") == 0) {
            if (bbx_config_parse_bool(value, &(opts->textarea.obscured))) {
                return 1;
            }
        } else if (strcmp(key, "bullet") == 0) {
            char *bullet = strdup(value);
            if (bullet) {
                opts->textarea.bullet = bullet;
                return 1;
            }
        }
    } else if (strcmp(section, "theme") == 0) {
        if (strcmp(key, "default") == 0) {
            bbx_themes_theme_id_t id = bbx_themes_find_theme_with_name(value);
            if (id != BBX_THEMES_THEME_NONE) {
                opts->theme.default_id = id;
                return 1;
            }
        } else if (strcmp(key, "alternate") == 0) {
            bbx_themes_theme_id_t id = bbx_themes_find_theme_with_name(value);
            if (id != BBX_THEMES_THEME_NONE) {
                opts->theme.alternate_id = id;
                return 1;
            }
        }
    } else if (strcmp(section, "input") == 0) {
        if (strcmp(key, "keyboard") == 0) {
            if (bbx_config_parse_bool(value, &(opts->input.keyboard))) {
                return 1;
            }
        } else if (strcmp(key, "pointer") == 0) {
            if (bbx_config_parse_bool(value, &(opts->input.pointer))) {
                return 1;
            }
        } else if (strcmp(key, "touchscreen") == 0) {
            if (bbx_config_parse_bool(value, &(opts->input.touchscreen))) {
                return 1;
            }
        }
    } else if (strcmp(section, "quirks") == 0) {
        if (strcmp(key, "fbdev_force_refresh") == 0) {
            if (bbx_config_parse_bool(value, &(opts->quirks.fbdev_force_refresh))) {
                return 1;
            }
        } else if (strcmp(key, "terminal_prevent_graphics_mode") == 0) {
            if (bbx_config_parse_bool(value, &(opts->quirks.terminal_prevent_graphics_mode))) {
                return 1;
            }
        } else if (strcmp(key, "terminal_allow_keyboard_input") == 0) {
            if (bbx_config_parse_bool(value, &(opts->quirks.terminal_allow_keyboard_input))) {
                return 1;
            }
        }
    }

    bbx_log(BBX_LOG_LEVEL_ERROR, "Ignoring invalid config value \"%s\" for key \"%s\" in section \"%s\"", value, key, section);
    return 1; /* Return 1 (true) so that we can use the return value of ini_parse exclusively for file-level errors (e.g. file not found) */
}


/**
 * Public functions
 */

void ul_config_init_opts(ul_config_opts *opts) {
    opts->general.animations = false;
    opts->general.backend = ul_backends_backends[0] == NULL ? UL_BACKENDS_BACKEND_NONE : 0;
    opts->general.timeout = 0;
    opts->keyboard.autohide = true;
    opts->keyboard.layout_id = SQ2LV_LAYOUT_US;
    opts->keyboard.popovers = true;
    opts->textarea.obscured = true;
    opts->textarea.bullet = LV_SYMBOL_BULLET;
    opts->theme.default_id = BBX_THEMES_THEME_BREEZY_DARK;
    opts->theme.alternate_id = BBX_THEMES_THEME_BREEZY_LIGHT;
    opts->input.keyboard = true;
    opts->input.pointer = true;
    opts->input.touchscreen = true;
    opts->quirks.fbdev_force_refresh = false;
    opts->quirks.terminal_prevent_graphics_mode = false;
    opts->quirks.terminal_allow_keyboard_input = false;
}

void ul_config_parse_directory(const char *path, ul_config_opts *opts) {
    /* Find files in directory */
    char **found = NULL;
    int num_found = 0;
    bbx_config_find_files(path, &found, &num_found);

    /* Sort and parse files */
    qsort(found, num_found, sizeof(char *), bbx_config_compare_strings);
    ul_config_parse_files((const char **)found, num_found, opts);

    /* Free memory */
    for (int i = 0; i < num_found; ++i) {
        free(found[i]);
    }
    free(found);
}

void ul_config_parse_files(const char **files, int num_files, ul_config_opts *opts) {
    for (int i = 0; i < num_files; ++i) {
        ul_config_parse_file(files[i], opts);
    }
}

void ul_config_parse_file(const char *path, ul_config_opts *opts) {
    bbx_log(BBX_LOG_LEVEL_VERBOSE, "Parsing config file %s", path);
    if (ini_parse(path, parsing_handler, opts) != 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Ignoring invalid config file %s", path);
    }
}
