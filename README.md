Buffyboard
==========

Buffyboard is a touch-enabled on-screen keyboard running on the Linux framebuffer. It's primarily intended for vampire ⃰  hunting but you can also use it as a general purpose keyboard.

Buffyboard uses [lvgl] for input processing and rendering. Key events are forwarded directly to the kernel via a uinput device. Since the latter emulates a hardware keyboard, the terminal keymap must match with Buffyboard's layout or else on-screen keys might not produce the correct result.

# Status

While partially usable, buffyboard currently still is a work in progress.

## What works

- On-screen keyboard control via mouse, trackpad or touchscreen
- Multi-layer keyboard layout including lowercase letters, uppercase letters, numbers and selected symbols (based on top three layers of [squeekboard's US terminal layout])
- Key chords with one or more modifiers terminated by a single non-modifier (e.g. `CTRL-c`)
- Highlighting of active modifiers
- Automatic resizing (and later reset) of active VT to prevent overlap with keyboard

## To do

- Show keyboard at the bottom of the screen instead of at the top (just need to set the offset once [lvgl/lv_drivers#164] is merged)
- Support different screen rotations
- Eliminate `libinput_multi.[ch]` once support for multiple input devices has been upstreamed to [lv_drivers] (see [lvgl/lv_drivers#151])
- Add remaining layers from [squeekboard's US terminal layout] (symbols and actions)
- Warn about mismatches between on-screen keyboard layout and terminal keyboard layout
- Add [squeekboard's FR terminal layout]
- And, and, and ...

# Development

## Dependencies

- [lvgl] (git submodule / linked statically)
- [lv_drivers] (git submodule / linked statically)
- [libinput]
- evdev kernel module
- uinput kernel module

## Building & running

For development and testing you can run the app in a VT. Usually `sudo` will be needed in order to access input device files.

```
$ make
$ sudo chvt 2
$ sudo ./buffyboard
```

## Changing fonts

In order to work with [lvgl], fonts need to be converted to C arrays. Buffyboard currently uses a combination of the [Montserrat] font for text and the [FontAwesome] font for pictograms. For both fonts only limited character ranges are included. To (re)generate the C file containing the combined font, run the following command

```
$ npx lv_font_conv --bpp 4 --size 32 --no-compress -o montserrat_extended_32.c --format lvgl \
    --font Montserrat-Regular.ttf \
      --range '0x0020-0x007F' \ # basic Latin
      --range '0x00A0-0x00FF' \ # Latin-1 supplement
      --range '0x0100-0x017F' \ # Latin extended A
      --range '0x2000-0x206F' \ # general punctuation
      --range '0x20A0-0x20CF' \ # currency symbols
    --font FontAwesome5-Solid+Brands+Regular.woff \
      --range '0xF001,0xF008,0xF00B,0xF00C,0xF00D,0xF011,0xF013,0xF015,0xF019,0xF01C,0xF021,0xF026,0xF027,0xF028,0xF03E,0xF0E0,0xF304,0xF043,0xF048,0xF04B,0xF04C,0xF04D,0xF051,0xF052,0xF053,0xF054,0xF067,0xF068,0xF06E,0xF070,0xF071,0xF074,0xF077,0xF078,0xF079,0xF07B,0xF093,0xF095,0xF0C4,0xF0C5,0xF0C7,0xF0C9,0xF0E7,0xF0EA,0xF0F3,0xF11C,0xF124,0xF158,0xF1EB,0xF240,0xF241,0xF242,0xF243,0xF244,0xF287,0xF293,0xF2ED,0xF55A,0xF7C2,0xF8A2' # Standard LV_SYMBOL_* glyphs
```

## Changing layouts

Buffyboard uses [squeekboard layouts] converted to C via [squeek2lvgl]. To regenerate the layouts, run

```
$ ./regenerate-layouts.sh
```

from the root of the repository.

# Acknowledgements

The [lv_port_linux_frame_buffer] project served as a starting point for the codebase.

The mouse cursor image was taken from [lv_sim_emscripten].

Buffyboard was inspired by [bakonyiferenc/fbkeyboard].

# License

Buffyboard is licensed under the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

[squeekboard] and its keyboard layouts are licensed under the GNU General Public License version 3.0.

The [Montserrat] font is licensed under the Open Font License.

The [FontAwesome] font is licensed under the Open Font License version 1.1.

# Footnotes

⃰  If you still don't know what vampires have to do with all of this, the train of thought is: Linux frame**buffer** 👉 **buffy**board 👉 [90s TV show featuring Sarah Michelle Gellar]. There you have it. I never claimed I was funny. 😅

[90s TV show featuring Sarah Michelle Gellar]: https://en.wikipedia.org/wiki/Buffy_the_Vampire_Slayer
[squeekboard's US terminal layout]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/blob/master/data/keyboards/terminal/us.yaml
[lv_drivers]: https://github.com/lvgl/lv_drivers
[lvgl/lv_drivers#164]: https://github.com/lvgl/lv_drivers/pull/164
[lvgl/lv_drivers#151]: https://github.com/lvgl/lv_drivers/issues/151
[squeekboard's FR terminal layout]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/blob/master/data/keyboards/terminal/fr.yaml
[lvgl]: https://github.com/lvgl/lvgl
[libinput]: https://gitlab.freedesktop.org/libinput/libinput
[Montserrat]: https://fonts.google.com/specimen/Montserrat
[FontAwesome]: https://fontawesome.com/
[squeekboard layouts]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/blob/master/data/keyboards
[squeek2lvgl]: https://gitlab.com/cherrypicker/squeek2lvgl]
[lv_port_linux_frame_buffer]: https://github.com/lvgl/lv_port_linux_frame_buffer
[lv_sim_emscripten]: https://github.com/lvgl/lv_sim_emscripten/blob/master/mouse_cursor_icon.c
[bakonyiferenc/fbkeyboard]: https://github.com/bakonyiferenc/fbkeyboard
[squeekboard]: https://gitlab.gnome.org/World/Phosh/squeekboard/-/tree/master
