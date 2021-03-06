// Defines the C-code kernel entry point, calls initialisation routines

#include "src/common.h"
#include "src/monitor.h"
#include "src/descriptorTables.h"
#include "src/interrupts.h"
#include "src/timer.h"
#include "src/paging.h"
#include "src/kernelHeap.h"
#include "src/fileSystem.h"


u32int tick = 0;

int helloWorld ()  // section 2
{
	return 0xDEADBEEF;
}

int testScreen ()  // section 3
{
	monitor_write( "Bonjour!" );

	monitor_put( '\n' );
	monitor_write_hex( 255 );

	monitor_put( '\n' );
	monitor_write_dec( 12345 );

	return 0;
}

int testInterrupt ()  // section 4
{

	asm volatile ( "int $0x3" );
	asm volatile ( "int $0x4" );
	asm volatile ( "int $0x5" );

	return 0;
}

static void timerCallback ( registers_t regs )
{
	tick += 1;

	monitor_write( "Tick: " );
	monitor_write_dec( tick );
	monitor_put( '\n' );
}

int testTimer ()  // section 5
{
	// register handler
	register_interrupt_handler( IRQ0, &timerCallback );

	// enable interrupts
	asm volatile("sti");

	// init timer
	init_timer( 20 );  // frequency in Hz

	return 0;
}

int testPaging ()  // section 6
{
	initialise_paging();

	monitor_write( "Paging test...\n" );

	u32int *ptr = ( u32int* ) 0xA0000000;

	u32int trigger_page_fault = *ptr;  // force page fault by reading location

	return 0;
}

int testHeap ()  // section 7
{
	u32int a = kmalloc( 8 );  // allocated via placement_address because called before initializing paging

	initialise_paging();

	u32int b = kmalloc( 8 );
	u32int c = kmalloc( 8 );

	u32int b_old = b;

	monitor_write( "a: " ); monitor_write_hex( a ); monitor_writeln();
	monitor_write( "b: " ); monitor_write_hex( b ); monitor_writeln();
	monitor_write( "c: " ); monitor_write_hex( c ); monitor_writeln();

	kfree( ( void * ) c );
	kfree( ( void * ) b );

	u32int d = kmalloc( 12 );

	monitor_write( "d: " ); monitor_write_hex( d ); monitor_writeln();

	// if the address of d is the same as previously b, the reclaim was successful
	ASSERT( d == b_old );

	return 0;
}

int testFileSystem ( struct multiboot *mboot_ptr )  // section 8
{
	// Initialize filesystem. Also initializes paging.
	initialise_fileSystem( mboot_ptr );

    // list the contents of /
	int i = 0;
	struct dirent *node = 0;

	node = readdir_fs( fs_root, i );

	while ( node != 0 )
	{
		monitor_write( "Found file " );
		monitor_write( node -> name );
		monitor_writeln();
		
		fs_node_t *fsnode = finddir_fs( fs_root, node -> name );

		// Node is a directory
		if ( ( fsnode -> flags & 0x7 ) == FS_FLAG_DIRECTORY )
		{
			monitor_write( "(directory)\n\n" );
		}

		// Node is a file
		else
		{
			int j;
			char buffer[ 256 ];
			
			monitor_write( "__contents__\n" );
			
			u32int size = read_fs( fsnode, 0, 256, ( u8int * ) buffer );
			
			for ( j = 0; j < size; j += 1 )
			{
				monitor_put( buffer[ j ] );
			}

			monitor_write( "\n\n" );
		}

		i += 1;

		node = readdir_fs( fs_root, i );

	}

	return 0;
}

int testScrolling ()
{
	monitor_write( "[Verse 1]\n" );
	monitor_write( "I wanna be the very best\n" );
	monitor_write( "Like no one ever was\n" );
	monitor_write( "To catch them is my real test\n" );
	monitor_write( "To train them is my cause\n" );
	monitor_write( "I will travel across the land\n" );
	monitor_write( "Searching far and wide\n" );
	monitor_write( "Teach Pokemon to understand\n" );
	monitor_write( "The power that's inside\n" );
	monitor_write( "[Chorus]\n" );
	monitor_write( "(Pokemon, gotta catch 'em all) It's you and me\n" );
	monitor_write( "I know it's my destiny\n" );
	monitor_write( "(Pokemon) Oh, you're my best friend\n" );
	monitor_write( "In a world we must defend\n" );
	monitor_write( "(Pokemon, gotta catch 'em all) A heart so true\n" );
	monitor_write( "Our courage will pull us through\n" );
	monitor_write( "You teach me and I'll teach you\n" );
	monitor_write( "Pokemon! (Gotta catch 'em all) Gotta catch 'em all\n" );
	monitor_write( "[Verse 2]\n" );
	monitor_write( "Every challenge along the way\n" );
	monitor_write( "With courage I will face\n" );
	monitor_write( "I will battle every day\n" );
	monitor_write( "To claim my rightful place\n" );
	monitor_write( "Come with me, the time is right\n" );
	monitor_write( "There's no better team\n" );
	// monitor_write( "Arm in arm we'll win the fight\n" );
	// monitor_write( "It's always been our dream\n" );
	// monitor_write( "[Chorus]\n" );
	// monitor_write( "(Pokemon, gotta catch 'em all) It's you and me\n" );
	// monitor_write( "I know it's my destiny\n" );
	// monitor_write( "(Pokemon) Oh, you're my best friend\n" );
	// monitor_write( "In a world we must defend\n" );
	// monitor_write( "(Pokemon, gotta catch 'em all) A heart so true\n" );
	// monitor_write( "Our courage will pull us through\n" );
	// monitor_write( "You teach me and I'll teach you\n" );
	// monitor_write( "Pokemon! (Gotta catch 'em all) Gotta catch 'em all\n" );


	//
	// monitor_scrollUp();
	// monitor_scrollDown();
	// monitor_scrollDown();
	// monitor_scrollDown();
	// monitor_scrollDown();
	// monitor_scrollDown();
	// monitor_scrollDown();

	return 0;
}

void init ()
{
	// Initialize ISRs and segments
	init_descriptor_tables();

	// Initialize screen (by clearing it)
	monitor_clear();
}

int main ( struct multiboot *mboot_ptr )
{
	init();

	// Test ---
	// return helloWorld();
	// return testScreen();
	// return testInterrupt();
	// return testTimer();
	// return testPaging();
	// return testHeap();
	return testFileSystem( mboot_ptr );
	// return testScrolling();
}
