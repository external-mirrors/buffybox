#!/bin/bash

rm -rf _build
meson _build
meson compile -C _build
