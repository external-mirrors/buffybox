# Copyright 2021 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later


buffyboard_sources = [
  'command_line.c',
  'config.c',
  'main.c',
  'sq2lv_layouts.c',
  'terminal.c',
  'uinput_device.c',
]

shared_sources = [
  '../shared/cursor/cursor.c',
  '../shared/fonts/font_32.c',
  '../shared/config.c',
  '../shared/indev.c',
  '../shared/log.c',
  '../shared/theme.c',
  '../shared/themes.c',
]

squeek2lvgl_sources = [
  '../squeek2lvgl/sq2lv.c',
]

man_files = [
	'doc/buffyboard.1',
	'doc/buffyboard.conf.5',
]

lvgl_sources = run_command('../find-lvgl-sources.sh', '../lvgl', check: true).stdout().strip().split('\n')

executable(
  'buffyboard',
  sources: buffyboard_sources + shared_sources + squeek2lvgl_sources + lvgl_sources,
  include_directories: ['..'],
  dependencies: [
    dependency('inih'),
    dependency('libinput'),
    dependency('libudev'),
    meson.get_compiler('c').find_library('m', required: false),
  ],
  install: true
)

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
