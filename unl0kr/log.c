/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/**
 * Static variables
 */

static ul_log_level log_level = UL_LOG_LEVEL_ERROR;


/**
 * Public functions
 */

void ul_log_set_level(ul_log_level level) {
    log_level = level;
}

void ul_log(ul_log_level level, const char *format, ...) {
    if (level > log_level) {
        return;
    }

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    size_t l = strlen(format);
    if (l > 0 && format[l - 1] != '\n') {
        fprintf(stderr, "\n");
    }
}

void ul_log_print_cb(lv_log_level_t level, const char *msg) {
    LV_UNUSED(level);
    ul_log(UL_LOG_LEVEL_VERBOSE, msg);
}
