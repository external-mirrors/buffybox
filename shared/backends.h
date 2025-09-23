/**
 * Copyright 2022 Eugenio Paolantonio (g7)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_BACKENDS_H
#define BBX_BACKENDS_H

#include "lvgl/lvgl.h"

/**
 * Backend identifiers
 *
 * Only BBX_BACKENDS_BACKEND_NONE should have an explicit value assigned
 */
typedef enum {
    BBX_BACKENDS_BACKEND_NONE = -1,
#if LV_USE_LINUX_FBDEV
    BBX_BACKENDS_BACKEND_FBDEV,
#endif /* LV_USE_LINUX_FBDEV */
#if LV_USE_LINUX_DRM
    BBX_BACKENDS_BACKEND_DRM,
#endif /* LV_USE_LINUX_DRM */
} bbx_backends_backend_id_t;

/**
 * Actual backends
 */
extern const char *bbx_backends_backends[];

/**
 * Find the first backend with a given name.
 *
 * @param name backend name
 * @return ID of the first matching backend or BBX_BACKENDS_BACKEND_NONE if no backend matched
 */
bbx_backends_backend_id_t bbx_backends_find_backend_with_name(const char *name);

#endif /* BBX_BACKENDS_H */
