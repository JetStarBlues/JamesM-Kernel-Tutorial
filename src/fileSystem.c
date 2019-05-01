#include "fileSystem.h"
#include "paging.h"
#include "initialRamDisk.h"


extern u32int placement_address;  //


// The root of the file system
fs_node_t *fs_root = 0;

u32int read_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer )
{
	// Does the node have a callback
	if ( node -> read != 0 )
	{
		return node -> read( node, offset, size, buffer );  // call callback
	}
	else
	{
		return 0;  // error
	}
}

u32int write_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer )
{
	// Does the node have a callback
	if ( node -> write != 0 )
	{
		return node -> write( node, offset, size, buffer );  // call callback
	}
	else
	{
		return 0;
	}
}

void open_fs ( fs_node_t *node, u8int read, u8int write )
{
	// Does the node have a callback
	if ( node -> open != 0 )
	{
		node -> open( node );  // call callback
	}
}

void close_fs ( fs_node_t *node )
{
	// Does the node have a callback
	if ( node -> close != 0 )
	{
		node -> close( node );  // call callback
	}
}

struct dirent *readdir_fs ( fs_node_t *node, u32int index )
{
	// Is the node a directory and does it have a callback
	if ( ( ( node -> flags & 0x7 ) == FS_FLAG_DIRECTORY ) &&
		 node -> readdir != 0 )
	{
		return node -> readdir( node, index );  // call callback
	}
	else
	{
		return 0;
	}
}

fs_node_t *finddir_fs ( fs_node_t *node, char *name )
{
	// Is the node a directory and does it have a callback
	if ( ( ( node -> flags & 0x7 ) == FS_FLAG_DIRECTORY ) &&
		 node -> finddir != 0 )
	{
		return node -> finddir( node, name );  // call callback
	}
	else
	{
		return 0;
	}
}

//
void initialise_fileSystem ( struct multiboot *mboot_ptr )
{
	// Find the location of our initial ramdisk
	ASSERT( mboot_ptr -> mods_count > 0 );

	u32int initrd_location = *( ( u32int * )   mboot_ptr -> mods_addr );
	u32int initrd_end      = *( ( u32int * ) ( mboot_ptr -> mods_addr + 4 ) );

	// Don't trample our module with placement accesses
	placement_address = initrd_end;

	// Initialise paging
	initialise_paging();

	// Initialise the initial ramdisk, and set it as the filesystem root
	fs_root = initialise_initrd( initrd_location );
}
