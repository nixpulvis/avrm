#include <avr/io.h>
#include <util/delay.h>

void spi_begin(void);
unsigned char spi_transfer(unsigned char data);
void MAX7221_send(unsigned char address, unsigned char value);

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

  while(1)
  {
    MAX7221_send(0x01, random() % 256);
    MAX7221_send(0x02, random() % 256);
    MAX7221_send(0x03, random() % 256);
    MAX7221_send(0x04, random() % 256);
    MAX7221_send(0x05, random() % 256);
    MAX7221_send(0x06, random() % 256);
    MAX7221_send(0x07, random() % 256);
    MAX7221_send(0x08, random() % 256);
    _delay_ms(1000);
  }
}

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

void MAX7221_send(unsigned char address, unsigned char value)
{
  PORTB &= ~_BV(PORTB2);
  spi_transfer(address & 0xF);
  spi_transfer(value);
  PORTB |= _BV(PORTB2);
}
