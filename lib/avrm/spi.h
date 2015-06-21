#ifndef __AVR_SPI_H__
#define __AVR_SPI_H__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "../avrm.h"

#define SPI_INT            (0x01 << SPIE)
#define SPI_DORD_MSB       (0x00 << DORD)
#define SPI_DORD_LSB       (0x01 << DORD)
#define SPI_SLAVE          (0x00 << MSTR)
#define SPI_MASTER         (0x01 << MSTR)
#define SPI_CPOL_IDLE_LOW  (0x00 << CPOL)
#define SPI_CPOL_IDLE_HIGH (0x01 << CPOL)
#define SPI_CPHA_TRAILING  (0x00 << CPHA)
#define SPI_CPHA_LEADING   (0x01 << CPHA)
#define SPI_SPR_4           0x00
#define SPI_SPR_16          0x01
#define SPI_SPR_64          0x02
#define SPI_SPR_128         0x03

// spi_init
// Initialize the SPI bus.
//
// config - Value for the SPCR register, constants are provided above
//          to make setting this more convenient. The SPE bit will always
//          be set to 1.
void spi_init(byte config);

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