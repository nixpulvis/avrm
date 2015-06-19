#include "spi.h"

//
// spi_init implementation.
//
void spi_init(void)
{
  // SS is output.
  PORTB |= _BV(PORTB2);
  DDRB |= _BV(DDB2);

  // Set MOSI, SCK as output.
  DDRB |= _BV(DDB3);
  DDRB |= _BV(DDB5);

  // Enable SPI, and set as master.
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);
}


//
// spi_start implementation.
//
void spi_start(void)
{
  PORTB &= ~_BV(PORTB2);
}


//
// spi_end implementation.
//
void spi_end(void)
{
  PORTB |= _BV(PORTB2);
}


//
// spi_transfer implementation.
//
byte spi_transfer(byte data)
{
  // Load data into the buffer.
  SPDR = data;

  // Wait until transmission complete.
  while (!(SPSR & _BV(SPIF)));

  // Return received data.
  return SPDR;
}
