// Kernel heap functions, also provides
// a placement malloc() for use before the heap is initialised

#include "kernelHeap.h"

// end is defined in the linker script?
extern u32int end;

u32int placement_address = ( u32int ) &end;  // ??

u32int kmalloc_ ( u32int size, int align, u32int *physical_address )
{
	/* This will eventually call malloc() on the kernel heap.
	   For now, though, we just assign memory at placement_address
	   and increment it by sz. Even when we've coded our kernel
	   heap, this will be useful for use before the heap is initialised
	*/

	// Page align the address
	if ( align == 1 && ( placement_address & 0xFFFFF000 ) )
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;  // ?? 4K
	}

	u32int base = placement_address;

	// Share physical address
	if ( physical_address )
	{
		*physical_address = base;
	}

	placement_address += size;

	return base;  // how tf is this different from physical_address?
}

// Page align
u32int kmalloc_aligned ( u32int size )
{
	return kmalloc_( size, 1, 0 );
}

// Return a physical address
u32int kmalloc_p ( u32int size, u32int *physical_address )
{
	return kmalloc_( size, 0, physical_address );
}

// Page align and return a physical address
u32int kmalloc_alignedp ( u32int size, u32int *physical_address )
{
	return kmalloc_( size, 1, physical_address );
}

// Vanilla
u32int kmalloc ( u32int size )
{
	return kmalloc_( size, 0, 0 );
}
