#!/bin/sh

# Copyright 2022 Johannes Marbach, Oliver Smith
# SPDX-License-Identifier: GPL-3.0-or-later


find "$1/src" -name 'lv_init.c' | LC_ALL=C sort
find "$1/src/core" -name '*.c' | LC_ALL=C sort
find "$1/src/display" -name '*.c' | LC_ALL=C sort
find "$1/src/draw" -name '*.c' | LC_ALL=C sort
find "$1/src/drivers" -name '*.c' | LC_ALL=C sort
find "$1/src/indev" -name '*.c' | LC_ALL=C sort
find "$1/src/font" -name '*.c' | LC_ALL=C sort
find "$1/src/layouts" -name '*.c' | LC_ALL=C sort
find "$1/src/libs" -name '*.c' | LC_ALL=C sort
find "$1/src/misc" -name '*.c' | LC_ALL=C sort
find "$1/src/osal" -name '*.c' | LC_ALL=C sort
find "$1/src/stdlib" -name '*.c' | LC_ALL=C sort
find "$1/src/tick" -name '*.c' | LC_ALL=C sort
find "$1/src/themes" -name '*.c' | LC_ALL=C sort
find "$1/src/widgets" -name '*.c' | LC_ALL=C sort
