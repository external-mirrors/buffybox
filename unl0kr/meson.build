# Copyright 2021 Clayton Craft
# SPDX-License-Identifier: GPL-3.0-or-later

unl0kr_sources = files(
    'command_line.c',
    'config.c',
    'main.c',
    'sq2lv_layouts.c',
)

unl0kr_dependencies = [
    common_dependencies,
    depxkbcommon
]

unl0kr_args = []

if get_option('lvgl_backends').contains('framebuffer')
    unl0kr_args += '-DLV_USE_LINUX_FBDEV=1'
else
    unl0kr_args += '-DLV_USE_LINUX_FBDEV=0'
endif

if get_option('lvgl_backends').contains('drm')
    unl0kr_dependencies += dependency('libdrm')
    unl0kr_args += '-DLV_USE_LINUX_DRM=1'
else
    unl0kr_args += '-DLV_USE_LINUX_DRM=0'
endif

if get_option('lvgl_backends').contains('sdl')
    unl0kr_dependencies += dependency('sdl2')
    unl0kr_args += '-DLV_USE_SDL=1'
else
    unl0kr_args += '-DLV_USE_SDL=0'
endif

executable('unl0kr',
    include_directories: common_include_dirs,
    sources: unl0kr_sources + shared_sources + shared_sources_ul_f0 + squeek2lvgl_sources + lvgl_sources,
    dependencies: unl0kr_dependencies,
    c_args: unl0kr_args,
    install: true,
    install_tag: 'unl0kr'
)

install_data('unl0kr.conf',
    install_dir: get_option('sysconfdir'),
    install_tag: 'unl0kr'
)

if get_option('systemd')
    executable('unl0kr-agent',
        sources: files('unl0kr-agent.c'),
        dependencies: depinih,
        c_args: '-DUNL0KR_BINARY="@0@"'.format(get_option('prefix') / get_option('bindir') / 'unl0kr'),
        install: true,
        install_dir: get_option('libexecdir'),
        install_tag: 'unl0kr'
    )

    install_data('unl0kr-agent.path',
        install_dir: systemd_system_unit_dir,
        install_tag: 'unl0kr'
    )

    configure_file(
        configuration: {'LIBEXECDIR': get_option('prefix') / get_option('libexecdir')},
        input: 'unl0kr-agent.service.in',
        output: 'unl0kr-agent.service',
        install: true,
        install_dir: systemd_system_unit_dir,
        install_tag: 'unl0kr'
    )
endif
