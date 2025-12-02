/**
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef BBX_FORCE_FEEDBACK_H
#define BBX_FORCE_FEEDBACK_H

#include <libudev.h>

/**
 * Try to connect a force feedback device.
 */
void bbx_force_feedback_connect(struct udev_device* device);

/**
 * Play a force feedback effect.
 */
void bbx_force_feedback_play();

#endif /* BBX_FORCE_FEEDBACK_H */
