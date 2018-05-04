#include "initialRamDisk.h"
#include "kernelHeap.h"

initrd_header_t      *initrd_header;  // The header

initrd_file_header_t *file_headers;   // The list of file headers

fs_node_t            *initrd_root;    // Our root directory node
fs_node_t            *initrd_dev;     // We also add a directory node for /dev, so we can mount devfs later on

fs_node_t            *root_nodes;     // List of file nodes
int                  nroot_nodes;     // Number of file nodes

struct dirent        dirent;

//
static u32int initrd_read ( fs_node_t *node, u32int offset, u32int size, u8int *buffer )
{
	initrd_file_header_t header = file_headers[ node -> inode ];

	if ( offset > header.length )
	{
		return 0;
	}

	if ( offset + size > header.length )
	{
		size = header.length - offset;
	}

	memcpy( ( u8int * ) buffer, ( u8int * ) ( header.offset + offset ), size );

	return size;
}

//
static struct dirent *initrd_readdir ( fs_node_t *node, u32int index )
{
	char *s;

	if ( node == initrd_root && index == 0 )
	{
		strcpy( dirent.name, "dev" );

		dirent.name[ 3 ] = 0;  // Make sure the string is null-terminated

		dirent.ino = 0;

		return &dirent;
	}

	if ( index - 1 >= nroot_nodes )
	{
		return 0;
	}

	s = root_nodes[ index - 1 ].name;

	strcpy( dirent.name, s );
	
	dirent.name[ strlen( s ) ] = 0;  // Make sure the string is null-terminated

	dirent.ino = root_nodes[ index - 1 ].inode;

	return &dirent;
}

//
static fs_node_t *initrd_finddir ( fs_node_t *node, char *name )
{
	int i;

	if ( node == initrd_root && ! strcmp( name, "dev" ) )
	{
		return initrd_dev;
	}

	for ( i = 0; i < nroot_nodes; i += 1 )
	{
		if ( ! strcmp( name, root_nodes[ i ].name ) )
		{
			return &root_nodes[ i ];
		}
	}

	return 0;
}

// Initialise the main and file header pointers and populate the root directory
fs_node_t *initialise_initrd ( u32int location )
{
	int i;

	// Initialise the main and file header pointers
	initrd_header = ( initrd_header_t * ) location;
	file_headers = ( initrd_file_header_t * ) ( location + sizeof( initrd_header_t ) );


	// Initialise the root directory
	initrd_root = ( fs_node_t * ) kmalloc( sizeof( fs_node_t ) );

	strcpy( initrd_root -> name, "root" );

	initrd_root -> mask    = 0;
	initrd_root -> uid     = 0;
	initrd_root -> gid     = 0;
	initrd_root -> inode   = 0;
	initrd_root -> length  = 0;

	initrd_root -> read    = 0;
	initrd_root -> write   = 0;
	initrd_root -> open    = 0;
	initrd_root -> close   = 0;

	initrd_root -> flags   = FS_FLAG_DIRECTORY;

	initrd_root -> readdir = &initrd_readdir;
	initrd_root -> finddir = &initrd_finddir;

	initrd_root -> impl    = 0;
	initrd_root -> ptr     = 0;


	// Initialise the /dev directory
	initrd_dev = ( fs_node_t * ) kmalloc( sizeof( fs_node_t ) );

	strcpy( initrd_dev -> name, "dev" );

	initrd_dev -> mask    = 0;
	initrd_dev -> uid     = 0;
	initrd_dev -> gid     = 0;
	initrd_dev -> inode   = 0;
	initrd_dev -> length  = 0;

	initrd_dev -> read    = 0;
	initrd_dev -> write   = 0;
	initrd_dev -> open    = 0;
	initrd_dev -> close   = 0;

	initrd_dev -> flags   = FS_FLAG_DIRECTORY;

	initrd_dev -> readdir = &initrd_readdir;
	initrd_dev -> finddir = &initrd_finddir;

	initrd_dev -> impl    = 0;
	initrd_dev -> ptr     = 0;


	//
	nroot_nodes = initrd_header -> nfiles;
	root_nodes = ( fs_node_t * ) kmalloc( sizeof( fs_node_t ) * nroot_nodes );

	// For every file...
	for ( i = 0; i < nroot_nodes; i += 1 )
	{
		/* Edit the file's header. Currently it holds the file offset relative
		   to the start of the ramdisk. We want it relative to the start of memory
		*/
		file_headers[ i ].offset += location;

		// Create a new file node
		strcpy( root_nodes[ i ].name, ( const char * ) &file_headers[ i ].name );

		root_nodes[ i ].mask    = 0;
		root_nodes[ i ].uid     = 0;
		root_nodes[ i ].gid     = 0;
		root_nodes[ i ].inode   = i;
		root_nodes[ i ].length  = file_headers[ i ].length;

		root_nodes[ i ].read    = &initrd_read;
		root_nodes[ i ].write   = 0;
		root_nodes[ i ].open    = 0;
		root_nodes[ i ].close   = 0;

		root_nodes[ i ].flags   = FS_FLAG_FILE;

		root_nodes[ i ].readdir = 0;
		root_nodes[ i ].finddir = 0;

		root_nodes[ i ].impl    = 0;
		root_nodes[ i ].ptr     = 0;
	}

	//
	return initrd_root;
}