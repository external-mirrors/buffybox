# Copyright 2021 Clayton Craft
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


# project(
#   'unl0kr',
#   'c',
#   version: '2.0.2',
#   default_options: 'warning_level=3',
#   meson_version: '>=0.53.0'
# )

unl0kr_sources = [
  'backends.c',
  'command_line.c',
  'config.c',
  'cursor.c',
  'font_32.c',
  'indev.c',
  'log.c',
  'main.c',
  'sq2lv_layouts.c',
  'terminal.c',
  'theme.c',
  'themes.c',
]

man_files = [
	'doc/unl0kr.1',
	'doc/unl0kr.conf.5',
]

unl0kr_dependencies = [
  dependency('inih'),
  dependency('libinput'),
  dependency('libudev'),
  dependency('xkbcommon'),
  # Set in parent meson.build
  lvgl,
  lv_drivers,
  squeek2lvgl,
]

# libdrm_dep set in parent meson.build
if libdrm_dep.found()
  unl0kr_dependencies += [libdrm_dep]
endif

install_data(sources: 'unl0kr.conf', install_dir : get_option('sysconfdir'))

unl0kr = executable(
  'unl0kr',
  sources: unl0kr_sources,
  #include_directories: buffybox_includes,
  dependencies: unl0kr_dependencies,
  install: true
)

if get_option('with-docs').enabled()
  scdoc = dependency('scdoc')
  scdoc_prog = find_program(scdoc.get_pkgconfig_variable('scdoc'), native : true)
  sh = find_program('sh', native : true)
  foreach file : man_files
    filename = file + '.scd'
    section = file.split('.')[-1]
    topic = file.split('.' + section)[-2].split('/')[-1]
    output = '@0@.@1@'.format(topic, section)

    custom_target(
      output,
      input : filename,
      output : output,
      capture : true,
      command : [sh, '-c', scdoc_prog.path() + ' < @INPUT@'],
      install : true,
      install_dir : get_option('mandir') / 'man' + section
    )
  endforeach
endif
