/**
 * Copyright 2022 Eugenio Paolantonio (g7)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_BACKENDS_H
#define BBX_BACKENDS_H

#include "lv_conf.h"

#if LV_USE_LINUX_FBDEV == 0 && LV_USE_LINUX_DRM == 0
#error Neither of graphical backends is enabled
#endif

/**
 * Backend identifiers
 *
 * Only BBX_BACKENDS_BACKEND_NONE should have an explicit value assigned
 */
typedef enum {
    BBX_BACKENDS_BACKEND_NONE = -1,
#if LV_USE_LINUX_FBDEV
    BBX_BACKENDS_BACKEND_FBDEV,
#endif
#if LV_USE_LINUX_DRM
    BBX_BACKENDS_BACKEND_DRM,
#endif
} bbx_backends_backend_id_t;

/**
 * Find the first backend with a given name.
 *
 * @param name backend name
 * @return ID of the first matching backend or BBX_BACKENDS_BACKEND_NONE if no backend matched
 */
bbx_backends_backend_id_t bbx_backends_find_backend_with_name(const char *name);

#endif /* BBX_BACKENDS_H */
