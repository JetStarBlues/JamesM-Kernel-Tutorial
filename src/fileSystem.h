#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H


#include "common.h"
#include "../multiboot.h"

#define FS_FLAG_FILE        0x01  // is a file
#define FS_FLAG_DIRECTORY   0x02  // is a directory
#define FS_FLAG_CHARDEVICE  0x03
#define FS_FLAG_BLOCKDEVICE 0x04
#define FS_FLAG_PIPE        0x05
#define FS_FLAG_SYMLINK     0x06
#define FS_FLAG_MOUNTPOINT  0x08  // Is the file an active mountpoint?


struct fs_node;

// These typedefs define the type of callbacks called when read/write/open/close are called
typedef u32int ( *read_type_t  ) ( struct fs_node*, u32int, u32int, u8int* );
typedef u32int ( *write_type_t ) ( struct fs_node*, u32int, u32int, u8int* );
typedef void   ( *open_type_t  ) ( struct fs_node* );
typedef void   ( *close_type_t ) ( struct fs_node* );

//
typedef struct dirent *  ( *readdir_type_t ) ( struct fs_node*, u32int );
typedef struct fs_node * ( *finddir_type_t ) ( struct fs_node*, char *name );

//
typedef struct fs_node
{
	char           name[ 128 ];  // filename
	u32int         mask;         // permissions mask
	u32int         uid;          // owning user
	u32int         gid;          // owning group
	u32int         flags;        // see #defines above
	u32int         inode;        // device-specific. Provides a way for a filesystem to identify files
	u32int         length;       // size of the file in bytes
	u32int         impl;         // an implementation-defined number ?

	read_type_t    read;         // fx pointer
	write_type_t   write;        // fx pointer
	open_type_t    open;         // fx pointer
	close_type_t   close;        // fx pointer
	readdir_type_t readdir;      // fx pointer
	finddir_type_t finddir;      // fx pointer

	struct fs_node *ptr;         // Used by mountpoints and symlinks(shortcuts)
}
fs_node_t;

// One of these is returned by the readdir call, according to POSIX
struct dirent
{
	char   name [ 128 ];  // filename
	u32int ino;           // inode number
};

// The root of the filesystem
extern fs_node_t *fs_root;

/* Standard read/write/open/close functions. Note that these are all suffixed with
   _fs to distinguish them from the read/write/open/close which deal with file descriptors
   , not file nodes
*/
u32int read_fs  ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );
u32int write_fs ( fs_node_t *node, u32int offset, u32int size, u8int *buffer );
void   open_fs  ( fs_node_t *node, u8int read, u8int write );
void   close_fs ( fs_node_t *node );

//
struct dirent *readdir_fs ( fs_node_t *node, u32int index );
fs_node_t     *finddir_fs ( fs_node_t *node, char *name );

//
void initialise_fileSystem ( struct multiboot *mboot_ptr );


#endif  // FILE_SYSTEM_H
