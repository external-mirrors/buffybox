/**
 * Copyright 2022 Eugenio Paolantonio (g7)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "backends.h"

#include "log.h"

#include <string.h>

static const char *backends[] = {
#if LV_USE_LINUX_FBDEV
    "fbdev",
#endif
#if LV_USE_LINUX_DRM
    "drm",
#endif
    NULL
};

bbx_backends_backend_id_t bbx_backends_find_backend_with_name(const char *name) {
    for (int i = 0; backends[i] != NULL; ++i) {
        if (strcmp(backends[i], name) == 0) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Found backend: %s\n", name);
            return i;
        }
    }
    bbx_log(BBX_LOG_LEVEL_WARNING, "Backend %s not found\n", name);
    return BBX_BACKENDS_BACKEND_NONE;
}
