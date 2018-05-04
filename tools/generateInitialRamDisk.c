/*
	Usage:
		gcc -Wall generateInitialRamDisk.c -o generateInitialRamDisk.o
		./generateInitialRamDisk.o srcFilea1 dstFile 1 scrFile2 dstFile2 ...
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct initrd_header
{
	unsigned char magic;
	char          name [ 64 ];
	unsigned int  offset;  // ?
	unsigned int  length;  // length of file
};

int main ( int argc, char **argv )
{
	int i;
	int sz_initrd_header;
	char* srcFile;
	char* dstFile;

	int nheaders = ( argc - 1 ) / 2;

	struct initrd_header headers[ 64 ];

	sz_initrd_header = sizeof( struct initrd_header );

	printf( "Size of header: %d\n", sz_initrd_header );

	unsigned int offset = sz_initrd_header * 64 + sizeof( int );

	for ( i = 0; i < nheaders; i += 1 )
	{
		srcFile = argv[ i * 2 + 1 ];
		dstFile = argv[ i * 2 + 2 ];

		printf( "Writing file %s to %s at 0x%x\n", srcFile, dstFile, offset );

		strcpy( headers[ i ].name, dstFile );

		headers[ i ].offset = offset;

		FILE *stream = fopen( srcFile, "r" );

		if ( stream == 0 )
		{
			printf( "Error: file not found: %s\n", srcFile );

			return 1;
		}

		fseek( stream, 0, SEEK_END );

		headers[ i ].length = ftell( stream );

		offset += headers[ i ].length;

		fclose( stream );

		headers[ i ].magic = 0xBF;
	}

	FILE *wstream = fopen( "../initialRamDisk.img", "w" );

	unsigned char *data = ( unsigned char * ) malloc( offset );

	fwrite( &nheaders, sizeof( int ), 1, wstream );

	fwrite( headers, sz_initrd_header, 64, wstream );
	
	for ( i = 0; i < nheaders; i += 1 )
	{
		srcFile = argv[ i * 2 + 1 ];

		FILE *stream = fopen( srcFile, "r" );

		unsigned char *buf = ( unsigned char * ) malloc( headers[ i ].length );

		fread( buf, 1, headers[ i ].length, stream );

		fwrite( buf, 1, headers[ i ].length, wstream );

		fclose( stream );

		free( buf );
	}

	fclose( wstream );

	free( data );

	return 0;
}

