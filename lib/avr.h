#ifndef __AVR_H__
#define __AVR_H__

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// Macros
/////////

#define delay_ms(ms) (_delay_ms(ms))

// Type Definitions
///////////////////

// The bool type, with supporting TRUE and FALSE constants.
#define FALSE 0
#define TRUE 1
typedef _Bool bool;

// The byte type.
typedef unsigned char byte;

// General Functions
////////////////////

// printb
// Print out the given data in it's binary representation.
// Assumes little endian.
//
void printb(void const * const ptr, size_t const size);


// bcd_to_dec
// Convert a Binary Coded Digit into it's corresponding value.
// TODO: Add support for the sign bit, allowing output range to be (-99 - 99).
//
// data - The BCD value to convert into normal decimal form.
//
// Returns the decimal value (0 - 99).
byte bcd_to_dec(byte data);

// dec_to_bcd
// Convert a value into it's corresponding Binary Coded Digit.
// TODO: Add support for the sign bit, allowing input range to be (-99 - 99).
//
// data - The decimal value (0 - 99) to convert into BCD form.
//
// Returns the BCD value.
byte dec_to_bcd(byte data);

#endif
