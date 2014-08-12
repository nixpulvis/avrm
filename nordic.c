/* Here in lies a concerted and complete effort to get my Nordic
 * nRF24L01+ working. I've had this thing collecting dust for
 * about a year, and it's time to buckle down and figure it out.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"

typedef unsigned char byte;

void nRF24L01_setup(void);

void main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_begin();

  // Start up the SPI bus.
  spi_begin();

  // Setup the nRF24L01.
  nRF24L01_setup();
}

void nRF24L01_setup(void)
{
  // Activate command.
  PORTB &= ~_BV(PORTB2);
  spi_transfer(0b01010000);
  spi_transfer(0x73);
  PORTB |= _BV(PORTB2);

  // Set CONFIG register to 00001110 (0x0E)
  PORTB &= ~_BV(PORTB2);
  spi_transfer(0b00100000);
  spi_transfer(0b00001110);
  PORTB |= _BV(PORTB2);

  // Get the status of register 00000.
  PORTB &= ~_BV(PORTB2);
  spi_transfer(0b00000000);
  byte status = spi_transfer(0b11111111);
  PORTB |= _BV(PORTB2);

  printf("%X\n", status);

  // Pull CE low.
  PORTB |= _BV(PORTB0);
}
