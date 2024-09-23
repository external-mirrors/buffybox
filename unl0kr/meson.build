# Copyright 2021 Clayton Craft
# SPDX-License-Identifier: GPL-3.0-or-later


unl0kr_sources = [
  'backends.c',
  'command_line.c',
  'config.c',
  'main.c',
  'sq2lv_layouts.c',
  'terminal.c',
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
	'doc/unl0kr.1',
	'doc/unl0kr.conf.5',
]

inih_dep = dependency('inih')

unl0kr_dependencies = [
  inih_dep,
  dependency('libinput'),
  dependency('libudev'),
  dependency('xkbcommon'),
]

libdrm_dep = dependency('libdrm', required: get_option('with-drm'))
if libdrm_dep.found()
  unl0kr_dependencies += [libdrm_dep]
  add_project_arguments('-DLV_USE_LINUX_DRM=1', language: ['c'])
endif

lvgl_sources = run_command('../find-lvgl-sources.sh', '../lvgl', check: true).stdout().strip().split('\n')

install_data(sources: 'unl0kr.conf', install_dir : get_option('sysconfdir'))

executable(
  'unl0kr',
  sources: unl0kr_sources + shared_sources + squeek2lvgl_sources + lvgl_sources,
  include_directories: ['..'],
  dependencies: unl0kr_dependencies,
  install: true
)

systemd_dep = dependency('systemd', required: get_option('systemd-password-agent'))
if systemd_dep.found()
  executable(
    'unl0kr-agent',
    sources: files('unl0kr-agent.c'),
    dependencies: inih_dep,
    c_args: '-DUNL0KR_BINARY=' + get_option('prefix') / get_option('bindir') / 'unl0kr',
    install: true,
    install_dir: get_option('libexecdir')
  )

  system_unit_dir = systemd_dep.get_variable('systemd_system_unit_dir')

  install_data(
    'unl0kr-agent.path',
    install_dir: system_unit_dir
  )

  configure_file(
    configuration: {'LIBEXECDIR': get_option('libexecdir')},
    input: 'unl0kr-agent.service.in',
    output: 'unl0kr-agent.service',
    install: true,
    install_dir: system_unit_dir
  )
endif

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
