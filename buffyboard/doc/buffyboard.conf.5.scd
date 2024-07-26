buffyboard(5) "buffyboard"

# NAME

BUFFYBOARD - configuration file

# DESCRIPTION

BUFFYBOARD expects a configuration file with options in the following format, with
one option/value pair per line:

```
[section]
<option>=<value>
```

The default configuration file is at /etc/buffyboard.conf, BUFFYBOARD will use this
configuration file if present along with overrides placed in
/etc/buffyboard.conf.d/ and those specified with the -C option. All config files
will be merged in the following order:
	- /etc/buffyboard.conf
	- /etc/buffyboard.conf.d/ (alphabetically)
	- Override files (in supplied order)

# OPTIONS

## Theme
*default* = <adwaita-light|adwaita-dark|breezy-light|breezy-dark|nord-light|nord-dark|pmos-light|pmos-dark>
	Selects the default theme on boot. Can be changed at runtime to the
	alternative theme. Default: breezy-dark.

## Input
*pointer* = <true|false>
	Enable or disable the use of a hardware mouse or other pointing device.
	Default: true.

*touchscreen* = <true|false>
	Enable or disable the use of the touchscreen.
	Default: true.

## Quirks
*fbdev_force_refresh* = <true|false>
	If true and using the framebuffer backend, this triggers a display refresh
	after every draw operation. This has a negative performance impact.
	Default: false.

# SEE ALSO
	*buffyboard*(1)

# AUTHORS

*Undef* <debian@undef.tools>
*Johannes Marbach* <n0-0ne@mailbox.org>
