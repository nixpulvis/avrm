#ifndef __AVR_H__
#define __AVR_H__

#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

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

// UART Functions
/////////////////

// uart_init
// Initialize the UART bus. The bus is bound to STDIN and STDOUT, so
// access to the UART is done through the standard stdio functions.
//
void uart_init(void);


// SPI Functions
////////////////

// spi_init
// Initialize the SPI bus. TODO: Add configuration options,
// and constants.
//
void spi_init(void);

// spi_start
// Pull the SS low to indicate we're using the bus. This
// function must be executed before `spi_transfer`.
//
void spi_start(void);

// spi_end
// Pull the SS high to indicate we are done using the bus.
// This should be done after using `spi_transfer`.
//
void spi_end(void);

// spi_transfer
// Send one and receive one byte of data over the SPI bus.
//
// data - The byte to send.
//
// Returns the data received.
//
byte spi_transfer(byte data);

#endif
