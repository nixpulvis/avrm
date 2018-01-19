#ifndef __AVR_SHIFT_H__
#define __AVR_SHIFT_H__

#include <stdint.h>
#include <avr/io.h>
#include "../avrm.h"
#include "../avrm/pin.h"

/// ShiftConfig
/// Pin mapping for data and a clock.
typedef struct {
    int data_pin;
    int clock_pin;
} ShiftConfig;

/// ShiftLatchConfig
/// Pin mapping for data, clock and a latch.
typedef struct {
    int data_pin;
    int clock_pin;
    int latch_pin;
} ShiftLatchConfig;

/// shift_init
/// Set the configured pins to OUTPUTs.
///
/// # Example
///
/// ```
/// // Shifting with data pin 8, and clock pin 9.
/// ShiftConfig config = { 8, 9 };
/// shift_init(config);
/// ```
void shift_init(ShiftConfig);

/// shift_byte
/// Bit bangs a byte via the given data/clock interface.
///
/// # Example
///
/// ```
/// // Setup the shift.
/// ShiftConfig config = { 8, 9 };
/// shift_init(config);
///
/// // Shift the decimal, 42. Remember, this will not latch if your hardware
/// // requires it.
/// shift_byte(config, 42);
/// ```
void shift_byte(ShiftConfig, byte);

/// shift
/// Shift a buffer of the given length to the given data/clock interface.
///
/// # Example
///
/// ```
/// // Setup the shift.
/// ShiftConfig config = { 8, 9 };
/// shift_init(config);
///
/// // Create a buffer.
/// byte buff[5] = { 1, 1, 2, 3, 5 };
///
/// // Shift the contents of `buff`.
/// shift(config, buff, 5);
/// ```
void shift(ShiftConfig, byte*, unsigned int);

/// shift_latch_init
/// Set the configured pins to OUTPUTs.
///
/// # Example
///
/// ```
/// // Shifting with data pin 8, pin 9, and latching with pin 10.
/// ShiftLatchConfig config = { 8, 9, 10 };
/// shift_latch_init(config);
/// ```
void shift_latch_init(ShiftLatchConfig);

/// shift_latch
/// Shift a buffer of the given length to the given data/clock interface,
/// and latch the output.
///
/// # Example
///
/// ```
/// // Setup the shift/latch.
/// ShiftLatchConfig config = { 8, 9, 10 };
/// shift_latch_init(config);
///
/// // Create a buffer.
/// byte buff[5] = { 1, 1, 2, 3, 5 };
///
/// // Shift and latch the contents of `buff`.
/// shift_latch(config, buff, 5);
/// ```
void shift_latch(ShiftLatchConfig, byte*, unsigned int);

#endif
