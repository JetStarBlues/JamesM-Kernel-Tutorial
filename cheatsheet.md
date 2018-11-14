# QEMU

No display
* exit: `ctrl-a + x`

Curses as display
* exit:
	* `esc+2` to switch to qemu command line
	* Then type `q` and press `enter`


# GDB

Debugging
* `break`
	* set breakpoints. Pause execution when reach a point in the code.
	* examples:
		* `break fxName`
		* `break *(0xFFFF)`
		* `break *(fxName+offset)`
* `watch`
	* set watchpoints. Pause execution when a value changes.
	* examples:
		* `watch varName`
		* ...
* `info`
	* list breakpoints: `info break`
	* list watchpoints: `info watch`
* `delete #`
	* delete a break or watchpoint
* `continue`
	* resume program
* `step`
	* execute next line of code
* `stepi`
	* execute next line of assembly code
* `next`
	* Like `step` but steps over function calls (instead of entering them)
* `nexti`
	* ditto
* `print`
	* print variables
	* examples:
		* ...
* `x`
	* print memory
	* examples:
		* ...
* `backtrace`
	* show call stack
* `up`, `down`
	* navigate call stack
* `quit`

TUI
* enter or leave TUI mode: `ctrl-x + a`
* change focused window: `ctrl-x + o`
* show three windows: `ctrl-x + 2`
* change window content: `ctrl-x + 2` subsequent times
* previous command: `ctrl-p`

Trace commands
* Commands and outputs are written to gdb.txt
	* `set trace-commands on`
	* `set logging on`
* On another console/terminal, type `tail -f gdb.txt` to view the file in realtime

More
* Online manual([old][1], [new][2])






[1]: https://ftp.gnu.org/old-gnu/Manuals/gdb/html_node/gdb_toc.html
[2]: https://sourceware.org/gdb/onlinedocs/gdb/index.html
