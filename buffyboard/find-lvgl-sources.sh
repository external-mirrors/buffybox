#!/bin/sh

# Copyright 2021 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later


find "$1" -name "*.mk" | while IFS= read -r makefile; do
    grep "^CSRCS\s*+=" "$makefile" | sed "s|.*=\s*||g" | while read -r expr; do
        # Ignore example code
        if [[ $(dirname $makefile) =~ .*/examples ]]; then
            continue
        fi

        # Handle full & relative paths
        if echo "$expr" | grep -q '$(LVGL_DIR'; then
            expr=$(echo "$expr" \
                | sed 's|$(LVGL_DIR)/||g' \
                | sed 's|$(LVGL_DIR_NAME)/|lvgl/|g' \
                | sed 's|$(LV_DRIVERS_DIR_NAME)/|lv_drivers/|g')
        else
            expr="$(dirname $makefile)/$expr"
        fi

        # Resolve $(wildcard ...)
        expr=$(echo "$expr" | sed 's|$(wildcard\s*\(.*\))|\1|g')

        # Resolve $(shell ...)
        if echo "$expr" | grep -q '$(shell'; then
            expr=$(echo "$expr" | sed 's|$(shell\s*\(.*\))|\1|g')
            expr=$(eval "$expr")
        fi

        # Resolve wildcards
        for file in $expr; do
            echo $file
        done
    done
done
