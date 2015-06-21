#include "spi.h"

//
// spi_init implementation.
//
void spi_init(byte config)
{
  if ((config >> MSTR) & 0x01)
    DDRB = (1 << DDB5) | (1 << DDB3) | (1 << DDB2);
  else
    DDRB = (1 << DDB4);

  SPCR = config | (1 << SPE);

  if ((config >> SPIE) & 0x01)
    sei();
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
