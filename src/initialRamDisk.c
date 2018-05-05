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
static u32int read_initrd ( fs_node_t *node, u32int offset, u32int size, u8int *buffer )
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
static struct dirent *readdir_initrd ( fs_node_t *node, u32int index )
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
static fs_node_t *finddir_initrd ( fs_node_t *node, char *name )
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

// Setup default values for a directory
static void initialise_dir ( fs_node_t* dir, const char *name )
{
	strcpy( dir -> name, name );

	dir -> mask    = 0;
	dir -> uid     = 0;
	dir -> gid     = 0;
	dir -> inode   = 0;
	dir -> length  = 0;

	dir -> read    = 0;
	dir -> write   = 0;
	dir -> open    = 0;
	dir -> close   = 0;

	dir -> flags   = FS_FLAG_DIRECTORY;

	dir -> readdir = &readdir_initrd;
	dir -> finddir = &finddir_initrd;

	dir -> impl    = 0;
	dir -> ptr     = 0;
}

// Setup default values for a file
static void initialise_file ( fs_node_t* file, const char *name, u32int length, u32int inodeNumber )
{
	strcpy( file -> name, name );

	file -> mask    = 0;
	file -> uid     = 0;
	file -> gid     = 0;
	file -> inode   = inodeNumber;
	file -> length  = length;

	file -> read    = &read_initrd;
	file -> write   = 0;
	file -> open    = 0;
	file -> close   = 0;

	file -> flags   = FS_FLAG_FILE;

	file -> readdir = 0;
	file -> finddir = 0;

	file -> impl    = 0;
	file -> ptr     = 0;
}

// Initialise the main and file header pointers and populate the root directory
fs_node_t *initialise_initrd ( u32int location )
{
	u32int i;

	// Initialise the main and file header pointers
	initrd_header = ( initrd_header_t * ) location;
	file_headers = ( initrd_file_header_t * ) ( location + sizeof( initrd_header_t ) );


	// Initialise the root directory
	initrd_root = ( fs_node_t * ) kmalloc( sizeof( fs_node_t ) );
	initialise_dir( initrd_root, "root" );


	// Initialise the /dev directory
	initrd_dev = ( fs_node_t * ) kmalloc( sizeof( fs_node_t ) );
	initialise_dir( initrd_dev, "dev" );


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

		// TODO.. somehow distinguish if dir, and call initialise_dir() instead

		// Initialise the file
		initialise_file(

			&root_nodes[ i ],                          // file
			( const char * ) &file_headers[ i ].name,  // name
			file_headers[ i ].length,                  // length
			i                                          // inode number
		);
	}

	//
	return initrd_root;
}
