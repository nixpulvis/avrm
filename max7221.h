#ifndef __MAX7221_H__
#define __MAX7221_H__

#define MAX7221_SIZE 8

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "avr.h"

// MAX7221_wipe
// Flashes the display.
//
void MAX7221_wipe(void);

// MAX7221_display
// Write the matrix to the MAX7221, FALSE representing 0 (LED off),
// TRUE representing 1 (LED on).
//
// matrix - The matrix representing the game of life.
//
void MAX7221_display(bool matrix[MAX7221_SIZE][MAX7221_SIZE]);

// MAX7221_init
// Setup the MAX7221, running a 1 second test and clearing
// the display registers.
//
void MAX7221_init(void);

// MAX7221_test
// Test the MAX7221 using it's internal test register.
// This will set all display bits to on at full brightness
// temporarily.
//
void MAX7221_test(void);

// MAX7221_send
// Set register address to a value on the MAX7221.
//
// address - The address of the register to write to.
// value - The value to write to the register.
//
void MAX7221_send(byte address, byte value);

#endif