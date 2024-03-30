Fonts
=====

In order to work with [LVGL], fonts need to be converted to bitmaps, stored as C arrays. BuffyBox currently uses a combination of the [OpenSans] font for text and the [FontAwesome] font for pictograms. For both fonts only limited character ranges are included to reduce the binary size. To (re)generate the C file containing the combined font, run the following command

```
$ ./regenerate.sh
```

Below is a short explanation of the different unicode ranges used above.

- [OpenSans]
  - Basic Latin (`0x0020-0x007F`)
  - Latin-1 supplement (`0x00A0-0x00FF`)
  - Latin extended A (`0x0100-0x017F`)
  - Greek and Coptic (`0x0370-0x03FF`)
  - General punctuation (`0x2000-0x206F`)
  - Currency symbols (`0x20A0-0x20CF`)
  - Mathematical operators (`0x2200-0x22FF`)
- [FontAwesome]
  - Standard `LV_SYMBOL_*` glyphs (`0xF001,0xF008,0xF00B,0xF00C,0xF00D,0xF011,0xF013,0xF015,0xF019,0xF01C,0xF021,0xF026,0xF027,0xF028,0xF03E,0xF0E0,0xF304,0xF043,0xF048,0xF04B,0xF04C,0xF04D,0xF051,0xF052,0xF053,0xF054,0xF067,0xF068,0xF06E,0xF070,0xF071,0xF074,0xF077,0xF078,0xF079,0xF07B,0xF093,0xF095,0xF0C4,0xF0C5,0xF0C7,0xF0C9,0xF0E7,0xF0EA,0xF0F3,0xF11C,0xF124,0xF158,0xF1EB,0xF240,0xF241,0xF242,0xF243,0xF244,0xF287,0xF293,0xF2ED,0xF55A,0xF7C2,0xF8A2`)
  - [adjust] (`0xF042`)
  - [arrow-alt-circle-up] (`0xF35B`)

## License

The [OpenSans] font is licensed under the Apache License 2.0.

The [FontAwesome] font is licensed under the Open Font License version 1.1.

[LVGL]: https://lvgl.io
[FontAwesome]: https://fontawesome.com/
[OpenSans]: https://fonts.google.com/specimen/Open+Sans
[adjust](https://fontawesome.com/v5/icons/adjust)
[arrow-alt-circle-up](https://fontawesome.com/v5/icons/arrow-alt-circle-up)
