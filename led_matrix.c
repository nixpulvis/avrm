#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"

typedef enum { FALSE, TRUE } bool;

void randomize(unsigned char *matrix);
void evolve(unsigned char *matrix);
int neighbours(unsigned char *matrix, int x, int y);
void display(unsigned char *matrix);
bool get_bit(unsigned char *matrix, int x, int y);
void set_bit(unsigned char *matrix, int x, int y, bool val);
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
  MAX7221_send(0x0A, 0x07);

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

  // The display matrix.
  unsigned char matrix[8];

  // Random seed.
  randomize(matrix);

  // Run the game.
  while(1)
  {
    // Display the matrix.
    display(matrix);
    evolve(matrix);
    _delay_ms(400);
  }
}

void randomize(unsigned char *matrix)
{
  for (int i = 0; i < 8; i++)
  {
    matrix[i] = random() % 256;
  }
}

void evolve(unsigned char *matrix)
{
  char new_matrix[8];
  memcpy(new_matrix, matrix, sizeof(unsigned char) * 8);

  for (int x = 0; x < 8; x++)
  for (int y = 0; y < 8; y++)
  {
    int count = neighbours(matrix, x, y);

    // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    if (get_bit(matrix, x, y)) {
      if (count < 2) {
        set_bit(new_matrix, x, y, FALSE);

      // Any live cell with two or three live neighbours lives on to the next generation.
      } else if ((count == 2) || (count == 3)) {
        set_bit(new_matrix, x, y, TRUE);

      // Any live cell with more than three live neighbours dies, as if by overcrowding.
      } else {
        set_bit(new_matrix, x, y, FALSE);
      }
    } else {
      // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
      if (count == 3)
      {
        set_bit(new_matrix, x, y, TRUE);
      } else {
        set_bit(new_matrix, x, y, FALSE);
      }
    }
  }

  // Break a static configuration.
  if (memcmp(matrix, new_matrix, sizeof(unsigned char) * 8) == 0)
  {
    _delay_ms(5000);
    randomize(matrix);
  } else {
    memcpy(matrix, new_matrix, sizeof(unsigned char) * 8);
  }
}

int neighbours(unsigned char *matrix, int x, int y)
{
  int count = 0;
  for (int i = -1; i <= 1; i++)
  for (int j = -1; j <= 1; j++)
  {
    if ((i || j) &&
        (x + i) < 8 &&
        (y + j) < 8 &&
        get_bit(matrix, x + i, y + j))
      count++;
  }
  return count;
}

void display(unsigned char *matrix)
{
  for (int i = 0; i < 8; i++)
    MAX7221_send(i + 1, matrix[i]);
}

/* x and y must be in the range [0, 7].
 * Result must be interpreted as 0 or 1 */
bool get_bit(unsigned char *matrix, int x, int y)
{
  return matrix[y] & 1 << x;
}

/* x and y must be in the range [0, 7]. val must be 0 or 1. */
void set_bit(unsigned char *matrix, int x, int y, bool val)
{
  matrix[y] = val ? matrix[y] | val << x : matrix[y] & ~(1 << x);
}

void MAX7221_send(unsigned char address, unsigned char value)
{
  PORTB &= ~_BV(PORTB2);
  spi_transfer(address & 0xF);
  spi_transfer(value);
  PORTB |= _BV(PORTB2);
}
