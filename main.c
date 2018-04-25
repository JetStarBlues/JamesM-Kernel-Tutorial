// Defines the C-code kernel entry point, calls initialisation routines

#include "src/monitor.h"

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
	return testInterrupt();
}
