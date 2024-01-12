#!/bin/sh

# Copyright 2022 Johannes Marbach, Oliver Smith
# SPDX-License-Identifier: GPL-3.0-or-later


find lv_drivers -name '*.c'
find lvgl/src/core -name '*.c'
find lvgl/src/draw -name '*.c'
find lvgl/src/extra -name '*.c'
find lvgl/src/extra/layouts/flex -name '*.c'
find lvgl/src/extra/widgets/keyboard -name '*.c'
find lvgl/src/extra/widgets/msgbox -name '*.c'
find lvgl/src/extra/widgets/span -name '*.c'
find lvgl/src/font -name '*.c'
find lvgl/src/hal -name '*.c'
find lvgl/src/misc -name '*.c'
find lvgl/src/widgets -name '*.c'
