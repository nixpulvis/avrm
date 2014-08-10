#include "avr.h"

/* SPI Functions */

void spi_begin(void)
{
  /* SS is output. */
  PORTB |= _BV(PORTB2);
  DDRB |= _BV(DDB2);

  // Set MOSI, SCK as Output
  DDRB |= _BV(DDB3);
  DDRB |= _BV(DDB5);

  // Enable SPI, Set as Master
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);
}

unsigned char spi_transfer(unsigned char data)
{
  // Load data into the buffer
  SPDR = data;

  // Wait until transmission complete
  while (!(SPSR & _BV(SPIF)));

  // Return received data
  return SPDR;
}
