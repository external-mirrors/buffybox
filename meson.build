project(
  'buffybox',
  'c',
  version: '3.2.0',
  default_options: 'warning_level=3',
  meson_version: '>=0.53.0'
)

add_project_arguments('-DBB_VERSION="@0@"'.format(meson.project_version()), language: ['c'])
add_project_arguments('-DUL_VERSION="@0@"'.format(meson.project_version()), language: ['c'])

subdir('unl0kr')
subdir('buffyboard')
