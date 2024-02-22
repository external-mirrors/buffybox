#!/bin/sh

# Copyright 2022 Johannes Marbach, Oliver Smith
# SPDX-License-Identifier: GPL-3.0-or-later


find "$1/src" -name 'lv_init.c'
find "$1/src/core" -name '*.c'
find "$1/src/dev" -name '*.c'
find "$1/src/display" -name '*.c'
find "$1/src/draw" -name '*.c'
find "$1/src/drivers" -name '*.c'
find "$1/src/extra" -name '*.c'
find "$1/src/extra/widgets/keyboard" -name '*.c'
find "$1/src/extra/widgets/msgbox" -name '*.c'
find "$1/src/extra/widgets/span" -name '*.c'
find "$1/src/indev" -name '*.c'
find "$1/src/font" -name '*.c'
find "$1/src/hal" -name '*.c'
find "$1/src/layouts" -name '*.c'
find "$1/src/libs" -name '*.c'
find "$1/src/misc" -name '*.c'
find "$1/src/osal" -name '*.c'
find "$1/src/stdlib" -name '*.c'
find "$1/src/tick" -name '*.c'
find "$1/src/themes" -name '*.c'
find "$1/src/widgets" -name '*.c'
