#ifndef __MAX7221_H__
#define __MAX7221_H__

// Registers.

#define MAX7221_REGISTER_NOOP         0x00
#define MAX7221_REGISTER_DIGIT_0      0x01
#define MAX7221_REGISTER_DIGIT_1      0x02
#define MAX7221_REGISTER_DIGIT_2      0x03
#define MAX7221_REGISTER_DIGIT_3      0x04
#define MAX7221_REGISTER_DIGIT_4      0x05
#define MAX7221_REGISTER_DIGIT_5      0x06
#define MAX7221_REGISTER_DIGIT_6      0x07
#define MAX7221_REGISTER_DIGIT_7      0x08
#define MAX7221_REGISTER_DECODE_MODE  0x09
#define MAX7221_REGISTER_INTENSITY    0x0A
#define MAX7221_REGISTER_SCAN_LIMIT   0x0B
#define MAX7221_REGISTER_SHUTDOWN     0x0C
#define MAX7221_REGISTER_DISPLAY_TEST 0x0F

// BCD Code B Characters.

#define MAX7221_BCD_MINUS 0x0A
#define MAX7221_BCD_E     0x0B
#define MAX7221_BCD_H     0x0C
#define MAX7221_BCD_L     0x0D
#define MAX7221_BCD_P     0x0E
#define MAX7221_BCD_BLANK 0x0F

//

#define MAX7221_SIZE 8

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "avr.h"

// MAX7221_init
// Setup the MAX7221, running a 1 second test and clearing
// the display registers.
//
void MAX7221_init(void);

//

// MAX7221_set_decode_mode
// Set specified digits to be BCD code B decoded. This is
// useful when used with 7 segment displays as it allows for
// values (0-9, E, H, L, P, and -). For more information on
// the decoding, see MAX7221_display.
//
// Disabling the decoder allows for individual control over
// each segment line of the chip.
//
// value - A byte with bits corresponding to the digits for
//         which the decoder should be active.
//
// Returns 0 when successful, and -1 otherwise.
//
int MAX7221_set_decode_mode(byte value);

// MAX7221_set_intensity
// Set the display intensity for the whole device. This effects
// the perceived brightness of device.
//
// value - A byte between 0x00 and 0x0F from lowest intensity
//         to highest.
//
// Returns 0 when successful, and -1 otherwise.
//
int MAX7221_set_intensity(byte value);

// MAX7221_set_scan_limit
// Sets the number of digits to scan across. Changing this value
// affects the timing of each pass, and therefor effects the
// perceived brightness.
//
// value - A number from 0 to 7 indicating the last digit to
//         scan to.
//
int MAX7221_set_scan_limit(byte value);

// MAX7221_set_power
// Set the power of the device to either Shutdown Mode (FALSE)
// or Normal Operation (TRUE).
//
// value - Either TRUE or FALSE.
//
// Returns 0 when successful, and -1 otherwise.
//
int MAX7221_set_power(bool value);

// MAX7221_test
// When enabled the device internally activates all digits at full
// intensity, not effecting the settings of other registers.
//
// value - TRUE or FALSE
//
// Returns 0 when successful, and -1 otherwise.
//
int MAX7221_set_display_test(bool value);

//

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

//

// MAX7221_set_register
// Set register address to a value on the MAX7221.
//
// address - The address of the register to write to.
// value - The value to write to the register.
//
void MAX7221_set_register(byte address, byte value);

#endif