#include "max7221.h"

//
// MAX7221_wipe implementation.
//
void MAX7221_wipe(void)
{
  // Iterate the rows.
  for (int y = 0; y < MAX7221_SIZE; y++)
  {
    // Set all cells in this row up to MAX7221_SIZE to on.
    MAX7221_send(y + 1, pow(2, MAX7221_SIZE) - 1);
    // Delay briefly to create a wipe effect.
    _delay_ms(50);
  }

  // Iterate the rows, again.
  for (int y = 0; y < MAX7221_SIZE; y++)
  {
    // Set all cells in this row to off.
    MAX7221_send(y + 1, 0);
    // Delay briefly to create a wipe effect.
    _delay_ms(50);
  }
}


//
// MAX7221_display implementation.
//
void MAX7221_display(bool matrix[MAX7221_SIZE][MAX7221_SIZE])
{
  // Iterate the rows.
  for (byte y = 0; y < MAX7221_SIZE; y++)
  {
    // Create a byte where it's bits are equivalent to the contiguous
    // values of the columns in this row.
    byte data = 0;
    for (byte x = 0; x < MAX7221_SIZE; x++)
      data |= (matrix[x][y] << x);

    // Send the row to the MAX7221 which is indexed starting at 1.
    MAX7221_send(y + 1, data);
  }
}


//
// MAX7221_init implementation.
//
void MAX7221_init(void)
{
  // Setup SPI communication for the MAX7221.
  spi_init();

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
}


//
// MAX7221_test implementation.
//
void MAX7221_test(void)
{
  // Test for 1 second.
  MAX7221_send(0x0F, 0x01);
  _delay_ms(1000);
  MAX7221_send(0x0F, 0x00);
}


//
// MAX7221_send implementation.
//
void MAX7221_send(byte address, byte value)
{
  spi_start();
  spi_transfer(address & 0xF);
  spi_transfer(value);
  spi_end();
}
