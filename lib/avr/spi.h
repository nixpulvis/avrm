#ifndef __AVR_SPI_H__
#define __AVR_SPI_H__

#include <avr.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

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