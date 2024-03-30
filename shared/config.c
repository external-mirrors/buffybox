/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "config.h"

#include "log.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>


/**
 * Static prototypes
 */

/**
 * Checks whether a string ends with a suffix
 *
 * @param string string to check
 * @param suffix suffix to compare to
 * @return true if the suffix matches at the end of the string, false otherwise
 */
static bool string_ends_with(const char *string, const char *suffix);


/**
 * Static functions
 */

static bool string_ends_with(const char *string, const char *suffix) {
    if (!string || !suffix || strlen(suffix) > strlen(string)) {
        return false;
    }
    return strncmp(string + strlen(string) - strlen(suffix), suffix, strlen(suffix)) == 0;
}


/**
 * Public functions
 */

int bbx_config_compare_strings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void bbx_config_find_files(const char *path, char ***found, int *num_found) {
    /* Initialise output variables */
    *found = NULL;
    *num_found = 0;

    /* Count length of directory path */
    const int path_length = strlen(path);

    /* Open directory */
    DIR *d = opendir(path);
    if (!d) {
        bbx_log(BBX_LOG_LEVEL_WARNING, "Could not read contents of folder %s", path);
        return;
    }

    /* Loop over directory contents */
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        /* Ignore anything except for .conf files */
        if (dir->d_type != DT_REG || !string_ends_with(dir->d_name, ".conf")) {
            continue;
        }

        /* Grow output array */
        char **tmp = realloc(*found, (*num_found + 1) * sizeof(char *));
        if (!tmp) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Could not reallocate memory for configuration file paths");
            break;
        }
        *found = tmp;

        /* Extract file name and length */
        char *name = dir->d_name;
        int name_length = strlen(name);

        /* Allocate memory for full path */
        char *found_path = malloc(path_length + name_length + 2); /* +1 for path separator and null terminator, respectively */
        if (!found_path) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Could not allocate memory for configuration file path");
            break;
        }

        /* Build full path */
        memcpy(found_path, path, path_length);
        found_path[path_length] = '/';
        memcpy(found_path + path_length + 1, dir->d_name, name_length + 1); /* +1 for path separator and null terminator, respectively */

        /* Store file path */
        (*found)[*num_found] = found_path;
        *num_found += 1;
    }

    /* Close directory */
    closedir(d);
}

bool bbx_config_parse_bool(const char *value, bool *result) {
    if (strcmp(value, "true") == 0) {
        *result = true;
        return true;
    }

    if (strcmp(value, "false") == 0) {
        *result = false;
        return true;
    }

    return false;
}
