#include "max7221.h"

//
// MAX7221_init implementation.
//
void MAX7221_init(byte options)
{
  // Setup SPI communication for the MAX7221.
  spi_init();

  if (options == MAX7221_INIT_SANE)
  {
    MAX7221_set_scan_limit(7);
    MAX7221_set_intensity(0x0F);
    MAX7221_set_power(TRUE);
    MAX7221_clear();
  }
}


// Configuration Functions
//////////////////////////


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


// Display Functions
////////////////////


//
// MAX7221_display_matrix implementation.
//
int MAX7221_display_matrix(bool matrix[MAX7221_SIZE][MAX7221_SIZE])
{
  int ret = 0;
  for (byte y = 0; y < MAX7221_SIZE; y++)
    ret |= MAX7221_display_vector(y, matrix[y]);

  return ret;
}


//
// MAX7221_display_vector implementation.
//
int MAX7221_display_vector(byte row, bool vector[MAX7221_SIZE])
{
  // Create a byte where it's bits are equivalent to the contiguous
  // values of the vector.
  byte data = 0;
  for (byte x = 0; x < MAX7221_SIZE; x++)
    data |= (vector[x] << x);

  // Display the byte.
  return MAX7221_display_byte(row, data);
}


//
// MAX7221_display_vector implementation.
//
int MAX7221_display_byte(byte row, byte value)
{
  if (0 > row || row > 7)
    return -1;

  // Display the value.
  MAX7221_set_register(row + 1, value);

  return 0;
}


//
// MAX7221_display_bcd_digit implementation.
//
int MAX7221_display_bcd_digit(byte digit, byte value)
{
  if (0 > digit || digit > 7)
    return -1;

  // TODO: I'm pretty sure this logic is correct, but I'm on
  // an airplane and who knows what my brain is doing.
  if ((0 > value || value > 9) &&
      (value != MAX7221_BCD_MINUS ||
       value != MAX7221_BCD_E ||
       value != MAX7221_BCD_H ||
       value != MAX7221_BCD_L ||
       value != MAX7221_BCD_P ||
       value != MAX7221_BCD_BLANK))
    return -1;

  // Display the value.
  MAX7221_set_register(digit + 1, value);

  return 0;
}


// TODO: Write MAX7221_display_bcd_int.
// TODO: Write MAX7221_display_bcd_float.


//
// MAX7221_clear implementation.
//
void MAX7221_clear(void)
{
  MAX7221_set_register(0x01, 0x00);
  MAX7221_set_register(0x02, 0x00);
  MAX7221_set_register(0x03, 0x00);
  MAX7221_set_register(0x04, 0x00);
  MAX7221_set_register(0x05, 0x00);
  MAX7221_set_register(0x06, 0x00);
  MAX7221_set_register(0x07, 0x00);
  MAX7221_set_register(0x08, 0x00);
}


// Helper Functions
///////////////////


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
