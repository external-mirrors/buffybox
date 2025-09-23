/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "command_line.h"

#include "unl0kr.h"

#include "../shared/log.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Static prototypes
 */

/**
 * Initialise a command line options struct with default values and exit on failure.
 * 
 * @param opts pointer to the options struct
 */
static void init_opts(ul_cli_opts *opts);

/**
 * Output usage instructions.
 */
static void print_usage();


/**
 * Static functions
 */

static void init_opts(ul_cli_opts *opts) {
    bbx_cli_init_common_opts(&opts->common);
    opts->num_config_files = 0;
    opts->config_files = NULL;
    opts->message = NULL;
    opts->newline = true;
}

static void print_usage() {
    fprintf(stderr,
        /*-------------------------------- 78 CHARS --------------------------------*/
        "Usage: unl0kr [OPTION]\n"
        "\n"
        "Unl0kr values the CRYPTTAB_TRIED variable. Upon completion, the entered\n"
        "password is printed to STDOUT. All other output happens on STDERR.\n"
        "\n"
        "Mandatory arguments to long options are mandatory for short options too.\n"
        "  -m, --message             A message that will be displayed to a user\n"
        "  -C, --config-override     Path to a config override file. Can be supplied\n"
        "                            multiple times. Config files are merged in the\n"
        "                            following order:\n"
        "                            * /usr/share/unl0kr/unl0kr.conf\n"
        "                            * /usr/share/unl0kr/unl0kr.conf.d/* (alphabetically)\n"
        "                            * /etc/unl0kr.conf\n"
        "                            * /etc/unl0kr.conf.d/* (alphabetically)\n"
        "                            * Override files (in supplied order)\n"
        "  -g, --geometry=NxM[@X,Y]  Force a display size of N horizontal times M\n"
        "                            vertical pixels, offset horizontally by X\n"
        "                            pixels and vertically by Y pixels\n"
        "  -d  --dpi=N               Override the display's DPI value\n"
        "  -h, --help                Print this message and exit\n"
        "  -n                        Do not append a newline character to a password\n"
        "  -v, --verbose             Enable more detailed logging output on STDERR\n"
        "  -V, --version             Print the unl0kr version and exit\n");
        /*-------------------------------- 78 CHARS --------------------------------*/
}


/**
 * Public functions
 */

void ul_cli_parse_opts(int argc, char *argv[], ul_cli_opts *opts) {
    init_opts(opts);

    struct option long_opts[] = {
        { "message",         required_argument, NULL, 'm' },
        { "config-override", required_argument, NULL, 'C' },
        { "geometry",        required_argument, NULL, 'g' },
        { "dpi",             required_argument, NULL, 'd' },
        { "help",            no_argument,       NULL, 'h' },
        { "verbose",         no_argument,       NULL, 'v' },
        { "version",         no_argument,       NULL, 'V' },
        { NULL, 0, NULL, 0 }
    };

    int opt, index = 0;

    while ((opt = getopt_long(argc, argv, "m:C:g:d:hnvV", long_opts, &index)) != -1) {
        switch (opt) {
        case 'm':
            opts->message = strdup(optarg);
            break;
        case 'C':
            opts->config_files = realloc(opts->config_files, (opts->num_config_files + 1) * sizeof(char *));
            if (!opts->config_files) {
                bbx_log(BBX_LOG_LEVEL_ERROR, "Could not allocate memory for config file paths");
                exit(EXIT_FAILURE);
            }
            opts->config_files[opts->num_config_files] = optarg;
            opts->num_config_files++;
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
        case 'n':
            opts->newline = false;
            break;
        case 'v':
            opts->common.verbose = true;
            break;
        case 'V':
            bbx_cli_print_version_and_exit("unl0kr");
            break;
        default:
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}
