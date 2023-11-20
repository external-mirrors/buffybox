# Copyright 2021 Johannes Marbach
#
# This file is part of buffyboard, hereafter referred to as the program.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.


project(
  'buffyboard',
  'c',
  version: '0.2.0',
  default_options: 'warning_level=1',
  meson_version: '>=0.53.0'
)

add_project_arguments('-DBB_VERSION="@0@"'.format(meson.project_version()), language: ['c'])

buffyboard_sources = [
  'command_line.c',
  'cursor.c',
  'font_32.c',
  'indev.c',
  'main.c',
  'sq2lv_layouts.c',
  'terminal.c',
  'uinput_device.c',
]

squeek2lvgl_sources = [
  '../squeek2lvgl/sq2lv.c',
]

lvgl_sources = run_command('find-lvgl-sources.sh', 'lvgl').stdout().strip().split('\n')

lv_drivers_sources = run_command('find-lvgl-sources.sh', 'lv_drivers').stdout().strip().split('\n')

executable(
  'buffyboard',
  sources: buffyboard_sources + squeek2lvgl_sources + lvgl_sources + lv_drivers_sources,
  include_directories: ['lvgl', 'lv_drivers'],
  dependencies: [
    dependency('libinput'),
    meson.get_compiler('c').find_library('m', required: false),
    dependency('xkbcommon'),
  ],
  install: true
)
