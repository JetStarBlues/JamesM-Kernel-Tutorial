// Defines typedefs and some global functions

#ifndef COMMON_H

#define COMMON_H

// Some nice typedefs to standardize sizes across platforms
// These typedefs are written for 32-bit x86
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;


// Write a byte out to the specified port
void   outb ( u16int port, u8int value );

// Read a byte from the specified port
u8int  inb  ( u16int port );

// Read a short from the specified port
u16int inw  ( u16int port );

/*
// Copy len bytes from src to dest
void memcpy ( u8int *dest, const u8int *src, u32int len );

// Write len copies of val into dest?
void memset ( u8int *dest, u8int val, u32int len );

// Compare two strings
int strcmp ( char *str1, char *str2 );

// Copy the NULL-terminated string src into dest, and return dest
char *strcpy ( char *dest, const char *src );

// Concatenate the NULL-terminated string src onto the end of dest, and return dest
char *strcat ( char *dest, const char *src );
*/

#endif  // COMMON_H
