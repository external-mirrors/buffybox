# Changelog

We're using semantic versioning and the following changelog types:

- feat: New features
- fix: Bug fixes
- misc: Anything else

If a change only affects particular applications, they are listed in parentheses after the type, e.g. feat(unl0kr).

## Unreleased

- feat: Add adwaita-dark theme (thanks @topjor)

## 3.1.0 (2024-04-10)

- feat(buffyboard): Handle input device connection/disconnection at runtime; adds new dependency libudev
- feat(buffyboard): Allow choosing theme via config and add all themes from unl0kr
- feat(buffyboard): Add fbdev force-refresh quirk via config
- feat(buffyboard): Allow disabling input devices via config
- feat(buffyboard): Add CLI flags for overriding geometry & DPI
- feat(buffyboard): Add CLI flag for verbose logging
- fix(unl0kr): Shutdown message box doesn't close on decline
- fix(unl0kr): Shutdown message box buttons and label are unstyled
- fix(unl0kr): Build fails when DRM is disabled
- misc: Update lvgl to git master (2023-04-10)

## 3.0.0 (2024-03-22)

- misc: Share the same lvgl submodule (and version) between buffyboard and unl0kr
- misc: Update lvgl to git master (2023-03-21) and eliminate lv_drivers

## unl0kr-2.0.3 (2023-12-19)

- fix: Clear the screen before exiting (https://gitlab.com/cherrypicker/unl0kr/-/issues/50)
- feat: Add extra top row for numbers & symbols, drop duplicative label (https://gitlab.com/cherrypicker/unl0kr/-/issues/49)
- fix: Stopgap solution for slow react on hardware keyboard (https://gitlab.com/cherrypicker/unl0kr/-/issues/36, thanks @jane400)

## unl0kr-2.0.2 (2023-10-07)

- fix: Prefer keyboard capability when input devices offer both keyboard and pointer (https://gitlab.com/cherrypicker/unl0kr/-/merge_requests/24, thanks @TravMurav)

## unl0kr-2.0.1 (2023-10-01)

- fix: Keyboard layout selection is broken (https://gitlab.com/cherrypicker/unl0kr/-/issues/45, thanks @Jarrah)

## unl0kr-2.0.0 (2023-07-28)

- feat: Add man page (https://gitlab.com/cherrypicker/unl0kr/-/issues/6, thanks @Jarrah)
- feat: Enable direct rendering
- feat: Add quirk for forcing fbdev refresh (https://gitlab.com/cherrypicker/unl0kr/-/issues/34, thanks @calebccff)
- fix: Fix slow react on hardware keyboard (https://gitlab.com/cherrypicker/unl0kr/-/issues/36, thanks @calebccff)

## unl0kr-1.0.0 (2023-03-14)

- feat: Handle input device connection/disconnection at runtime; adds new dependency libudev (https://gitlab.com/cherrypicker/unl0kr/-/issues/38)
- feat: Update lv_drivers to git master (2023-03-14)
- feat: Update lvgl to v8.3 (2023-03-14)
- feat: Deprecate -c CLI option and add support for reading from /etc/unl0kr.conf.d directory
- fix: Print correct version in logs
- feat: Add config options to enable terminal quirks (https://gitlab.com/cherrypicker/unl0kr/-/issues/34)

## unl0kr-0.3.1 (2023-02-25)

- fix: Changed Color of the cursor in breeze_dark to be able to see it (https://gitlab.com/cherrypicker/unl0kr/-/issues/39)

## unl0kr-0.3.0 (2022-11-13)

- feat: Update lvgl & lv_drivers to git master (2022-10-16)
- feat: Add config option to customise bullet character (https://gitlab.com/cherrypicker/unl0kr/-/issues/17)
- fix: Use actual screen DPI value to compute sizes and spaces
- feat: Allow shutting down the device on inactivity
- fix: Disable terminal keyboard while unl0kr is running
- feat: Extend --geometry flag to also accept display offsets
- fix: Swap order of KDSKBMODE and KDSETMODE to prevent blank screen (https://gitlab.com/cherrypicker/unl0kr/-/issues/34)
- fix: Increase maximum textarea width to prevent clipping on OnePlus 6 (https://gitlab.com/cherrypicker/unl0kr/-/issues/33)
- fix: Disable unused fonts, widgets and drivers (https://gitlab.com/cherrypicker/unl0kr/-/issues/13)
- fix: Rework pmOS theme to allow for more key space (https://gitlab.com/cherrypicker/unl0kr/-/issues/30)

## unl0kr-0.2.0 (2022-05-27)

- fix: Make meson actually install things (thanks @craftyguy)
- feat: Make en-US the default layout
- fix: Use correct pointer type in memset to avoid segfault
- feat: Set correct version in meson.build
- feat: Add postmarketOS theme (thanks @dylanvanassche)
- feat: Handle physical return key (https://gitlab.com/cherrypicker/unl0kr/-/issues/19)
- fix: Avoid TTY cursor blinking (https://gitlab.com/cherrypicker/unl0kr/-/issues/26)
- fix: Specify `check: true` when running external commands from meson (https://gitlab.com/cherrypicker/unl0kr/-/issues/23)
- feat: Hide partition name (https://gitlab.com/cherrypicker/unl0kr/-/issues/27)
- fix: Prevent scrolling when keyboard hides (https://gitlab.com/cherrypicker/unl0kr/-/issues/21)
- feat: Do not show last typed character when typing (https://gitlab.com/cherrypicker/unl0kr/-/issues/25)
- feat: Update lvgl & lv_drivers to git master (2022-02-21)
- feat: Allow runtime selection of the lvgl display driver ("backend")
- feat: Allow overriding dpi with the --dpi command line parameter

## buffyboard-0.2.0 (2021-11-23)

- feat: Enable software rotation

## buffyboard-0.1.0 (2021-11-21)

Initial release

## unl0kr-0.1.0 (2021-11-15)

Initial release
