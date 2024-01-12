/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BB_COMMAND_LINE_H
#define BB_COMMAND_LINE_H

#include "lvgl/lvgl.h"

/**
 * Options parsed from command line arguments
 */
typedef struct {
    /* Display rotation */
    lv_disp_rot_t rotation;
} bb_cli_opts;

/**
 * Parse command line arguments and exit on failure.
 *
 * @param argc number of provided command line arguments
 * @param argv arguments as an array of strings
 * @param opts pointer for writing the parsed options into
 */
void bb_cli_parse_opts(int argc, char *argv[], bb_cli_opts *opts);

#endif /* BB_COMMAND_LINE_H */
