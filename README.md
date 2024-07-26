# BuffyBox

BuffyBox is a suite of graphical applications for the terminal.

## Components

### Standalone apps

**[buffyboard]** – Touch-enabled on-screen keyboard for virtual terminals

**[unl0kr]** – Disk unlocking UI for the initramfs

### Utilities

**[squeek2lvgl]** – Converter for transforming [Squeekboard] layouts into [LVGL]-compatible C code

**[iskey]** - A utility for checking for key presses in bash scripts.

**[shared]** – Internal code that is shared by some or all applications in the suite but not meant to be used externally 

## Contributing

You can join our development chat at [#buffybox:matrix.org].

## Building & running

Some of unl0kr's dependencies are included as git submodules in this repository. You can clone the repository and initialise the submodules with

```
$ git clone https://gitlab.com/postmarketOS/buffybox.git
$ cd buffybox
$ git submodule init
$ git submodule update
```

When pulling changes from the remote later, either use `git pull --recurse-submodules` or manually run `git submodule update` as needed after pulling.

Once you have the sources, you can build the app and run it in a VT. Unless your user account has special privileges, `sudo` will be needed to access input device files.

```
$ meson setup _build
$ meson compile -C _build
$ sudo chvt 2
$ sudo ./_build/unl0kr/unl0kr # For Unl0kr
$ sudo ./_build/buffyboard/buffyboard # For Buffyboard
```

With meson <0\.55 use `ninja` instead of `meson compile`\.

## Making a release

To make it easier for distributions to package BuffyBox, we include source tarballs including the LVGL submodule in GitLab releases. See [unl0kr#42] for more background on this.

Producing and uploading a source tarball requires [git-archive-all] to be installed as well as an access token for the GitLab API. To tag a release and upload a source tarball, run

```
./release.sh $VERSION $TOKEN
```

Once the upload completes, create a new release on GitLab and attach the URL of the uploaded source tarball.

## License

This project is licensed under the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

For the license of bundled images and fonts, see [shared/cursor] and [shared/fonts].

[#buffybox:matrix.org]: https://matrix.to/#/#buffybox:matrix.org
[buffyboard]: ./buffyboard
[git-archive-all]: https://github.com/Kentzo/git-archive-all
[LVGL]: https://github.com/lvgl/lvgl
[shared]: ./shared
[squeek2lvgl]: ./squeek2lvgl
[iskey]: ./iskey
[Squeekboard]: https://gitlab.gnome.org/World/Phosh/squeekboard
[shared/cursor]: ./shared/cursor
[shared/fonts]: ./shared/fonts
[unl0kr]: ./unl0kr
[unl0kr#42]: https://gitlab.com/cherrypicker/unl0kr/-/issues/42
