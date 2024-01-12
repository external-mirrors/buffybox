/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "command_line.h"

#include "buffyboard.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * Static prototypes
 */

/**
 * Initialise a command line options struct with default values and exit on failure.
 *
 * @param opts pointer to the options struct
 */
static void init_opts(bb_cli_opts *opts);

/**
 * Output usage instructions.
 */
static void print_usage();


/**
 * Static functions
 */

static void init_opts(bb_cli_opts *opts) {
    opts->rotation = LV_DISP_ROT_NONE;
}

static void print_usage() {
    fprintf(stderr,
        /*-------------------------------- 78 CHARS --------------------------------*/
        "Usage: buffyboard [OPTION]\n"
        "\n"
        "Mandatory arguments to long options are mandatory for short options too.\n"
        "  -r, --rotate=[0-3]  Rotate the UI to the given orientation.\n"
        "                        0 - normal orientation (0 degree)\n"
        "                        1 - clockwise orientation (90 degrees)\n"
        "                        2 - upside down orientation (180 degrees)\n"
        "                        3 - counterclockwise orientation (270 degrees)\n"
        "                      The values match the ones provided by the kernel in\n"
        "                      /sys/class/graphics/fbcon/rotate.\n"
        "  -h, --help          Print this message and exit\n"
        "  -V, --version       Print the buffyboard version and exit\n");
        /*-------------------------------- 78 CHARS --------------------------------*/
}


/**
 * Public functions
 */

void bb_cli_parse_opts(int argc, char *argv[], bb_cli_opts *opts) {
    init_opts(opts);

    struct option long_opts[] = {
        { "rotate",  required_argument, NULL, 'r' },
        { "help",    no_argument,       NULL, 'h' },
        { "version", no_argument,       NULL, 'V' },
        { NULL, 0, NULL, 0 }
    };

    int opt, index = 0;

    while ((opt = getopt_long(argc, argv, "r:hV", long_opts, &index)) != -1) {
        switch (opt) {
        case 'r': {
            int orientation;
            if (sscanf(optarg, "%i", &orientation) != 1 || orientation < 0 || orientation > 3) {
                fprintf(stderr, "Invalid orientation argument \"%s\"\n", optarg);
                exit(EXIT_FAILURE);
            }
            switch (orientation) {
                case 0:
                    opts->rotation = LV_DISP_ROT_NONE;
                    break;
                case 1:
                    opts->rotation = LV_DISP_ROT_270;
                    break;
                case 2:
                    opts->rotation = LV_DISP_ROT_180;
                    break;
                case 3:
                    opts->rotation = LV_DISP_ROT_90;
                    break;
            }
            break;
        }
        case 'h':
            print_usage();
            exit(EXIT_SUCCESS);
        case 'V':
            fprintf(stderr, "buffyboard %s\n", BB_VERSION);
            exit(0);
        default:
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}
