#!/bin/bash

# Copyright 2021 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later


cd ../squeek2lvgl
pipenv install
pipenv run python squeek2lvgl.py \
    --input us.yaml \
      --name "US English" \
    --input de.yaml \
      --name "Deutsch" \
    --input es.yaml \
      --name "Español" \
    --input fr.yaml \
      --name "Français" \
    --extra-top-row-base "1 2 3 4 5 6 7 8 9 0" \
    --extra-top-row-upper "! @ # $ % ^ & * ( )" \
    --output ../unl0kr \
    --surround-space-with-arrows \
    --shift-keycap '\xef\x8d\x9b'
