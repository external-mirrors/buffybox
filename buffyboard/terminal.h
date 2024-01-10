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


#ifndef BB_TERMINAL_H
#define BB_TERMINAL_H

#include <stdbool.h>

/**
 * Prepare for resizing terminals by opening the current one.
 * 
 * @param factor factor (between 0 and 1) by which to adapt terminal sizes
 * @return true if the operation was successful, false otherwise. No other bb_terminal_* functions
 * must be called if false is returned.
 */
bool bb_terminal_init(float factor);

/**
 * Shrink the height of the active terminal by the current factor.
 */
void bb_terminal_shrink_current(void);

/**
 * Re-maximise the height of all previously resized terminals.
 */
void bb_terminal_reset_all(void);

#endif /* BB_TERMINAL_H */
