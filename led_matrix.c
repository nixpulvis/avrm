#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"

void set_bit(char x, char y, char val);
void MAX7221_send(unsigned char address, unsigned char value);

static char display_buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void main(void)
{
  /* setup SPI communication for the MAX7221. */
  spi_begin();

  // Test for 1 second.
  MAX7221_send(0x0F, 0x01);
  _delay_ms(1000);
  MAX7221_send(0x0F, 0x00);

  // Turn off decoding.
  MAX7221_send(0x09, 0x00);

  // Set scan limiter to display all registers.
  MAX7221_send(0x0B, 0x07);

  // Set display intensity (0-F).
  MAX7221_send(0x0A, 0x05);

  // Turn on display.
  MAX7221_send(0x0C, 0x01);

  // Clear the display.
  MAX7221_send(0x01, 0x00);
  MAX7221_send(0x02, 0x00);
  MAX7221_send(0x03, 0x00);
  MAX7221_send(0x04, 0x00);
  MAX7221_send(0x05, 0x00);
  MAX7221_send(0x06, 0x00);
  MAX7221_send(0x07, 0x00);
  MAX7221_send(0x08, 0x00);

  while(1)
  {
    for (int i = 0; i < 8; i++)
      MAX7221_send(i + 1, random() % 256);
      // MAX7221_send(i + 1, display_buffer[i]);

    _delay_ms(500);
  }
}

/* x and y must be in the range [0, 7]. val must be 0 or 1. */
void set_bit(char x, char y, char val)
{
  display_buffer[y] |= val << x;
}

void MAX7221_send(unsigned char address, unsigned char value)
{
  PORTB &= ~_BV(PORTB2);
  spi_transfer(address & 0xF);
  spi_transfer(value);
  PORTB |= _BV(PORTB2);
}
