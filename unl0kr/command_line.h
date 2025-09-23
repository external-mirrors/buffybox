/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef UL_COMMAND_LINE_H
#define UL_COMMAND_LINE_H

#include "../shared/cli_common.h"

#include <stdbool.h>

/**
 * Options parsed from command line arguments
 */
typedef struct {
    /* Common cli options */
    bbx_cli_common_opts common;
    /* Number of config files */
    int num_config_files;
    /* Paths of config file */
    const char **config_files;
    /* Message for a user */
    const char *message;
    bool newline;
} ul_cli_opts;

/**
 * Parse command line arguments and exit on failure.
 * 
 * @param argc number of provided command line arguments
 * @param argv arguments as an array of strings
 * @param opts pointer for writing the parsed options into
 */
void ul_cli_parse_opts(int argc, char *argv[], ul_cli_opts *opts);

#endif /* UL_COMMAND_LINE_H */
