#!/bin/bash

rm -rf _build
meson setup _build -Dlvgl_backends=framebuffer
meson compile -C _build
