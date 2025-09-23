/**
 * Copyright 2025 buffybox contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BBX_CLI_COMMON_H
#define BBX_CLI_COMMON_H

#include <stdbool.h>

/**
 * Common CLI options shared across buffybox tools
 */
typedef struct {
    /* Display geometry */
    int hor_res;
    int ver_res;
    int x_offset;
    int y_offset;
    /* DPI override */
    int dpi;
    /* Verbose logging */
    bool verbose;
} bbx_cli_common_opts;

/**
 * Initialize common CLI options with default values.
 *
 * @param opts pointer to the options struct
 */
void bbx_cli_init_common_opts(bbx_cli_common_opts *opts);

/**
 * Parse geometry argument in format "NxM[@X,Y]".
 *
 * @param optarg the argument string
 * @param opts pointer to options struct to populate
 * @return 0 on success, -1 on failure
 */
int bbx_cli_parse_geometry(const char *optarg, bbx_cli_common_opts *opts);

/**
 * Parse DPI argument.
 *
 * @param optarg the argument string
 * @param opts pointer to options struct to populate
 * @return 0 on success, -1 on failure
 */
int bbx_cli_parse_dpi(const char *optarg, bbx_cli_common_opts *opts);

/**
 * Print version information and exit.
 *
 * @param program_name name of the program
 */
void bbx_cli_print_version_and_exit(const char *program_name);

#endif /* BBX_CLI_COMMON_H */
