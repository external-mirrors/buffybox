/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "cli_common.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "?" /* Real version injected by meson during build */
#endif

void bbx_cli_init_common_opts(bbx_cli_common_opts *opts) {
    opts->hor_res = -1;
    opts->ver_res = -1;
    opts->x_offset = 0;
    opts->y_offset = 0;
    opts->dpi = 0;
    opts->verbose = false;
}

int bbx_cli_parse_geometry(const char *optarg, bbx_cli_common_opts *opts) {
    /* Try parsing with offset first: NxM@X,Y */
    if (sscanf(optarg, "%ix%i@%i,%i", &(opts->hor_res), &(opts->ver_res),
               &(opts->x_offset), &(opts->y_offset)) == 4) {
        return 0;
    }

    /* Try parsing without offset: NxM */
    if (sscanf(optarg, "%ix%i", &(opts->hor_res), &(opts->ver_res)) == 2) {
        return 0;
    }

    bbx_log(BBX_LOG_LEVEL_ERROR, "Invalid geometry argument \"%s\"", optarg);
    return -1;
}

int bbx_cli_parse_dpi(const char *optarg, bbx_cli_common_opts *opts) {
    if (sscanf(optarg, "%i", &(opts->dpi)) != 1) {
        bbx_log(BBX_LOG_LEVEL_ERROR, "Invalid dpi argument \"%s\"", optarg);
        return -1;
    }
    return 0;
}

void bbx_cli_print_version_and_exit(const char *program_name) {
    fprintf(stderr, "%s %s\n", program_name, PROJECT_VERSION);
    exit(0);
}
