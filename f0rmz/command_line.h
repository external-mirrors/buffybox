/**
 * Copyright 2025 Clayton Craft
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef F0_COMMAND_LINE_H
#define F0_COMMAND_LINE_H

#include "../shared/cli_common.h"

typedef struct {
    bbx_cli_common_opts common;
    const char *config_file;
} f0_cli_opts;

void f0_cli_parse_opts(int argc, char *argv[], f0_cli_opts *opts);

#endif /* F0_COMMAND_LINE_H */
