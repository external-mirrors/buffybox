#!/bin/bash

rm -rf _build
meson setup _build -Dlvgl_backends=framebuffer,drm -Dsystemd=true
meson compile -C _build
