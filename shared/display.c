/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "display.h"
#include "log.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

lv_display_t *bbx_display_create(bbx_backends_backend_id_t backend_id, bbx_display_config_t *config) {
    lv_display_t *disp = NULL;

    switch (backend_id) {
#if LV_USE_LINUX_FBDEV
    case BBX_BACKENDS_BACKEND_FBDEV:
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Using framebuffer backend");
        disp = lv_linux_fbdev_create();
        if (!disp) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Failed to create framebuffer display");
            return NULL;
        }

        if (access("/dev/fb0", F_OK) != 0) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "/dev/fb0 is not available");
            return NULL;
        }

        lv_linux_fbdev_set_file(disp, "/dev/fb0");
        if (config && config->fbdev_force_refresh) {
            lv_linux_fbdev_set_force_refresh(disp, true);
        }
        break;
#endif /* LV_USE_LINUX_FBDEV */

#if LV_USE_LINUX_DRM
    case BBX_BACKENDS_BACKEND_DRM:
        bbx_log(BBX_LOG_LEVEL_VERBOSE, "Using DRM backend");
        disp = lv_linux_drm_create();
        if (!disp) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "Failed to create DRM display");
            return NULL;
        }

        char drm_path[16];
        bool found = false;
        for (int i = 0; i < 9; i++) {
            sprintf(drm_path, "/dev/dri/card%d", i);

            if (access(drm_path, F_OK) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            bbx_log(BBX_LOG_LEVEL_ERROR, "/dev/dri/card* are not available");
            return NULL;
        }

        lv_linux_drm_set_file(disp, drm_path, -1);
        break;
#endif /* LV_USE_LINUX_DRM */

    default:
        bbx_log(BBX_LOG_LEVEL_ERROR, "Unable to find suitable backend");
        return NULL;
    }

    lv_display_set_physical_resolution(disp,
        lv_display_get_horizontal_resolution(disp),
        lv_display_get_vertical_resolution(disp));

    /* Apply configuration overrides if provided */
    if (config) {
        /* Set display offset */
        lv_display_set_offset(disp, config->x_offset, config->y_offset);

        /* Override resolution if specified */
        if (config->hor_res > 0 && config->ver_res > 0) {
            lv_display_set_resolution(disp, config->hor_res, config->ver_res);
        }

        /* Override DPI if specified */
        if (config->dpi > 0) {
            lv_display_set_dpi(disp, config->dpi);
        }
    }

    return disp;
}
