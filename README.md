# BuffyBox

BuffyBox is a suite of graphical applications for the terminal.

## Components

### Standalone apps

**[buffyboard]** – Touch-enabled on-screen keyboard for virtual terminals

**[unl0kr]** – Disk unlocking UI for the initramfs

### Utilities

**[squeek2lvgl]** – Converter for transforming [Squeekboard] layouts into [LVGL]-compatible C code

**[shared]** – Internal code that is shared by some or all applications in the suite but not meant to be used externally 

## Contributing

You can join our development chat at [#buffybox:matrix.org].

## Making a release

To make it easier for distributions to package BuffyBox, we include source tarballs including the LVGL submodule in GitLab releases. See [unl0kr#42] for more background on this.

Producing and uploading a source tarball requires [git-archive-all] to be installed as well as an access token for the GitLab API. To tag a release and upload a source tarball, run

```
./release.sh $VERSION $TOKEN
```

Once the upload completes, create a new release on GitLab and attach the URL of the uploaded source tarball.

## License

This project is licensed under the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

[#buffybox:matrix.org]: https://matrix.to/#/#buffybox:matrix.org
[buffyboard]: ./buffyboard
[git-archive-all]: https://github.com/Kentzo/git-archive-all
[LVGL]: https://github.com/lvgl/lvgl
[shared]: ./shared
[squeek2lvgl]: ./squeek2lvgl
[Squeekboard]: https://gitlab.gnome.org/World/Phosh/squeekboard
[unl0kr]: ./unl0kr
[unl0kr#42]: https://gitlab.com/cherrypicker/unl0kr/-/issues/42
