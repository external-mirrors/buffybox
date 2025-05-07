project('buffybox', 'c',
    version: '3.3.0',
    default_options: ['warning_level=3', 'b_ndebug=if-release'],
    meson_version: '>= 0.60.0'
)

add_project_arguments(
    '-DPROJECT_VERSION="@0@"'.format(meson.project_version()),
    '-DLV_BIG_ENDIAN_SYSTEM=' + (host_machine.endian() == 'big'? '1' : '0'),
    language: 'c'
)

depinih      = dependency('inih')
deplibinput  = dependency('libinput')
deplibudev   = dependency('libudev')
depxkbcommon = dependency('xkbcommon') # For unl0kr only

if get_option('man')
    depscdoc = dependency('scdoc', native: true)
endif

common_include_dirs = include_directories('.')

shared_sources = files(
    'shared/cursor/cursor.c',
    'shared/fonts/font_32.c',
    'shared/config.c',
    'shared/indev.c',
    'shared/log.c',
    'shared/theme.c',
    'shared/themes.c'
)

squeek2lvgl_sources = files(
    'squeek2lvgl/sq2lv.c'
)

lvgl_sources = files(
    run_command('find-lvgl-sources.sh', 'lvgl', check: true).stdout().strip().split('\n')
)

common_dependencies = [
    depinih,
    deplibinput,
    deplibudev
]

subdir('unl0kr')
subdir('buffyboard')

if get_option('man')
    subdir('man')
endif
