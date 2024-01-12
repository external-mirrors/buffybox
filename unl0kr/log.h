/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef UL_LOG_H
#define UL_LOG_H

/**
 * Log levels
 */
typedef enum {
    /* Errors only */
    UL_LOG_LEVEL_ERROR   = 0,
    /* Warnings and errors */
    UL_LOG_LEVEL_WARNING = 1,
    /* Include non-errors in log */
    UL_LOG_LEVEL_VERBOSE = 2
} ul_log_level;

/**
 * Set the log level.
 * 
 * @param level new log level value
 */
void ul_log_set_level(ul_log_level level);

/**
 * Log a message. A newline character is appended unless the message ends in one.
 * 
 * @param level log level of the message
 * @param format message format string
 * @param ... parameters to fill into the format string
 */
void ul_log(ul_log_level level, const char *format, ...);

/**
 * Handle an LVGL log message.
 * 
 * @param msg message to print
 */
void ul_log_print_cb(const char *msg);

#endif /* UL_LOG_H */
