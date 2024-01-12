/**
 * Copyright 2022 Eugenio Paolantonio (g7)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "backends.h"

#include <string.h>

/**
 * Public interface
 */

const char *ul_backends_backends[] = {
#if USE_FBDEV
    "fbdev",
#endif /* USE_FBDEV */
#if USE_DRM
    "drm",
#endif /* USE_DRM */
    NULL
};

ul_backends_backend_id_t ul_backends_find_backend_with_name(const char *name) {
    for (int i = 0; ul_backends_backends[i] != NULL; ++i) {
        if (strcmp(ul_backends_backends[i], name) == 0) {
            ul_log(UL_LOG_LEVEL_VERBOSE, "Found backend: %s\n", name);
            return i;
        }
    }
    ul_log(UL_LOG_LEVEL_WARNING, "Backend %s not found\n", name);
    return UL_BACKENDS_BACKEND_NONE;
}
