#ifndef MONITOR_H
#define MONITOR_H


#include "common.h"

// Updates the hardware cursor
// static void move_cursor ();

// Scrolls the text on the screen up by one line
// static void scroll ();

// Write a single character out to the screen
void monitor_put ( char c );

// Clear the screen to all black
void monitor_clear ();

// Output a null-terminated ASCII string to the monitor
void monitor_write ( char *c );

// Write a single character out to the screen
void monitor_put ( char c );

//
void monitor_write_hex ( u32int n );

//
void monitor_write_dec ( u32int n );


#endif  // MONITOR_H
