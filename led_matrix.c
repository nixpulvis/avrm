#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"
#include "max7221.h"

#define MATRIX_SIZE 8
#define MAX_GENERATION MATRIX_SIZE * MATRIX_SIZE
#define GENERATION_TIME 500

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
// new_matrix - The matrix representing the game of life after one generation.
// matrix - The original matrix representing the game of life.
//
void evolve(bool new_matrix[MATRIX_SIZE][MATRIX_SIZE],
            bool matrix[MATRIX_SIZE][MATRIX_SIZE]);

// neighbors
// Returns the number of alive neighbors for a given cell in the game.
//
// matrix - The matrix representing the game of life.
// x - The x coordinate of the cell to check.
// y - The y coordinate of the cell to check.
//
byte neighbors(bool matrix[MATRIX_SIZE][MATRIX_SIZE], byte x, byte y);

int main(void)
{
  // Setup the MAX7221.
  MAX7221_init();

  // The display matrix.
  bool matrix[MATRIX_SIZE][MATRIX_SIZE];
  memset(matrix, 0, sizeof(matrix));

  // The matrix for building the next generation.
  bool new_matrix[MATRIX_SIZE][MATRIX_SIZE];

  // Randomize the initial game.
  randomize(matrix);

  unsigned int generation = 0;

  // Run the game.
  while(1)
  {
    // Display the matrix.
    MAX7221_display(matrix);

    // Zero out new matrix.
    memset(new_matrix, 0, sizeof(new_matrix));

    // Evolve the game into new_matrix.
    evolve(new_matrix, matrix);

    // Compare the two generations.
    bool eq = 1;
    for (int x = 0; x < MATRIX_SIZE; x++)
    for (int y = 0; y < MATRIX_SIZE; y++)
      eq &= matrix[x][y] == new_matrix[x][y];

    // Restart the game if the two generations are the same or
    // we've hit the MAX_GENERATIONth generation.
    if (eq || generation > MAX_GENERATION)
    {
      // Wait 2 seconds to indicate a final state.
      _delay_ms(2000);

      // Display a screen wipe.
      MAX7221_wipe();

      // Make a new random seed matrix.
      randomize(matrix);

      // Reset the generation.
      generation = 0;
    }
    else
    {
      // Copy new_matrix into matrix.
      for (int x = 0; x < MATRIX_SIZE; x++)
      for (int y = 0; y < MATRIX_SIZE; y++)
        matrix[x][y] = new_matrix[x][y];

      // Increment the generation.
      generation++;
    }

    // Wait GENERATION_TIME.
    _delay_ms(GENERATION_TIME);
  }

  return 0;
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
void evolve(bool new_matrix[MATRIX_SIZE][MATRIX_SIZE],
            bool matrix[MATRIX_SIZE][MATRIX_SIZE])
{
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
}

//
// neighbors implementation.
//
byte neighbors(bool matrix[MATRIX_SIZE][MATRIX_SIZE], byte x, byte y)
{
  // The counter for number of neighbors.
  byte count = 0;

  for (char dx = -1; dx <= 1; dx++)
  for (char dy = -1; dy <= 1; dy++)
  {
    // Check all cells within 1 of the (x, y)th cell, except itself.
    if (dx || dy)
    {
      // Get the (x, y) of the neighboring cell wrapping around the
      // matrix when out of bounds. This creates a toroidal matrix.
      byte _x = (byte) (x + dx) % MATRIX_SIZE;
      byte _y = (byte) (y + dy) % MATRIX_SIZE;
      // Increment the neighbor count when the cell is on.
      if (matrix[_x][_y])
        count++;
    }
  }
  return count;
}
