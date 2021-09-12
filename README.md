# squeek2lvgl

squeek2lvgl is a Python script that makes it possible to use [squeekboard] keyboard layouts with [LVGL]'s keyboard widget. To achieve this, [squeekboard]'s YAML syntax for layout definitions is converted into a C file (with an accompanying header) that can then be added to an [LVGL] project.

To access [squeekboard]'s layout files, squeek2lvgl shallowly clones the [squeekboard] git repository into a temporary directory and purges it before exiting.

## Usage

First, set up the virtual Python environment and install the dependencies with

```
$ pipenv install
```

Afterwards you can execute the script with `pipenv run`. Use the `-h` / `--help` flag to print detailed usage instructions.

```
$ pipenv run python squeek2lvgl.py --help
usage: squeek2lvgl.py [-h] --input INPUT [--generate-scancodes] [--output OUTPUT]

Convert squeekboard layouts to LVGL-compatible C code.

optional arguments:
  -h, --help            show this help message and exit
  --input INPUT         squeekboard layout to use as input for generation. Has to be a YAML file path relative to data/keyboards. Can be specified
                        multiple times.
  --generate-scancodes  also generate scancode tables (only works for US layout currently
  --output OUTPUT       output directory for generated files
```

As an example, the following command converts [squeekboard]'s US layout to C

```
$ pipenv run python squeek2lvgl.py --input us.yaml --output .
```

When the process has finished, `sq2lv_layouts.h` and `sq2lv_layouts.c` will have been written into the current directory. Check the [examples] folder for further details about the generated files.

## License

squeek2lvgl Generator is licensed under the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

[squeekboard] and its keyboard layouts are licensed under the GNU General Public License version 3.0.

[LVGL]: https://github.com/lvgl/lvgl
[squeekboard]: https://gitlab.gnome.org/World/Phosh/squeekboard
[examples]: ./examples
