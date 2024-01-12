# Copyright 2024 Caleb Connolly
#
# This file is part of buffybox, hereafter referred to as the program.
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
  'buffybox',
  'c',
  version: '2.1.0',
  default_options: 'warning_level=3',
  meson_version: '>=0.63.0'
)

cmake = import('cmake')

# Common things (mostly unl0kr?)

add_project_arguments('-DBUFFYBOX_VERSION="@0@"'.format(meson.project_version()), language: ['c'])

libdrm_dep = dependency('libdrm', required: get_option('with-drm'))
if libdrm_dep.found()
  add_project_arguments('-DUSE_DRM=1', language: ['c'])
endif

# Used for lv_drivers to find lv_drv_conf.h, we could probably get rid of the need for the header
# but even better is to drop lv_drivers entirely and update to newer lvgl
# ALSO used for lv_conf.h in LVGL...
HACK_unl0kr_dir = '@0@/unl0kr'.format(meson.current_source_dir())

# LVGL

lvgl_opts = cmake.subproject_options()

# Fun hacks to correctly include lv_conf.h
lvgl_opts.append_compile_args('c', '-DLV_CONF_PATH=@0@/lv_conf.h'.format(HACK_unl0kr_dir))

# LVGL library
lvgl_proj = cmake.subproject('lvgl', options: lvgl_opts)
lvgl = lvgl_proj.dependency('lvgl')

# lv_drivers depends on LVGL
subdir('lv_drivers')

# subdirs expect "lvgl" and "lv_drivers" variables to be the respective libraries.

subdir('squeek2lvgl')
subdir('unl0kr')
