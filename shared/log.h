/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_LOG_H
#define BBX_LOG_H

#include "lvgl/lvgl.h"

/**
 * Log levels
 */
typedef enum {
    /* Errors only */
    BBX_LOG_LEVEL_ERROR   = 0,
    /* Warnings and errors */
    BBX_LOG_LEVEL_WARNING = 1,
    /* Include non-errors in log */
    BBX_LOG_LEVEL_VERBOSE = 2
} bbx_log_level;

/**
 * Set the log level.
 * 
 * @param level new log level value
 */
void bbx_log_set_level(bbx_log_level level);

/**
 * Log a message. A newline character is appended unless the message ends in one.
 * 
 * @param level log level of the message
 * @param format message format string
 * @param ... parameters to fill into the format string
 */
void bbx_log(bbx_log_level level, const char *format, ...);

/**
 * Handle an LVGL log message.
 *
 * @param level LVGL log level
 * @param msg message to print
 */
void bbx_log_print_cb(lv_log_level_t level, const char *msg);

#endif /* BBX_LOG_H */
