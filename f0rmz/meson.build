# Copyright 2025 buffybox contributors
# SPDX-License-Identifier: GPL-3.0-or-later

f0rmz_sources = files(
    'command_line.c',
    'config.c',
    'main.c',
    'sq2lv_layouts.c'
)

f0rmz_dependencies = [
    common_dependencies,
    depxkbcommon
]

f0rmz_args = []

if get_option('lvgl_backends').contains('framebuffer')
    f0rmz_args += '-DLV_USE_LINUX_FBDEV=1'
else
    f0rmz_args += '-DLV_USE_LINUX_FBDEV=0'
endif

if get_option('lvgl_backends').contains('drm')
    f0rmz_dependencies += dependency('libdrm')
    f0rmz_args += '-DLV_USE_LINUX_DRM=1'
else
    f0rmz_args += '-DLV_USE_LINUX_DRM=0'
endif

if get_option('lvgl_backends').contains('sdl')
    f0rmz_dependencies += dependency('sdl2')
    f0rmz_args += '-DLV_USE_SDL=1'
else
    f0rmz_args += '-DLV_USE_SDL=0'
endif

executable('f0rmz',
    include_directories: common_include_dirs,
    sources: f0rmz_sources + shared_sources + shared_sources_ul_f0 + squeek2lvgl_sources + lvgl_sources,
    dependencies: f0rmz_dependencies,
    c_args: f0rmz_args,
    install: true,
    install_tag: 'f0rmz'
)
