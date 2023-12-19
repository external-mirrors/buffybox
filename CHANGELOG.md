# Changelog

## Unreleased

- fix: Clear the screen before exiting (#50)
- feat: Add extra top row for numbers & symbols, drop duplicative label (#49)
- fix: Stopgap solution for slow react on hardware keyboard (#36, thanks @jane400)

## 2.0.2 (2023-10-07)

- fix: Prefer keyboard capability when input devices offer both keyboard and pointer (!24, thanks @TravMurav)

## 2.0.1 (2023-10-01)

- fix: Keyboard layout selection is broken (#45, thanks @Jarrah)

## 2.0.0 (2023-07-28)

- feat: Add man page (#6, thanks @Jarrah)
- feat!: Enable direct rendering
- feat: Add quirk for forcing fbdev refresh (#34, thanks @calebccff)
- fix: Fix slow react on hardware keyboard (#36, thanks @calebccff)

## 1.0.0 (2023-03-14)

- feat!: Handle input device connection/disconnection at runtime; adds new dependency libudev (#38)
- feat: Update lv_drivers to git master (2023-03-14)
- feat: Update lvgl to v8.3 (2023-03-14)
- feat!: Deprecate -c CLI option and add support for reading from /etc/unl0kr.conf.d directory
- fix: Print correct version in logs
- feat: Add config options to enable terminal quirks (#34)

## 0.3.1 (2023-02-25)

- fix: Changed Color of the cursor in breeze_dark to be able to see it (#39)

## 0.3.0 (2022-11-13)

- feat: Update lvgl & lv_drivers to git master (2022-10-16)
- feat: Add config option to customise bullet character (#17)
- fix: Use actual screen DPI value to compute sizes and spaces
- feat: Allow shutting down the device on inactivity
- fix: Disable terminal keyboard while unl0kr is running
- feat: Extend --geometry flag to also accept display offsets
- fix: Swap order of KDSKBMODE and KDSETMODE to prevent blank screen (#34)
- fix: Increase maximum textarea width to prevent clipping on OnePlus 6 (#33)
- fix: Disable unused fonts, widgets and drivers (#13)
- fix: Rework pmOS theme to allow for more key space (#30)

## 0.2.0 (2022-05-27)

- fix: Make meson actually install things (thanks @craftyguy)
- feat!: Make en-US the default layout
- fix: Use correct pointer type in memset to avoid segfault
- feat: Set correct version in meson.build
- feat: Add postmarketOS theme (thanks @dylanvanassche)
- feat: Handle physical return key (#19)
- fix: Avoid TTY cursor blinking (#26)
- fix: Specify `check: true` when running external commands from meson (#23)
- feat!: Hide partition name (#27)
- fix: Prevent scrolling when keyboard hides (#21)
- feat!: Do not show last typed character when typing (#25)
- feat: Update lvgl & lv_drivers to git master (2022-02-21)
- feat: allow runtime selection of the lvgl display driver ("backend")
- feat: allow overriding dpi with the --dpi command line parameter

## 0.1.0 (2021-11-15)

Initial release
