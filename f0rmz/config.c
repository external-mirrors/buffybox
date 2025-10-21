/**
 * Copyright 2025 Clayton Craft
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "config.h"

#include "../shared/config.h"
#include "../shared/log.h"
#include "../squeek2lvgl/sq2lv.h"

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
    f0_config_opts *opts = (f0_config_opts *)user_data;

    if (strcmp(section, "general") == 0) {
        if (strcmp(key, "backend") == 0) {
            bbx_backends_backend_id_t id = bbx_backends_find_backend_with_name(value);
            if (id != BBX_BACKENDS_BACKEND_NONE) {
                opts->general.backend = id;
                return 1;
            }
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
    } else if (strcmp(section, "hardware keyboard") == 0) {
        if (strcmp(key, "rules") == 0) {
            opts->hw_keyboard.rules = strdup(value);
            return 1;
        } else if (strcmp(key, "model") == 0) {
            opts->hw_keyboard.model = strdup(value);
            return 1;
        } else if (strcmp(key, "layout") == 0) {
            opts->hw_keyboard.layout = strdup(value);
            return 1;
        } else if (strcmp(key, "variant") == 0) {
            opts->hw_keyboard.variant = strdup(value);
            return 1;
        } else if (strcmp(key, "options") == 0) {
            opts->hw_keyboard.options = strdup(value);
            return 1;
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
    } else if (strcmp(section, "intro") == 0) {
        if (strcmp(key, "title") == 0) {
            char *title = strdup(value);
            if (title) {
                opts->intro.title = title;
                return 1;
            }
        } else if (strcmp(key, "body") == 0) {
            char *body = strdup(value);
            if (body) {
                opts->intro.body = body;
                return 1;
            }
        }
    } else if (strncmp(section, "form.", 5) == 0) {
        /* Extract field name */
        const char *field_name = section + 5;

        /* Find or create field */
        f0_form_field_t *field = NULL;
        for (int i = 0; i < opts->num_fields; i++) {
            if (strcmp(opts->fields[i].name, field_name) == 0) {
                field = &opts->fields[i];
                break;
            }
        }

        if (!field) {
            /* Create new field */
            opts->fields = realloc(opts->fields, (opts->num_fields + 1) * sizeof(f0_form_field_t));
            field = &opts->fields[opts->num_fields];
            lv_memzero(field, sizeof(f0_form_field_t));
            field->name = strdup(field_name);
            opts->num_fields++;
        }

        /* Parse field properties */
        if (strcmp(key, "type") == 0) {
            if (strcmp(value, "text") == 0) {
                field->type = F0_FIELD_TYPE_TEXT;
                return 1;
            } else if (strcmp(value, "password") == 0) {
                field->type = F0_FIELD_TYPE_PASSWORD;
                return 1;
            }
        } else if (strcmp(key, "label") == 0) {
            field->label = strdup(value);
            return 1;
        } else if (strcmp(key, "required") == 0) {
            if (bbx_config_parse_bool(value, &field->required)) {
                return 1;
            }
        }
    }
    bbx_log(BBX_LOG_LEVEL_ERROR, "Ignoring invalid config value \"%s\" for key \"%s\" in section \"%s\"", value, key, section);
    return 1; /* Return 1 (true) so that we can use the return value of ini_parse exclusively for file-level errors */
}

/**
 * Public functions
 */

void f0_config_init_opts(f0_config_opts *opts) {
    opts->general.backend = 0;
    opts->intro.title = "A Form!";
    opts->intro.body = "Please complete the following form.";
    opts->theme.default_id = BBX_THEMES_THEME_BREEZY_DARK;
    opts->theme.alternate_id = BBX_THEMES_THEME_BREEZY_LIGHT;
    opts->keyboard.autohide = true;
    opts->keyboard.layout_id = SQ2LV_LAYOUT_US;
    opts->keyboard.popovers = true;
    opts->textarea.obscured = true;
    opts->textarea.bullet = LV_SYMBOL_BULLET;
    opts->input.keyboard = true;
    opts->input.pointer = true;
    opts->input.touchscreen = true;
    opts->hw_keyboard.rules = NULL;
    opts->hw_keyboard.model = NULL;
    opts->hw_keyboard.layout = NULL;
    opts->hw_keyboard.variant = NULL;
    opts->hw_keyboard.options = NULL;
    opts->quirks.fbdev_force_refresh = false;
    opts->quirks.terminal_prevent_graphics_mode = false;
    opts->quirks.terminal_allow_keyboard_input = false;
    opts->fields = NULL;
    opts->num_fields = 0;
}

void f0_config_parse_directory(const char *path, f0_config_opts *opts) {
    /* Find files in directory */
    char **found = NULL;
    int num_found = 0;
    bbx_config_find_files(path, &found, &num_found);

    /* Sort and parse files */
    qsort(found, num_found, sizeof(char *), bbx_config_compare_strings);
    f0_config_parse_files((const char **)found, num_found, opts);

    /* Free memory */
    for (int i = 0; i < num_found; ++i) {
        free(found[i]);
    }
    free(found);
}

void f0_config_parse_files(const char **files, int num_files, f0_config_opts *opts) {
    for (int i = 0; i < num_files; ++i) {
        f0_config_parse_file(files[i], opts);
    }
}

void f0_config_parse_file(const char *path, f0_config_opts *opts) {
    bbx_log(BBX_LOG_LEVEL_VERBOSE, "Parsing config file %s", path);
    if (ini_parse(path, parsing_handler, opts) != 0) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Ignoring invalid config file %s", path);
    }
}
