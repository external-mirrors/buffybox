# Copyright 2021 Johannes Marbach
# SPDX-License-Identifier: GPL-3.0-or-later

buffyboard_sources = files(
    'command_line.c',
    'config.c',
    'main.c',
    'sq2lv_layouts.c',
    'terminal.c',
    'uinput_device.c'
)

buffyboard_dependencies = [
    common_dependencies,
    meson.get_compiler('c').find_library('m', required: false)
]

executable('buffyboard',
    include_directories: common_include_dirs,
    sources: buffyboard_sources + shared_sources + squeek2lvgl_sources + lvgl_sources,
    dependencies: buffyboard_dependencies,
    install: true
)

install_data('buffyboard.conf', install_dir: get_option('sysconfdir'))

systemd = dependency('systemd', required: get_option('systemd-buffyboard-service'))
if systemd.found()
    system_unit_dir = systemd.get_variable(
        pkgconfig: 'systemd_system_unit_dir',
        pkgconfig_define: ['prefix', get_option('prefix')],
    )

    configure_file(
      input : 'buffyboard.service.in',
      output : 'buffyboard.service',
      install : true,
      install_dir : system_unit_dir,
      configuration : {
        'bindir' : get_option('prefix') / get_option('bindir'),
      },
    )
endif
