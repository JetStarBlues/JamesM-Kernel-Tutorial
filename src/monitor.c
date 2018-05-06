#include "monitor.h"

/*
	Something about booting by GRUB in text mode. Thus framebuffer controls characters (not pixels).
	Accessible at address 0xB8000. Part of VGA controller's memory that has been memory-mapped via
	hardware into our linear address space.

	The frame buffer is an array of 16-bit words, with each representing the display of one character.
	It represents an area 80 wide, 25 high.
		idx = ( y * 80 + x ) * 2   // x2 because 16-bit and 32-bit...?
	
	8 bits are used to represent a character, the other 8 are used to to designate the foreground and
	background colors (4 bits each).
		FEDC | BA98  | 76543210
		----- ------- ------------
		bgCol| fgCol | ASCII code

	Possible colors
	 0 - black
	 1 - blue
	 2 - green
	 3 - cyan
	 4 - red
	 5 - magenta
	 6 - brown
	 7 - light grey
	 8 - dark grey
	 9 - light blue
	10 - light green
	11 - light cyan
	12 - light red
	13 - light magenta
	14 - light brown
	15 - white

	VGA's control register 0x3D4 and data register 0x3D5 are used to update the cursor position
*/

//
#define NCOLS   80
#define NROWS   25
#define BGCOLOR 0   // black
#define FGCOLOR 15  // white

// #define NROWS_HISTORY NROWS * 5

// VGA framebuffer starts at 0xB8000
u16int *videoMemory = ( u16int * ) 0xB8000;

//
// u16int *textBuffer [ NCOLS * NROWS * 5 ];  // history

// cursor position
u8int cursorX = 0;
u8int cursorY = 0;

// default colors
// u8int  m_attributeByte = ( BGCOLOR << 4 ) | ( FGCOLOR & 0x0F );  // hi byte of word have to send to VGA
u16int m_attribute = ( ( BGCOLOR << 4 ) | ( FGCOLOR & 0x0F ) ) << 8;


// Updates the hardware cursor
static void move_cursor ()
{
	u16int cursorLocation = cursorY * NCOLS + cursorX;

	// For some reason VGA accepts 16-bit location as two bytes
	outb( 0x3D4, 14 );                   // Tell VGA we are setting the high cursor byte
	outb( 0x3D5, cursorLocation >> 8 );  // Send the high cursor byte
	outb( 0x3D4, 15 );                   // Tell VGA we are setting the low byte
	outb( 0x3D5, cursorLocation );       // Send it
}

// Scrolls the text on the screen up by one line
void scrollUp ()
{
	int i;
	int lastRow = NROWS - 1;

	u16int spaceChar = m_attribute | 0x20;  // space character

	// Move the current text chunk that makes up the screen back in the buffer by a line
	for ( i = 0; i < NROWS * NCOLS; i += 1 )
	{
		videoMemory[ i ] = videoMemory[ i + NCOLS ];
	}

	// The last line should now be blank. Do this by writing 80 spaces to it
	for ( i = lastRow * NCOLS; i < NROWS * NCOLS; i += 1 )
	{
		videoMemory[ i ] = spaceChar;
	}

	// The cursor should now be on the last line
	cursorY = lastRow;
}

// Scrolls the text on the screen down by one line
void scrollDown ()
{
	// TODO
}

// Write a single character out to the screen
void monitor_put ( char c )
{
	u16int *location;

	// Handle a backspace
	if ( c == 0x08 && cursorX )
	{
		cursorX -= 1;
	}

	// Handle a tab by increasing the cursor's x-pos, but only to a point where it's divisible by 8
	else if ( c == 0x09 )
	{
		cursorX = ( cursorX + 8 ) & ~ ( 8 - 1 );
	}

	// Handle a carriage return
	else if ( c == '\r' )
	{
		cursorX = 0;
	}

	// Handle newline by moving cursor back to left and incrementing the row
	else if ( c == '\n' )
	{
		cursorX = 0;
		cursorY += 1;
	}

	// Handle any other printable character
	else if ( c >= ' ' && c < 127 )
	{
		location = videoMemory + ( cursorY * NCOLS + cursorX );

		*location = m_attribute | c;

		cursorX += 1;
	}

	// Check if we need to inser a new line because we've reached the end of the screen
	if ( cursorX >= NCOLS )
	{
		cursorX = 0;
		cursorY += 1;
	}

	// Scroll the screen if needed
	if ( cursorY >= NROWS )
	{
		scrollUp();
	}

	// Move the hardware cursor
	move_cursor();
}

// Clear the screen to all black by writting spaces to the framebuffer
void monitor_clear ()
{
	int i;

	u16int spaceChar = m_attribute | 0x20;  // space character

	// Fill buffer with spaces
	for ( i = 0; i < NROWS * NCOLS; i += 1 )
	{
		videoMemory[ i ] = spaceChar;
	}

	// Move the hardware cursor back to the start
	cursorX = 0;
	cursorY = 0;
	move_cursor();
}

// Output a null-terminated ASCII string to the monitor
void monitor_write ( char *c )
{
	int i = 0;
	while ( c[ i ] )
	{
		monitor_put( c[ i ] );
		i += 1;
	}
}

// Write a newline character
void monitor_writeln ()
{
	monitor_put( '\n' );
}

//
void monitor_write_hex ( u32int n )
{
	s32int tmp;
	int i;

	monitor_write( "0x" );

	char noZeroes = 1;

	for ( i = 28; i > 0; i -= 4 )
	{
		tmp = ( n >> i ) & 0xF;

		if ( tmp == 0 && noZeroes != 0 )
		{
			continue;
		}

		if ( tmp >= 0xA )
		{
			noZeroes = 0;
			monitor_put( tmp - 0xA + 'a' );  // a..f
		}
		else
		{
			noZeroes = 0;
			monitor_put( tmp + '0' );  // 0..9
		}
	}

	// last nibble
	tmp = n & 0xF;

	if ( tmp >= 0xA )
	{
		monitor_put( tmp - 0xA + 'a' );  // a..f
	}
	else
	{
		monitor_put( tmp + '0' );  // 0..9
	}
}

//
void monitor_write_dec ( u32int n )
{
	char  c [ 32 ];
	char c2 [ 32 ];
	int i;
	int j;

	if ( n == 0 )
	{
		monitor_put( '0' );
		return;
	}

	s32int acc = n;

	i = 0;
	while ( acc > 0 )
	{
		c[ i ] = acc % 10 + '0';  //

		acc /= 10;

		i += 1;
	}
	c[ i ] = 0;  // ?

	//
	c2[ i ] = 0;  // ?
	i -= 1;

	// Reverse order so that MSD stored at index 0 ?
	j = 0;
	while ( i >= 0 )
	{
		c2[ i ] = c[ j ];

		i -= 1;
		j += 1;
	}

	monitor_write( c2 );
}
