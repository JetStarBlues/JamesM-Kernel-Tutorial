Working through [James Molloy's Kernel Development tutorial][0].

Uses QEMU instead of Bochs.

To run:

- with display, without debugging
	- `make qemu`
- with display, with debugging
	- `make qemu-gdb`
- without display, without debugging
	- `make qemu-nox`
- without display, with debugging
	- `make qemu-nox-gdb`

Uses curses for display. In the makefile, you can change this so that it uses a [display of your choice][1].


[0]: http://www.jamesmolloy.co.uk/tutorial_html/
[1]: https://wiki.gentoo.org/wiki/QEMU/Options