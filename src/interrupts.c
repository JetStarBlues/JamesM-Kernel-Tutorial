#include "interrupts.h"
#include "monitor.h"

isr_t interrupt_handlers [ 256 ];

// This gets called from our ASM interrupt handler stub
void isr_handler( registers_t regs )
{
	monitor_write( "Recieved interrupt: " );
	monitor_write_dec( regs.int_no );
	monitor_put( '\n' );

	// Execute handler associated with interrupt
	if ( interrupt_handlers[ regs.int_no ] != 0 )
	{
		isr_t handler = interrupt_handlers[ regs.int_no ];

		handler( regs );
	}
}

// Allows you to register custom interrupt handlers??
void register_interrupt_handler ( u8int n, isr_t handler )
{
	interrupt_handlers[ n ] = handler;
}

// This gets called from our ASM interrupt handler stub
void irq_handler( registers_t regs )
{
	// Send an end of interrupt signal to the PICs (programmable interrupt controllers)

	// if this interrupt involved PIC2 (IRQ > 7)
	if ( regs.int_no >= 40 )
	{
		// send reset command to PIC2 (signal end of interrupt)
		outb( 0xA0, 0x20 );
	}
	// send reset command to PIC1 (signal end of interrupt)
	outb( 0x20, 0x20 );

	// Execute handler associated with interrupt
	if ( interrupt_handlers[ regs.int_no ] != 0 )
	{
		isr_t handler = interrupt_handlers[ regs.int_no ];

		handler( regs );
	}
}

/*
void dummyHandler ( registers_t regs )
{
	// stuff
}
register_interrupt_handler( 0, dummyHandler );
*/
