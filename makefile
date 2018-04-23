# Makefile code mashup of
#  https://github.com/mit-pdos/xv6-public/blob/master/Makefile
#  https://github.com/berkus/jamesm-tutorials/blob/master/Makefile

# Generate ELF ----------------------

# Compiles every file in SOURCES, then links them together
# into one ELF binary (aka kernel)

SRCDIR = src/

# C files
CSOURCES =              \
	main.c              \
	$(SRCDIR)common.c   \
	$(SRCDIR)monitor.c
COBJECTS =              \
	main.o              \
	$(SRCDIR)common.o   \
	$(SRCDIR)monitor.o

# Assembly files
SSOURCES = boot.s
SOBJECTS = boot.o


VPATH = $(SRCDIR)
ELF   = kernel

# 32 bit
CFLAGS  = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32 -Wall -g
ASFLAGS = -f elf -F dwarf -g
LDFLAGS = -T link.ld -m elf_i386

# 64 bit
# CFLAGS  = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -g
# ASFLAGS = -f elf64 -F dwarf -g
# LDFLAGS = -T link.ld

all:

	make clean

	@echo "Making all the things!"

	make _all

_all: $(SOBJECTS) $(COBJECTS) link

clean:

	@echo "Removing object files ..."

	-rm *.o $(ELF) $(IMAGE)

link:

	@echo "Linking ..."

	ld $(LDFLAGS) -o $(ELF) $(SOBJECTS) $(COBJECTS)

.s.o:

	@echo "Assembling" $< "..."

	nasm $(ASFLAGS) $<

.c.o:

	@echo "Compiling" $< "..."

	gcc -c $(CFLAGS) $< -o $@


# Run QEMU --------------------------

# Path to QEMU
QEMU = qemu-system-i386

# Flags
QEMUOPTS = -kernel $(ELF)

# Try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)

# QEMU's gdb stub command line changed in 0.11
QEMUGDB = \
	$(shell if $(QEMU) -help | grep -q '^-gdb';  \
	then echo "-gdb tcp::$(GDBPORT)";            \
	else echo "-s -p $(GDBPORT)"; fi)

# Prompt user to setup GDB appropriately
gdbinit:

	@echo "\nOpen gdb in another window and type the following:"
	@#echo "   set architecture i386"
	@echo "   target remote :"$(GDBPORT)
	@echo "   file kernel"
	@echo "Use 'break' to set breakpoints, and 'continue' to execute.\n"

qemu:

	make all

	@echo "Starting QEMU ..."
	@echo "If using curses, press Esc+2 then type 'quit' to exit"

	@# $(QEMU) $(QEMUOPTS)
	$(QEMU) -display curses $(QEMUOPTS)

qemu-nox:

	make all

	@echo "Starting QEMU ..."

	$(QEMU) -nographic $(QEMUOPTS)

qemu-gdb:

	make all

	make gdbinit

	@echo "Starting QEMU ..."

	@#$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)
	$(QEMU) -display curses $(QEMUOPTS) -S $(QEMUGDB)

qemu-nox-gdb:

	make all

	make gdbinit

	@echo "Starting QEMU ..."
	@echo "If using curses, press Esc+2 then type 'quit' to exit"

	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)
