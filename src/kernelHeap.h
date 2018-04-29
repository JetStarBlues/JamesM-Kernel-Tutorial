#ifndef KHEAP_H
#define KHEAP_H


#include "common.h"

// Page align
u32int kmalloc_aligned ( u32int size );

// Return a physical address
u32int kmalloc_p ( u32int size, u32int *physical_address );

// Page align and return a physical address
u32int kmalloc_alignedp ( u32int size, u32int *physical_address );

// Vanilla
u32int kmalloc ( u32int size );


#endif  // KHEAP_H
