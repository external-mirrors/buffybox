/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_CONFIG_H
#define BBX_CONFIG_H

#include <stdbool.h>

/**
 * Compares two strings from opaque types.
 *
 * @param a first string as void pointer
 * @param b second string as void pointer
 * @return a positive integer if a > b, a negative integer if a < b and 0 otherwise
 */
int bbx_config_compare_strings(const void* a, const void* b);

/**
 * Non-recursively searches a directory for configuration files.
 * 
 * @param path folder to search in
 * @param found pointer to write found configuration file names into (to be freed by the caller)
 * @param num_found pointer to write number of found files into
 */
void bbx_config_find_files(const char *path, char ***found, int *num_found);

/**
 * Attempt to parse a boolean value.
 *
 * @param value string to parse
 * @param result pointer to write result into if parsing is successful
 * @return true on success, false otherwise
 */
bool bbx_config_parse_bool(const char *value, bool *result);

#endif /* BBX_CONFIG_H */
