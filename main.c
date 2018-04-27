// Defines the C-code kernel entry point, calls initialisation routines

#include "src/monitor.h"
#include "src/descriptorTables.h"
#include "src/interrupts.h"
#include "src/timer.h"

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

static void timer_callback ( registers_t regs )
{
	tick += 1;

	monitor_write( "Tick: " );
	monitor_write_dec( tick );
	monitor_put( '\n' );
}

int testTimer ()  // section 5
{
	// register handler
	register_interrupt_handler( IRQ0, &timer_callback );

	// enable interrupts
	asm volatile("sti");

	// init timer
	init_timer( 20 );  // frequency in Hz

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
	return testTimer();
}
