#ifndef INITIAL_RAM_DISK_H
#define INITIAL_RAM_DISK_H


#include "common.h"
#include "fileSystem.h"

typedef struct
{
	u32int nfiles;  // The number of files in the ramdisk
}
initrd_header_t;

typedef struct
{
	u8int  magic;         // Magic number, for error checking
	s8int  name [ 128 ];  // Filename
	u32int offset;        // Offset in the initrd that the file starts
	u32int length;        // Length of the file
}
initrd_file_header_t;

// Initialises the initial ramdisk.
// Gets passed the address of the multiboot module,
// and returns a completed filesystem node
fs_node_t *initialise_initrd ( u32int location );


#endif  // INITIAL_RAM_DISK_H
