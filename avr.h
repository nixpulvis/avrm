#ifndef __AVR_H__
#define __AVR_H__

#include <avr/io.h>

// Basic Functions

// SPI Functions

void spi_begin(void);
unsigned char spi_transfer(unsigned char data);

#endif