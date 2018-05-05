# QEMU

No display
* exit: `ctrl-a + x`

Curses display
* exit:
	* `esc+2` to switch to qemu command line
	* Then type `q` and press `enter`


---

# GDB

Debugging
* `break`
	* set breakpoints. Pause execution when reach a point in the code.
* `watchpoint`
	* set watchpoints. Pause execution when a value changes.
* `up`, `down`
	* navigate call stack
* `print`
	* print variables
* `x`
	* print memory
* `info`

TUI
* enter or leave TUI mode: `ctrl-x + a`
* change focused window: `ctrl-x + o`
* show three windows: `ctrl-x + 2`
* change window content: `ctrl-x + 2` subsequent times

Trace commands
* Commands and outputs are written to gdb.txt
	* `set trace-commands on`
	* `set logging on`
* On another console/terminal, type `tail -f gdb.txt` to view the file in realtime
