/**
 * Copyright 2025 Clayton Craft
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "command_line.h"
#include "../shared/log.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void init_opts(f0_cli_opts *opts) {
    bbx_cli_init_common_opts(&opts->common);
    opts->config_file = NULL;
}

static void print_usage() {
    fprintf(stderr,
        "Usage: f0rmz [OPTION]\n"
        "\n"
        "  -C, --config              Path to form configuration file\n"
        "  -g, --geometry=NxM[@X,Y]  Force display size and offset\n"
        "  -d, --dpi=N               Override display DPI\n"
        "  -h, --help                Show this help\n"
        "  -v, --verbose             Enable verbose logging\n"
        "  -V, --version             Show version\n");
}

void f0_cli_parse_opts(int argc, char *argv[], f0_cli_opts *opts) {
    init_opts(opts);

    struct option long_opts[] = {
        { "config",   required_argument, NULL, 'C' },
        { "geometry", required_argument, NULL, 'g' },
        { "dpi",      required_argument, NULL, 'd' },
        { "help",     no_argument,       NULL, 'h' },
        { "verbose",  no_argument,       NULL, 'v' },
        { "version",  no_argument,       NULL, 'V' },
        { NULL, 0, NULL, 0 }
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "C:g:d:hvV", long_opts, NULL)) != -1) {
        switch (opt) {
        case 'C':
            opts->config_file = optarg;
            break;
        case 'g':
            if (bbx_cli_parse_geometry(optarg, &opts->common) != 0) {
                exit(EXIT_FAILURE);
            }
            break;
        case 'd':
            if (bbx_cli_parse_dpi(optarg, &opts->common) != 0) {
                exit(EXIT_FAILURE);
            }
            break;
        case 'h':
            print_usage();
            exit(EXIT_SUCCESS);
        case 'v':
            opts->common.verbose = true;
            break;
        case 'V':
            bbx_cli_print_version_and_exit("f0rmz");
            break;
        default:
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}
