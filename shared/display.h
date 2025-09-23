/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BBX_DISPLAY_H
#define BBX_DISPLAY_H

#include "backends.h"
#include "lvgl/lvgl.h"

/**
 * Display configuration options
 */
typedef struct {
    /* Display resolution override */
    int hor_res;
    int ver_res;
    /* Display offset */
    int x_offset;
    int y_offset;
    /* DPI override */
    int dpi;
    /* Framebuffer force refresh quirk */
    bool fbdev_force_refresh;
} bbx_display_config_t;

/**
 * Initialize LVGL display with the specified backend and configuration.
 *
 * @param backend_id the backend to use
 * @param config display configuration options
 * @return initialized display or NULL on failure
 */
lv_display_t *bbx_display_create(bbx_backends_backend_id_t backend_id, bbx_display_config_t *config);

#endif /* BBX_DISPLAY_H */
