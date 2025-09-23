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

const char *bbx_backends_backends[] = {
#if LV_USE_LINUX_FBDEV
    "fbdev",
#endif /* LV_USE_LINUX_FBDEV */
#if LV_USE_LINUX_DRM
    "drm",
#endif /* LV_USE_LINUX_DRM */
    NULL
};

bbx_backends_backend_id_t bbx_backends_find_backend_with_name(const char *name) {
    for (int i = 0; bbx_backends_backends[i] != NULL; ++i) {
        if (strcmp(bbx_backends_backends[i], name) == 0) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Found backend: %s\n", name);
            return i;
        }
    }
    bbx_log(BBX_LOG_LEVEL_WARNING, "Backend %s not found\n", name);
    return BBX_BACKENDS_BACKEND_NONE;
}
