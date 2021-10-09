Buffyboard
==========

Touch-enabled framebuffer keyboard (not only) for vampire slayers.

[[_TOC_]]

# About

Buffyboard is a touch-enabled on-screen keyboard running on the Linux framebuffer. It's primarily intended for vampire hunting[^1] but you can also use it as a general purpose keyboard.

Buffyboard uses [LVGL] for input processing and rendering. Key events are forwarded directly to the kernel via a uinput device. Since the latter emulates a hardware keyboard, the terminal keymap must match with buffyboard's layout or else on-screen keys might not produce the correct result.

# Status

We are en route to v1 which aims at providing a useable, visually pleasant application including [fbkeyboard]'s most essential features. For details about the current status, see the [v1 milestone]. You may also browse the full list of [open issues] to get an idea of what's planned beyond v1.

Here are a few highlights of what already works:

- On-screen keyboard control via mouse, trackpad or touchscreen
- Multi-layer keyboard layout including lowercase letters, uppercase letters, numbers and selected symbols (based on top three layers of [squeekboard's US terminal layout])
- Key chords with one or more modifiers terminated by a single non-modifier (e.g. `CTRL-c`)
- Highlighting of active modifiers
- Automatic resizing (and later reset) of active VT to prevent overlap with keyboard

For a growing collection of demo videos, see the [wiki].

## Upstreaming

Over the course of implementing buffyboard, suitable fixes and features have been upstreamed to the [lvgl] and [lv_drivers] repositories. The benefit of this goes both ways. Downstream we can rely on the features being maintained in the future and upstream they can make the features available to the larger audience of [LVGL] users.

Below is a summary of contributions upstreamed thus far.

### lvgl

- [feat(disp): Enable rendering to display subsection] (⏳ in review)
- [feat(keyboard): add user-defined modes]: (⏳ in review)

### lv_drivers

- [Make it possible to set the variable offset for fbdev] (✅ merged)

# Development

## Dependencies

- [lvgl] (git submodule / linked statically)
- [lv_drivers] (git submodule / linked statically)
- [squeek2lvgl] (git submodule / linked statically)
- [libinput]
- evdev kernel module
- uinput kernel module

## Building & running

For development and testing you can run the app in a VT. Unless your user account has special privileges, `sudo` will be needed to access input device files.

```
$ meson _build
$ meson compile -C _build
$ sudo chvt 2
$ sudo ./_build/buffyboard
```

With meson <0.55 use `ninja` instead of `meson compile`.

## Fonts

In order to work with [LVGL], fonts need to be converted to bitmaps, stored as C arrays. Buffyboard currently uses a combination of the [Montserrat] font for text and the [FontAwesome] font for pictograms. For both fonts only limited character ranges are included to reduce the binary size. To (re)generate the C file containing the combined font, run the following command

```
$ npx lv_font_conv --bpp 4 --size 32 --no-compress -o montserrat_extended_32.c --format lvgl \
    --font Montserrat-Regular.ttf \
      --range '0x0020-0x007F' \
      --range '0x00A0-0x00FF' \
      --range '0x0100-0x017F' \
      --range '0x2000-0x206F' \
      --range '0x20A0-0x20CF' \
    --font FontAwesome5-Solid+Brands+Regular.woff \
      --range '0xF001,0xF008,0xF00B,0xF00C,0xF00D,0xF011,0xF013,0xF015,0xF019,0xF01C,0xF021,0xF026,0xF027,0xF028,0xF03E,0xF0E0,0xF304,0xF043,0xF048,0xF04B,0xF04C,0xF04D,0xF051,0xF052,0xF053,0xF054,0xF067,0xF068,0xF06E,0xF070,0xF071,0xF074,0xF077,0xF078,0xF079,0xF07B,0xF093,0xF095,0xF0C4,0xF0C5,0xF0C7,0xF0C9,0xF0E7,0xF0EA,0xF0F3,0xF11C,0xF124,0xF158,0xF1EB,0xF240,0xF241,0xF242,0xF243,0xF244,0xF287,0xF293,0xF2ED,0xF55A,0xF7C2,0xF8A2' \
      --range '0xF35B'
```

Below is a short explanation of the different unicode ranges used above.

- [Montserrat]
  - Basic Latin (`0x0020-0x007F`)
  - Latin-1 supplement (`0x00A0-0x00FF`)
  - Latin extended A (`0x0100-0x017F`)
  - General punctuation (`0x2000-0x206F`)
  - Currency symbols (`0x20A0-0x20CF`)
- [FontAwesome]
  - Standard `LV_SYMBOL_*` glyphs (`0xF001,0xF008,0xF00B,0xF00C,0xF00D,0xF011,0xF013,0xF015,0xF019,0xF01C,0xF021,0xF026,0xF027,0xF028,0xF03E,0xF0E0,0xF304,0xF043,0xF048,0xF04B,0xF04C,0xF04D,0xF051,0xF052,0xF053,0xF054,0xF067,0xF068,0xF06E,0xF070,0xF071,0xF074,0xF077,0xF078,0xF079,0xF07B,0xF093,0xF095,0xF0C4,0xF0C5,0xF0C7,0xF0C9,0xF0E7,0xF0EA,0xF0F3,0xF11C,0xF124,0xF158,0xF1EB,0xF240,0xF241,0xF242,0xF243,0xF244,0xF287,0xF293,0xF2ED,0xF55A,0xF7C2,0xF8A2`)
  - [arrow-alt-circle-up] (`0xF35B`)

## Keyboard layouts

Buffyboard uses [squeekboard layouts] converted to C via [squeek2lvgl]. To regenerate the layouts, run

```
$ ./regenerate-layouts.sh
```

from the root of the repository.

# Acknowledgements

The [lv_port_linux_frame_buffer] project served as a starting point for the codebase.

The mouse cursor image was taken from [lv_sim_emscripten].

Buffyboard was inspired by [fbkeyboard].

# License

Buffyboard is licensed under the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

The [Montserrat] font is licensed under the Open Font License.

The [FontAwesome] font is licensed under the Open Font License version 1.1.

# Footnotes

[^1]: If you still don't know what vampires have to do with all of this, the train of thought is: Linux frame**buffer** 👉 **buffy**board 👉 ... wait for it ... 👉 [**Buffy** the Vampire Slayer]. There you have it. I never claimed I was funny. 😅

[**Buffy** the Vampire Slayer]: https://en.wikipedia.org/wiki/Buffy_the_Vampire_Slayer
[FontAwesome]: https://fontawesome.com/
[LVGL]: https://lvgl.io
[Make it possible to set the variable offset for fbdev]: https://github.com/lvgl/lv_drivers/pull/164
[Montserrat]: https://fonts.google.com/specimen/Montserrat
[arrow-alt-circle-up]: https://fontawesome.com/v5.15/icons/arrow-alt-circle-up?style=solid
[fbkeyboard]: https://github.com/bakonyiferenc/fbkeyboard
[feat(disp): Enable rendering to display subsection]: https://github.com/lvgl/lvgl/pull/2583
[feat(keyboard): add user-defined modes]: https://github.com/lvgl/lvgl/pull/2651/files
[libinput]: https://gitlab.freedesktop.org/libinput/libinput
[lv_drivers]: https://github.com/lvgl/lv_drivers
[lv_port_linux_frame_buffer]: https://github.com/lvgl/lv_port_linux_frame_buffer
[lv_sim_emscripten]: https://github.com/lvgl/lv_sim_emscripten/blob/master/mouse_cursor_icon.c
[lvgl/lv_drivers#151]: https://github.com/lvgl/lv_drivers/issues/151
[lvgl/lv_drivers#165]: https://github.com/lvgl/lv_drivers/pull/165
[lvgl/lv_drivers#166]: https://github.com/lvgl/lv_drivers/pull/166
[lvgl]: https://github.com/lvgl/lvgl
[open issues]: https://gitlab.com/cherrypicker/buffyboard/-/issues
[squeek2lvgl]: https://gitlab.com/cherrypicker/squeek2lvgl]
[squeekboard layouts]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/blob/master/data/keyboards
[squeekboard's US terminal layout]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/blob/master/data/keyboards/terminal/us.yaml
[squeekboard]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/tree/master
[v1 milestone]: https://gitlab.com/cherrypicker/buffyboard/-/milestones/1
[wiki]: https://gitlab.com/cherrypicker/buffyboard/-/wikis/home
