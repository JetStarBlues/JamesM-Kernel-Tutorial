#include "common.h"

// Write a byte out to the specified port
void outb ( u16int port, u8int value )
{
	asm volatile ( "outb %1, %0" : : "dN" ( port ), "a" ( value ) );
}

// Read a byte from the specified port
u8int inb ( u16int port )
{
	u8int ret;
	asm volatile ( "inb %1, %0" : "=a" ( ret ) : "dN" ( port ) );
	return ret;
}

// Read a short from the specified port
u16int inw ( u16int port )
{
	u16int ret;
	asm volatile ( "inw %1, %0" : "=a" ( ret ) : "dN" ( port ) );
	return ret;
}

// Copy len bytes from src to dest
void memcpy ( u8int *dest, const u8int *src, u32int len )
{
	for ( ; len != 0; len -= 1 )
	{
		*dest = *src;

		dest += 1;
		src  += 1;
	}
}

// Write len copies of val into dest
void memset ( u8int *dest, u8int val, u32int len )  // byte-addressable
{
	for ( ; len != 0; len -= 1 )
	{
		*dest = val;

		dest += 1;
	}
}

// Compare two strings.
/* Should return,
     -1 if ( len(str1) > len(str2) ) or ( len(str2) > len(str1) )  // sort of. Shouldn't be used for len comp as will report mismatch first
      0 if str1 == str2
      1 otherwise
*/
int strcmp ( char *str1, char *str2 )
{
	int i = 0;
	int failed = 0;

	while ( str1[ i ] != '\0' && str2[ i ] != '\0' )  // haven't reached null-terminal of either string
	{
		if ( str1[ i ] != str2[ i ] )
		{
			failed = 1;

			break;
		}

		i += 1;
	}

	// did loop exit due to unequal lengths?
	if ( ( str1[ i ] == '\0' && str2[ i ] != '\0' ) ||  // len( str2 ) > len( str1 )
	     ( str1[ i ] != '\0' && str2[ i ] == '\0' ) )   // len( str1 ) > len( str2 )
	{
		failed = - 1;
	}

	return failed;
}

// Copy the NULL-terminated string src into dest, and return dest
char* strcpy ( char *dest, const char *src )
{
	do  // executed at least once
	{
		*dest = *src;

		dest += 1;
		src  += 1;
	}
	while ( *src != 0 );

	return dest;
}

// Concatenate the NULL-terminated string src onto the end of dest, and return dest
char* strcat ( char *dest, const char *src )
{
	// while ( *dest != 0 )  // doesn't equal null
	while ( *dest != '\0' )
	{
		dest += 1;
	}

	do  // executed at least once
	{
		*dest = *src;

		dest += 1;
		src  += 1;
	}
	// while ( *src != 0 );
	while ( *src != '\0' );

	return dest;
}

// Get the string length
u32int strlen ( const char *s )
{
	u32int len = 0;

	while ( *s != '\0' )
	{
		len += 1;
		s   += 1;
	}

	return len;	
}

// Prints message and enters infinite loop (stopping program execution)
extern void panic ( const char *message, const char *file, u32int line )
{
	// We encountered a massive problem and have to stop
	asm volatile( "cli" );  // disable interrupts

	monitor_write( "PANIC(" );
	monitor_write( ( char * ) message  );
	monitor_write( ") at "  );
	monitor_write( ( char * ) file     );
	monitor_write( ":"      );
	monitor_write_dec( line );
	monitor_write( "\n"     );

	// Halt by going into an infinite loop
	for( ;; );
}

// Prints message and enters infinite loop (stopping program execution)
extern void panic_assert ( const char *file, u32int line, const char *desc )
{
	// An assertion failed, and we have to panic
	asm volatile( "cli" );  // disable interrupts

	monitor_write( "ASSERTION-FAILED(" );
	monitor_write( ( char * ) desc     );
	monitor_write( ") at "             );
	monitor_write( ( char * ) file     );
	monitor_write( ":"                 );
	monitor_write_dec( line            );
	monitor_write( "\n"                );

	// Halt by going into an infinite loop
	for( ;; );
}
