#!/bin/bash

# Copyright 2021 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later


pipenv install

pipenv run python squeek2lvgl.py \
    --input us.yaml \
    --output examples/us \
    --surround-space-with-arrows

pipenv run python squeek2lvgl.py \
    --input de.yaml \
    --output examples/de-with-custom-shift-keycap \
    --surround-space-with-arrows \
    --shift-keycap ABC

pipenv run python squeek2lvgl.py \
    --input es.yaml \
    --input fr.yaml \
    --input us.yaml \
    --output examples/es+fr+us \
    --surround-space-with-arrows

pipenv run python squeek2lvgl.py \
    --input terminal/us.yaml \
    --output examples/us-terminal-with-scancodes \
    --generate-scancodes
