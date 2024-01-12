

squeek2lvgl_src = files(
	'sq2lv.c',
)

squeek2lvgl_lib = static_library(
	'squeek2lvgl',
	squeek2lvgl_src,
	dependencies: [
		lvgl,
	],
)

squeek2lvgl = declare_dependency(
	link_with: squeek2lvgl_lib,
	include_directories: [
		'.',
	],
)
