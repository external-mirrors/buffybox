#!/bin/bash

rm -rf _build
meson _build -Dwith-drm=disabled
meson compile -C _build
