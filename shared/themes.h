/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_THEMES_H
#define BBX_THEMES_H

#include "theme.h"

/* Theme IDs, values can be used as indexes into the bbx_themes_themes array */
typedef enum {
    BBX_THEMES_THEME_NONE = -1,
    BBX_THEMES_THEME_BREEZY_LIGHT = 0,
    BBX_THEMES_THEME_BREEZY_DARK = 1,
    BBX_THEMES_THEME_PMOS_LIGHT = 2,
    BBX_THEMES_THEME_PMOS_DARK = 3,
    BBX_THEMES_THEME_ADWAITA_DARK = 4,
} bbx_themes_theme_id_t;

/* Themes */
extern const int bbx_themes_num_themes;
extern const bbx_theme *bbx_themes_themes[];

/**
 * Find the first theme with a given name.
 *
 * @param name theme name
 * @return ID of the first matching theme or BBX_THEMES_THEME_NONE if no theme matched
 */
bbx_themes_theme_id_t bbx_themes_find_theme_with_name(const char *name);

#endif /* BBX_THEMES_H */
