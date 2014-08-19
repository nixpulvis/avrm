#include "max7221.h"

//
// MAX7221_init implementation.
//
void MAX7221_init(void)
{
  // Setup SPI communication for the MAX7221.
  spi_init();

  // Set scan limiter to display all registers.
  MAX7221_set_scan_limit(7);

  // Set display intensity (0-F).
  MAX7221_set_intensity(0x0F);

  // Turn on display.
  MAX7221_set_power(TRUE);

  // Clear the display.
  MAX7221_set_register(0x01, 0x00);
  MAX7221_set_register(0x02, 0x00);
  MAX7221_set_register(0x03, 0x00);
  MAX7221_set_register(0x04, 0x00);
  MAX7221_set_register(0x05, 0x00);
  MAX7221_set_register(0x06, 0x00);
  MAX7221_set_register(0x07, 0x00);
  MAX7221_set_register(0x08, 0x00);
}


//
// MAX7221_set_decode_mode
//
int MAX7221_set_decode_mode(byte value)
{
  MAX7221_set_register(MAX7221_REGISTER_DECODE_MODE, value);
  return 0;
}


//
// MAX7221_set_intensity
//
int MAX7221_set_intensity(byte value)
{
  MAX7221_set_register(MAX7221_REGISTER_INTENSITY, value);
  return 0;
}


//
// MAX7221_set_scan_limit
//
int MAX7221_set_scan_limit(byte value)
{
  MAX7221_set_register(MAX7221_REGISTER_SCAN_LIMIT, value);
  return 0;
}


//
// MAX7221_set_power
//
int MAX7221_set_power(bool value)
{
  MAX7221_set_register(MAX7221_REGISTER_SHUTDOWN, value ? 0xFF : 0x00);
  return 0;
}


//
// MAX7221_set_display_test
//
int MAX7221_set_display_test(bool value)
{
  MAX7221_set_register(MAX7221_REGISTER_DISPLAY_TEST, value ? 0xFF : 0x00);
  return 0;
}


//
// MAX7221_wipe implementation.
//
void MAX7221_wipe(void)
{
  // Iterate the rows.
  for (int y = 0; y < MAX7221_SIZE; y++)
  {
    // Set all cells in this row up to MAX7221_SIZE to on.
    MAX7221_set_register(y + 1, pow(2, MAX7221_SIZE) - 1);
    // Delay briefly to create a wipe effect.
    _delay_ms(50);
  }

  // Iterate the rows, again.
  for (int y = 0; y < MAX7221_SIZE; y++)
  {
    // Set all cells in this row to off.
    MAX7221_set_register(y + 1, 0);
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
    MAX7221_set_register(y + 1, data);
  }
}


//
// MAX7221_set_register implementation.
//
void MAX7221_set_register(byte address, byte value)
{
  spi_start();
  spi_transfer(address & 0xF);
  spi_transfer(value);
  spi_end();
}
