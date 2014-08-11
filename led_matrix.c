#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"

#define MATRIX_SIZE 8
#define MAX_GENERATION MATRIX_SIZE * MATRIX_SIZE

// Boolean type.
typedef enum { FALSE, TRUE } bool;

// Byte type.
typedef unsigned char byte;

// randomize
// Set random bits in the matrix.
//
// matrix - The matrix representing the game of life.
//
void randomize(bool matrix[MATRIX_SIZE][MATRIX_SIZE]);

// evolve
// Mutate the matrix into it's next state following Conway's
// game of life's rules.
//
// matrix - The matrix representing the game of life.
// generation - The generation of the current game.
//
// Returns true when it generated the next generation, false when it
// reset.
//
bool evolve(bool matrix[MATRIX_SIZE][MATRIX_SIZE], unsigned int generation);

// neighbors
// Returns the number of alive neighbors for a given cell in the game.
//
// matrix - The matrix representing the game of life.
// x - The x coordinate of the cell to check.
// y - The y coordinate of the cell to check.
//
byte neighbors(bool matrix[MATRIX_SIZE][MATRIX_SIZE], byte x, byte y);

// MAX7221_display
// Write the matrix to the MAX7221, FALSE representing 0 (LED off),
// TRUE representing 1 (LED on).
//
// matrix - The matrix representing the game of life.
//
void MAX7221_display(bool matrix[MATRIX_SIZE][MATRIX_SIZE]);

// MAX7221_setup
// Setup the MAX7221, running a 1 second test and clearing
// the display registers.
//
void MAX7221_setup(void);

// MAX7221_send
// Set register address to a value on the MAX7221.
//
// address - The address of the register to write to.
// value - The value to write to the register.
//
void MAX7221_send(byte address, byte value);

void main(void)
{
  // Setup the MAX7221.
  MAX7221_setup();

  // The display matrix.
  bool matrix[MATRIX_SIZE][MATRIX_SIZE];
  memset(matrix, 0, sizeof(matrix));

  // Randomize the initial game.
  randomize(matrix);

  unsigned int generation = 0;

  // Run the game.
  while(1)
  {
    // Display the matrix.
    MAX7221_display(matrix);
    // Evolve the game.
    if (evolve(matrix, generation++))
      generation++;
    else
      generation = 0;
    // Wait half a second.
    _delay_ms(500);
  }
}

//
// randomize implementation.
//
void randomize(bool matrix[MATRIX_SIZE][MATRIX_SIZE])
{
  for (byte x = 0; x < MATRIX_SIZE; x++)
  for (byte y = 0; y < MATRIX_SIZE; y++)
    matrix[y][x] = random() % 2;
}

//
// evolve implementation.
//
bool evolve(bool matrix[MATRIX_SIZE][MATRIX_SIZE], unsigned int generation)
{
  // Allocate a new empty game matrix.
  bool new_matrix[MATRIX_SIZE][MATRIX_SIZE];
  memset(new_matrix, 0, sizeof(new_matrix));

  for (byte x = 0; x < MATRIX_SIZE; x++)
  for (byte y = 0; y < MATRIX_SIZE; y++)
  {
    // Count the number of neighbors of this cell in the original matrix.
    byte count = neighbors(matrix, x, y);

    // Alive cells.
    if (matrix[x][y])
    {
      // Any live cell with fewer than two live neighbors dies,
      // as if caused by under-population.
      if (count < 2)
        new_matrix[x][y] = FALSE;

      // Any live cell with two or three live neighbors lives on
      // to the next generation.
      else if ((count == 2) || (count == 3))
        new_matrix[x][y] = TRUE;

      // Any live cell with more than three live neighbors dies,
      // as if by overcrowding.
      else
        new_matrix[x][y] = FALSE;
    }
    // Dead cells.
    else
    {
      // Any dead cell with exactly three live neighbors becomes a
      // live cell, as if by reproduction.
      if (count == 3)
        new_matrix[x][y] = TRUE;

      // Any dead cell with not exactly three live neighbors remains
      // dead.
      else
        new_matrix[x][y] = FALSE;
    }
  }

  // Compare the two generations.
  bool eq = 1;
  for (int x = 0; x < MATRIX_SIZE; x++)
  for (int y = 0; y < MATRIX_SIZE; y++)
    eq &= matrix[x][y] == new_matrix[x][y];

  // Restart the game if the two generations are the same or
  // we've hit the MAX_GENERATIONth generation
  if (eq || generation > MAX_GENERATION)
  {
    _delay_ms(2000);
    randomize(matrix);
    return FALSE;
  }
  // Copy new_matrix into matrix.
  else
  {
    for (int x = 0; x < MATRIX_SIZE; x++)
    for (int y = 0; y < MATRIX_SIZE; y++)
      matrix[x][y] = new_matrix[x][y];
    return TRUE;
  }
}

//
// neighbors implementation.
//
byte neighbors(bool matrix[MATRIX_SIZE][MATRIX_SIZE], byte x, byte y)
{
  byte count = 0;
  // for (byte dx = -1; dx <= 1; dx++)
  // for (byte dy = -1; dy <= 1; dy++)
  // {
  //   if ( (dx || dy) && matrix[(x + dx) % 8][(y + dy) % 8] )
  //     count++;
  // }
  for (char dx = -1; dx <= 1; dx++)
  for (char dy = -1; dy <= 1; dy++)
  {
    if (dx || dy)
    {
      byte _x = (byte) (x + dx) % MATRIX_SIZE;
      byte _y = (byte) (y + dy) % MATRIX_SIZE;
      if (matrix[_x][_y])
        count++;
    }
  }
  return count;
}

//
// MAX7221_display implementation.
//
void MAX7221_display(bool matrix[MATRIX_SIZE][MATRIX_SIZE])
{
  for (byte y = 0; y < MATRIX_SIZE; y++)
  {
    // Need to create the byte because bool[8] isn't the same as byte.
    byte data = 0;
    for (byte x = 0; x < MATRIX_SIZE; x++)
      data |= (matrix[x][y] << x);

    // Send the row to the MAX7221 which is indexed starting at 1.
    MAX7221_send(y + 1, data);
  }
}

//
// MAX7221_setup implementation.
//
void MAX7221_setup(void)
{
  // Setup SPI communication for the MAX7221.
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
}

//
// MAX7221_send implementation.
//
void MAX7221_send(byte address, byte value)
{
  PORTB &= ~_BV(PORTB2);
  spi_transfer(address & 0xF);
  spi_transfer(value);
  PORTB |= _BV(PORTB2);
}
