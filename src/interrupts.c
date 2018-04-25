#include "interrupts.h"
#include "monitor.h"

// This gets called from our ASM interrupt handler stub
void isr_handler( registers_t regs )
{
	monitor_write( "Recieved interrupt: " );
	monitor_write_dec( regs.int_no );
	monitor_put( '\n' );
}
