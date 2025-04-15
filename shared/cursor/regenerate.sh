#!/bin/sh -ex

# Copyright 2023 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later

# Dependencies:
#   python-pypng
#   python-lz4

root=$(git rev-parse --show-toplevel)

"$root/lvgl/scripts/LVGLImage.py" \
    --ofmt C \
    --cf "ARGB8888" \
    --output "$root/shared/cursor" \
    --verbose \
    "$root/shared/cursor/cursor.png"
