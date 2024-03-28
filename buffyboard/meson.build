# Copyright 2021 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later


project(
  'buffyboard',
  'c',
  version: '3.0.0',
  default_options: 'warning_level=1',
  meson_version: '>=0.53.0'
)

add_project_arguments('-DBB_VERSION="@0@"'.format(meson.project_version()), language: ['c'])

buffyboard_sources = [
  'command_line.c',
  'font_32.c',
  'indev.c',
  'main.c',
  'sq2lv_layouts.c',
  'terminal.c',
  'uinput_device.c',
]

shared_sources = [
  '../shared/cursor/cursor.c'
]

squeek2lvgl_sources = [
  '../squeek2lvgl/sq2lv.c',
]

lvgl_sources = run_command('../find-lvgl-sources.sh', '../lvgl', check: true).stdout().strip().split('\n')

executable(
  'buffyboard',
  sources: buffyboard_sources + shared_sources + squeek2lvgl_sources + lvgl_sources,
  include_directories: ['..'],
  dependencies: [
    dependency('libinput'),
    meson.get_compiler('c').find_library('m', required: false),
  ],
  install: true
)
