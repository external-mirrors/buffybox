# squeek2lvgl

squeek2lvgl is a Python script and an accompanying C library that make it possible to use [squeekboard] keyboard layouts with [LVGL]'s keyboard widget. To achieve this, [squeekboard]'s YAML syntax for layout definitions is converted into C files that, together with the library C files, can then be added to an [LVGL] project.

To access [squeekboard]'s layout files, squeek2lvgl shallowly clones the [squeekboard] git repository into a temporary directory and purges it before exiting.

**squeek2lvgl has been donated to the postmarketOS project. Development continues in https://gitlab.postmarketos.org/postmarketOS/buffybox.**

## Usage

First, set up the virtual Python environment and install the dependencies with

```
$ pipenv install
```

Afterwards you can execute the script with `pipenv run`. Use the `-h` / `--help` flag to print detailed usage instructions.

```
$ pipenv run python squeek2lvgl.py --help
usage: squeek2lvgl.py [-h] --input INPUT --name NAME
                      [--extra-top-row-base EXTRA_TOP_ROW_BASE]
                      [--extra-top-row-upper EXTRA_TOP_ROW_UPPER]
                      [--shift-keycap SHIFT_KEYCAP]
                      [--surround-space-with-arrows] [--generate-scancodes]
                      --output OUTPUT

Convert squeekboard layouts to LVGL-compatible C code.

options:
  -h, --help            show this help message and exit
  --input INPUT         squeekboard layout to use as input for generation. Has
                        to be a YAML file path relative to data/keyboards. Can
                        be specified multiple times.
  --name NAME           name for the layout. Needs to be specified once for
                        every --input flag.
  --extra-top-row-base EXTRA_TOP_ROW_BASE
                        additional key row to add at the top of the base
                        layer.
  --extra-top-row-upper EXTRA_TOP_ROW_UPPER
                        additional key row to add at the top of the upper
                        layer.
  --shift-keycap SHIFT_KEYCAP
                        key caption for the Shift key. Defaults to "Shift".
  --surround-space-with-arrows
                        insert left / right arrow before / after space key
  --generate-scancodes  also generate scancode tables (only works for US
                        layout currently)
  --output OUTPUT       output directory for generated files

```

As an example, the following command converts [squeekboard's US layout] to C

```
$ pipenv run python squeek2lvgl.py --input us.yaml --output .
```

When the process has finished, `sq2lv_layouts.h` and `sq2lv_layouts.c` will have been written into the current directory. Check the [examples] folder for further details about the generated files.

To facilitate usage, a minimalist C API is available in [sq2lv.h] and [sq2lv.c].

### Integrating into a project

Similar to [LVGL] squeek2lvgl should be added into a project as a git submodule. The generated C files assume that you have integrated [LVGL] as a submodule in the `./lvgl` folder. The C library files in turn assume that you have imported squeek2lvgl as a submodule one folder above the location of the generated files.

The following is an example directory hierarchy:

```
$ tree .
.
├── lvgl             # LVGL submodule
│   ├── ...
├── main.c           # Project's own source files
├── ...
├── sq2lv_layouts.c  # Layouts generated with squeek2lvgl
├── sq2lv_layouts.h
├── ...
├── squeek2lvgl      # squeek2lvgl submodule
│   ├── sq2lv.c      # squeek2lvgl library code
│   ├── sq2lv.h
│   ├── ...
├── ...
```

Using the directory structure above, you can then add `sq2lv_layouts.c` and `squeek2lvgl/sq2lv.c` into your build process just like the rest of your project's sources.

## License

squeek2lvgl is licensed under the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

[squeekboard] and its keyboard layouts are licensed under the GNU General Public License version 3.0.

[LVGL]: https://github.com/lvgl/lvgl
[squeekboard]: https://gitlab.gnome.org/World/Phosh/squeekboard
[squeekboard's US layout]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/blob/master/data/keyboards/us.yaml
[examples]: ./examples
[sq2lv.h]: ./sq2lv.h
[sq2lv.c]: ./sq2lv.c
