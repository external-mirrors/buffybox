/**
 * Copyright 2022 Eugenio Paolantonio (g7)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef UL_BACKENDS_H
#define UL_BACKENDS_H

#include "lv_drv_conf.h"

#include "log.h"

/* NOTE: Only UL_BACKENDS_BACKEND_NONE is ought to have an explicit value assigned */
typedef enum {
    UL_BACKENDS_BACKEND_NONE = -1,
#if USE_FBDEV
    UL_BACKENDS_BACKEND_FBDEV,
#endif /* USE_FBDEV */
#if USE_DRM
    UL_BACKENDS_BACKEND_DRM,
#endif /* USE_DRM */
} ul_backends_backend_id_t;

/* Backends */
extern const char *ul_backends_backends[];

/**
 * Find the first backend with a given name.
 *
 * @param name backend name
 * @return ID of the first matching backend or UL_BACKENDS_BACKEND_NONE if no backend matched
 */
ul_backends_backend_id_t ul_backends_find_backend_with_name(const char *name);

#endif /* UL_BACKENDS_H */
