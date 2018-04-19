# Generate ELF ----------------------

# Compiles every file in SOURCES, then links them together
# into one ELF binary (aka kernel)

CSOURCES = main.c
COBJECTS = main.o
SSOURCES = boot.s
SOBJECTS = boot.o

# 32 bit
CFLAGS  = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32 -Wall -g
ASFLAGS = -f elf -F dwarf -g
LDFLAGS = -T link.ld -m elf_i386

# 64 bit
# CFLAGS  = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -g
# ASFLAGS = -f elf64
# LDFLAGS = -T link.ld

ELF = kernel

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


# Generate image --------------------
# Code sampled from https://github.com/mit-pdos/xv6-public/blob/master/Makefile
IMAGE = kernel.img

genImg:

	@echo "Generating image ..."

	dd if=/dev/zero of=$(IMAGE) count=10000  # /dev/zero is a special file in Unix that provides as many null characters (ASCII 0x00) as are read from it
	dd if=$(ELF)    of=$(IMAGE) conv=notrunc


# Run QEMU --------------------------
# Code sampled from https://github.com/mit-pdos/xv6-public/blob/master/Makefile

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

	@echo "Open gdb in another window and type the following:"
	@echo "\tset architecture i386"
	@echo "\ttarget remote localhost:"$(GDBPORT)
	@echo "\tsymbol-file kernel"


qemu-nox:

	make all

	@echo "Starting QEMU ..."

	$(QEMU) -nographic $(QEMUOPTS)

qemu-nox-gdb:

	make all

	make gdbinit

	@echo "Starting QEMU ..."

	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)
