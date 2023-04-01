unl0kr(5) "unl0kr"

# NAME

UNL0KR - configuration file

# DESCRIPTION

UNL0KR expects a configuration file with options in the following format, with 
one option/value pair per line:

```
[section]
<option>=<value>
```

The default configuration file is at /etc/unl0kr.conf, UNL0KR will use this 
configuration file if present along with overrides placed in 
/etc/unl0kr.conf.d/ and those specified with the -C option. All config files 
will be merged in the following order:
	- /etc/unl0kr.conf 
	- /etc/unl0kr.conf.d/ (alphabetically)
	- Override files (in supplied order)

# OPTIONS

## General
*animations* = <true|false>
	Enable or disable animations. Useful for slower devices. Default: false.

*backend* = <fbdev|drm>
	The rendering backend to use. Default: fbdev.

*timeout* = <value>
	The time in seconds before unl0kr will consider the entry a failure 
	and shutdown. Setting timeout to 0 disables this feature. Default: 0.

## Keyboard
*autohide* = <true|false>
	Whether to automatically hide the keyboard when a hardware keyboard 
	is detected on launch. Default: true.

*layout* = <us|de|fr|...>
	The default layout to use. Can be changed from the UI at runtime. 
	The available options are defined by the available keyboards at build time.
	Default: us.

*popovers* = <true|false>
	Enable or disable key press popovers showing the selected key.
	default: true.

## Textarea
*obscured* = <true|false>
	Whether the password in the text entry box can be read. Selectable in 
	the UI at runtime. Default: true.

*bullet* = <value>
	The character that will be used to obscure the password. Default: bullet.

## Theme
*default* = <breezy-light|breezy-dark|pmos-light|pmos-dark>
	Selects the default theme on boot. Can be changed at runtime to the 
	alternative theme. Default: breezy-dark.

*alternative* = <breezy-light|breezy-dark|pmos-light|pmos-dark>
	Selects the alternative theme which the user can then choose on boot.
	Default: breezy-light.

## Input
*keyboard* = <true|false>
	Enable or disable the use of hardware keyboards. Default: true

*pointer* = <true|false>
	Enable or disable the use of a hardware mouse or other pointing device.
	Default: true.

*touchscreen* = <true|false>
	Enable or disable the use of the touchscreen.
	Default: true.

## Quirks
*terminal_prevent_graphics_mode* = <true|false>
	If true this avoids setting the terminal into graphics mode. Will show 
	terminal command prompt. Default: false.

*terminal_allow_keyboard_input* = <true|false>
	If true this avoids turning off terminal keyboard input. This will show 
	your password on the terminal. Default: false.

# SEE ALSO
	*unl0kr*(1)

# AUTHORS

*Undef* <debian@undef.tools>
