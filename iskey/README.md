# iskey

A tiny utility for determining if a key is pressed.

iskey is intended to be used in the context of a constrained environment like
the initramfs, it allows for easily scripting things like "is the user holding
volume down" which are generally non-trivial to check for in a shell script.
