/**
 * Copyright 2021 Johannes Marbach
 *
 * This file is part of buffybox, hereafter referred to as the program.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
