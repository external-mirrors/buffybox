/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef BBX_TERMINAL_H
#define BBX_TERMINAL_H

#include <stdbool.h>

/**
 * Prepare the current TTY for graphics output.
 *
 * @param enable_graphics_mode if true, switch terminal into graphics mode (hides command prompt)
 * @param disable_keyboard_input if true, disable terminal keyboard input (hides entered text)
 */
void bbx_terminal_prepare_current_terminal(bool enable_graphics_mode, bool disable_keyboard_input);

/**
 * Reset the current TTY to text output.
 */
void bbx_terminal_reset_current_terminal(void);

#endif /* BBX_TERMINAL_H */
