# Generate initrd -------------------

FSPATH = ../../fs/

# Use 'find' to get all the files and folders (recursive) in the directory
# Use 'sed' to remove FSPATH from returned paths (and make them relative)
FILES = $(shell find $(FSPATH) -mindepth 1 | sed 's|^$(FSPATH)||')

all:

	@echo "Generating initialRamDisk ..."

	gcc -Wall generateInitialRamDisk.c -o generateInitialRamDisk.o

	./generateInitialRamDisk.o $(FSPATH) $(FILES)
