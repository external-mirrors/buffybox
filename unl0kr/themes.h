/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef UL_THEMES_H
#define UL_THEMES_H

#include "theme.h"
#include "log.h"

/* Theme IDs, values can be used as indexes into the ul_themes_themes array */
typedef enum {
    UL_THEMES_THEME_NONE = -1,
    UL_THEMES_THEME_BREEZY_LIGHT = 0,
    UL_THEMES_THEME_BREEZY_DARK = 1,
    UL_THEMES_THEME_PMOS_LIGHT = 2,
    UL_THEMES_THEME_PMOS_DARK = 3
} ul_themes_theme_id_t;

/* Themes */
extern const int ul_themes_num_themes;
extern const ul_theme *ul_themes_themes[];

/**
 * Find the first theme with a given name.
 *
 * @param name theme name
 * @return ID of the first matching theme or UL_THEMES_THEME_NONE if no theme matched
 */
ul_themes_theme_id_t ul_themes_find_theme_with_name(const char *name);

#endif /* UL_THEMES_H */
