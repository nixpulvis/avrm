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

#define MAX7221_INIT_NONE 0
#define MAX7221_INIT_SANE 1

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "avr.h"

// MAX7221_init
// Setup the MAX7221, setting some sane defaults if passed
// MAX7221_SANE.
//
// options - Either MAX7221_INIT_NONE or MAX7221_INIT_SANE.
//
void MAX7221_init(byte options);

// Configuration Functions
//////////////////////////

// MAX7221_set_decode_mode
// Set specified digits to be BCD code B decoded. This is
// useful when used with 7 segment displays as it allows for
// values (0-9, E, H, L, P, and -). For more information on
// the decoding, see MAX7221_display_bcd_b.
//
// Disabling the decoder allows for individual control over
// each segment of the chip.
//
// value - A single byte with bits corresponding to the 8
//         digits for which the decoder should be active.
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
// value - A number from 1 to 8 indicating the number of rows to
//         scan, thus making active and addressable.
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

// Display Functions
////////////////////

// MAX7221_display_matrix
// Write a matrix to the MAX7221, FALSE representing LED off,
// TRUE representing LED on.
//
// This function will only work as expected if all the digit's
// decoders are disabled. See MAX7221_set_decode_mode.
//
// matrix - The matrix of booleans representing display state.
//
// Returns 0 if successful, -1 otherwise.
//
int MAX7221_display_matrix(bool matrix[MAX7221_SIZE][MAX7221_SIZE]);

// MAX7221_display_vector
// Display a vector to a row of the device, FALSE representing LED off,
// TRUE representing LED on.
//
// This function will only work as expected if all the digit's
// decoders are disabled. See MAX7221_set_decode_mode.
//
// row - The row (0-7) to display the given vector.
// vector - The vector of booleans representing the row's display
//          state.
//
// Returns 0 if successful, -1 otherwise.
//
int MAX7221_display_vector(byte row, bool vector[MAX7221_SIZE]);

// MAX7221_display_byte
// Display a byte to a row of the device, each bit of the byte
// representing the state of the column.
//
// This function will only work as expected if all the digit's
// decoders are disabled. See MAX7221_set_decode_mode.
//
// row - The row (0-7) to display the given vector.
// value - The byte to display.
//
// Returns 0 if successful, -1 otherwise.
//
int MAX7221_display_byte(byte row, byte value);

// MAX7221_display_bcd_b
// Display a value using the MAX7221's built in decoder.
//
// This function does not set the chip's decoder configuration,
// the user must do that before calling this function. See
// MAX7221_set_decode_mode.
//
// digit - The digit (0-7) to display the given value.
// value - One of (0-9) or
//                MAX7221_BCD_MINUS or
//                MAX7221_BCD_E or
//                MAX7221_BCD_H or
//                MAX7221_BCD_L or
//                MAX7221_BCD_P or
//                MAX7221_BCD_BLANK.
//
// Returns 0 if successful, -1 otherwise.
//
int MAX7221_display_bcd_digit(byte digit, byte value);

// MAX7221_display_bcd_int
// Display an integer. This function uses the lower indexed digits
// first. For example given the integer 245 digits 0, 1, and 2 will
// be set to 5, 4, 2 respectively. If the number is negative the digit
// imitatively proceeding the last numeric value will be set to
// MAX7221_BCD_MINUS.
//
// Range:
//
// The range of this function is integers from -10^(segments - 1) + 1
// to 10^(segments) - 1.
//
// Examples:
//
//     // Good.
//     MAX7221_display_bcd_int(1337, 8);
//     MAX7221_display_bcd_int(-20, 4);
//
//     // Bad.
//     MAX7221_display_bcd_int(300, 2);
//     MAX7221_display_bcd_int(-3, 1);
//
// value - The integer to display on the devise.
// segments - The number (1-8) of segments available to this function.
//
// Returns 0 if successful, -1 otherwise.
//
int MAX7221_display_bcd_int(int32_t value, byte segments);

// MAX7221_display_bcd_float
// Display a floating point number. This functions uses `segments`
// amount of digits for decimals, and the remaining digits for
// whole numbers, and MAX7221_BCD_MINUS. Like MAX7221_display_bcd_int
// the least significant digits are assigned to the lower digits on
// the device.
//
// Range:
//
// .5   requires 2 segments.
// -.5  requires 2 segments.
// 1.5  requires 2 segments.
// -1.5 requires 3 segments.
// 20.5 requires 3 segments.
//
// From -10^(segments - decimals - 1) to 10^(segments - decimals)
// non-inclusive.
//
// Overflow:
//
// When given a number outside of the range of this function
// the value is rounded according to the `overflow` argument.
// TODO: Info on these options.
//
// Examples:
//
//     // Good.
//     MAX7221_display_bcd_float(13.37, 2, 4, MAX7221_DONTCARE);    // 13.37
//     MAX7221_display_bcd_float(-1.5, 1, 3, MAX7221_DONTCARE);    // -1.5
//     MAX7221_display_bcd_float(-1.5, 3, 5, MAX7221_DONTCARE);    // -1.500
//     MAX7221_display_bcd_float(3.14159, 3, 4, MAX7221_DONTCARE); // 3.141
//
//     // Bad.
//     MAX7221_display_bcd_float(13.37, 2, 3, MAX7221_DONTCARE);
//     MAX7221_display_bcd_float(-1.5, 1, 2, MAX7221_DONTCARE);
//     MAX7221_display_bcd_float(-1.5, 3, 4, MAX7221_DONTCARE);
//     MAX7221_display_bcd_float(3.14159, 3, 3, MAX7221_DONTCARE);
//
// value - The floating point number to display on the device.
// decimals - The number (1-7) of decimal point digits to display.
// segments - The number (2-8) of segments available to this function.
//            This value effects the range of the values accepted by
//            the function.
// round - The floating point rounding strategy.
//
// Returns 0 if successful, -1 if rounded and you "care", -2 otherwise.
//
int MAX7221_display_bcd_float(float value, byte decimals, byte segments,
                              byte round);

// MAX7221_clear
// Turn all segments of the display off.
//
void MAX7221_clear(void);

// Helper Functions
///////////////////

// MAX7221_set_register
// Set register address to a value on the MAX7221.
//
// address - The address of the register to write to.
// value - The value to write to the register.
//
void MAX7221_set_register(byte address, byte value);

#endif