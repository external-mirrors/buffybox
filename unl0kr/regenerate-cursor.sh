#!/bin/sh -ex

# Copyright 2023 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later


npm i

./node_modules/lv_img_conv/lv_img_conv.js -f \
    -i ul_cursor_img_dsc \
    -c CF_TRUE_COLOR_ALPHA \
    -o cursor.c \
    cursor.png

# We need to manually replace the old constants because the image converter hasn't
# yet been updated to https://github.com/lvgl/lvgl/issues/4011
sed 's/LV_IMG_CF_TRUE_COLOR_ALPHA/LV_COLOR_FORMAT_NATIVE_WITH_ALPHA/g' cursor.c \
    | sed '/.*LV_IMG_PX_SIZE_ALPHA_BYTE.*/d' \
    | sed '/.*header.always_zero.*/d' \
    | sed '/.*header.reserved.*/d'\
    > cursor.c.tmp
mv cursor.c.tmp cursor.c
