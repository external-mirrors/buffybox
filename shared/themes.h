/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BB_THEMES_H
#define BB_THEMES_H

#include "theme.h"

/* Theme IDs, values can be used as indexes into the bb_themes_themes array */
typedef enum {
    BB_THEMES_THEME_NONE = -1,
    BB_THEMES_THEME_BREEZY_LIGHT = 0,
    BB_THEMES_THEME_BREEZY_DARK = 1,
    BB_THEMES_THEME_PMOS_LIGHT = 2,
    BB_THEMES_THEME_PMOS_DARK = 3
} bb_themes_theme_id_t;

/* Themes */
extern const int bb_themes_num_themes;
extern const bb_theme *bb_themes_themes[];

/**
 * Find the first theme with a given name.
 *
 * @param name theme name
 * @return ID of the first matching theme or BB_THEMES_THEME_NONE if no theme matched
 */
bb_themes_theme_id_t bb_themes_find_theme_with_name(const char *name);

#endif /* BB_THEMES_H */
