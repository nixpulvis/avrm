#include "avr.h"

//
// printb implementation.
//
void printb(void const * const ptr, size_t const size)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--)
  for (j = 7; j >= 0; j--)
  {
    byte = b[i] & (1 << j);
    byte >>= j;
    printf("%u", byte);
  }
}

//
// bcd_to_dec implementation.
//
byte bcd_to_dec(byte data)
{
  // TODO: Bound checking.
  return (((data & 0xF0) >> 4) * 10) + (data & 0x0F);
}

//
// dec_to_bcd implementation.
//
byte dec_to_bcd(byte data)
{
  // TODO: Bound checking.
  return (((data / 10) << 4) & 0xF0) | (data % 10);
}
