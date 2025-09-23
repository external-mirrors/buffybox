f0rmz
=====

Form collection tool for the initramfs based on [LVGL].

[[_TOC_]]

# About

f0rmz is a standalone form collection tool built on [LVGL], with rendering directly to the Linux framebuffer. It shares the same look and feel as [unl0kr] while providing a way to collect structured user input through configurable forms.

f0rmz was created for use cases like prompting for username, passphrases, and hostname during "first boot" in the initramfs, before creating LUKS partitions or generating configuration files.

# Status

f0rmz provides a complete form collection workflow:

- **Intro Page**: Configurable welcome screen with title and body text
- **Multi-field Forms**: Runtime-configurable forms via INI files using the same format as unl0kr
- **Touch-optimized UI**: One field per page for simplified display on small screens
- **Field Types**: Text and password input fields with validation
- **Summary Screen**: Review all values with click-to-edit capability
- **Input Support**: On-screen keyboard with layout switching, physical keyboard, mouse, and touchscreen
- **Theming**: Light and dark themes with runtime switching (shared with unl0kr)
- **Structured Output**: Key=value pairs printed to stdout for easy parsing

# Usage

f0rmz can be invoked with various command line options:

```
$ f0rmz --help
Usage: f0rmz [OPTION]

  -C, --config              Path to form configuration file
  -g, --geometry=NxM[@X,Y]  Force display size and offset
  -d, --dpi=N               Override display DPI
  -h, --help                Show this help
  -v, --verbose             Enable verbose logging
  -V, --version             Show version
```

f0rmz reads configuration files in the following order:
- `/usr/share/f0rmz/f0rmz.conf`
- `/usr/share/f0rmz/f0rmz.conf.d/*` (alphabetically)
- `/etc/f0rmz.conf`
- `/etc/f0rmz.conf.d/*` (alphabetically)
- Custom config file (specified with `-C`)

## Configuration

### Basic Example

```ini
[intro]
title=Welcome to postmarketOS!
body=Let's set up your device with a few quick questions.

[form.username]
type=text
label=Username
required=true

[form.password]
type=password
label=Password
required=true

[form.hostname]
type=text
label=Hostname
required=false
```

### Configuration Sections

#### `[intro]`
- `title` - Welcome screen title text
- `body` - Welcome screen body text

#### `[form.fieldname]`
Define form fields where `fieldname` is the output key name.

- `type` - Field type: `text` or `password`
- `label` - Display label for the field
- `required` - Whether field is required: `true` or `false` (default: `true`)

#### Shared Sections
f0rmz supports the same configuration sections as unl0kr for theming, keyboard, and input device settings. See [unl0kr.conf] for examples of `[general]`, `[keyboard]`, `[textarea]`, `[theme]`, `[input]`, and `[quirks]` sections.

## Navigation Flow

1. **Intro Page** - Welcome screen with "Get Started" button
2. **Field Pages** - One field per page with Previous/Next navigation  
3. **Summary Screen** - Review all values, click any field to edit
4. **Output** - Print `key=value` pairs to stdout and exit

## Output Format

f0rmz outputs structured data as key=value pairs:

```
username=alice
password=secretpass
hostname=mydevice
```

# Development

## Dependencies

- [inih] - INI file parsing
- [lvgl] (git submodule / linked statically)
- [squeek2lvgl] (linked statically)
- [libinput] - Input device handling  
- [libudev] - Device enumeration
- [libxkbcommon] - Keyboard layout support
- evdev kernel module

## Backends

f0rmz uses the same backend system as unl0kr:

- fbdev (default)
- drm (optional, if [libdrm] is available)

The active backend can be selected via the `general.backend` option in the configuration file.

## Keyboard layouts

f0rmz uses [squeekboard layouts] converted to C via [squeek2lvgl], the same as unl0kr. To regenerate the layouts, ensure that you have pipenv installed and run:

```
$ ./regenerate-layouts.sh
```

[LVGL]: https://lvgl.io
[unl0kr]: ../unl0kr
[inih]: https://github.com/benhoyt/inih
[lvgl]: https://github.com/lvgl/lvgl
[squeek2lvgl]: ../squeek2lvgl
[libinput]: https://gitlab.freedesktop.org/libinput/libinput
[libudev]: https://github.com/systemd/systemd/tree/main/src/libudev
[libxkbcommon]: https://github.com/xkbcommon/libxkbcommon
[libdrm]: https://gitlab.freedesktop.org/mesa/drm
[squeekboard layouts]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/tree/master/data/keyboards
[unl0kr.conf]: ../unl0kr/unl0kr.conf
