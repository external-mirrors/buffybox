/**
 * Copyright 2022 Eugenio Paolantonio (g7)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "backends.h"

#include "../shared/log.h"

#include <string.h>


/**
 * Public interface
 */

const char *ul_backends_backends[] = {
#if LV_USE_LINUX_FBDEV
    "fbdev",
#endif /* LV_USE_LINUX_FBDEV */
#if LV_USE_LINUX_DRM
    "drm",
#endif /* LV_USE_LINUX_DRM */
    NULL
};

ul_backends_backend_id_t ul_backends_find_backend_with_name(const char *name) {
    for (int i = 0; ul_backends_backends[i] != NULL; ++i) {
        if (strcmp(ul_backends_backends[i], name) == 0) {
            bb_log(BB_LOG_LEVEL_VERBOSE, "Found backend: %s\n", name);
            return i;
        }
    }
    bb_log(BB_LOG_LEVEL_WARNING, "Backend %s not found\n", name);
    return UL_BACKENDS_BACKEND_NONE;
}
