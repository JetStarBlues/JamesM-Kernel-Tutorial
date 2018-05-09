#ifndef MONITOR_H
#define MONITOR_H


#include "common.h"

// Scrolls the text on the screen up by one line
void monitor_scrollUp ();

// Scrolls the text on the screen down by one line
void monitor_scrollDown ();

// Write a single character out to the screen
void monitor_put ( char c );

// Clear the screen to all black
void monitor_clear ();

// Output a null-terminated ASCII string to the monitor
void monitor_write ( char *c );

// Write a newline character
void monitor_writeln ();

//
void monitor_write_hex ( u32int n );

//
void monitor_write_dec ( u32int n );


#endif  // MONITOR_H
