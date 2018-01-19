#ifndef __AVR_PIN_H__
#define __AVR_PIN_H__

#include <avr/io.h>
#include "../avrm.h"

/// PinMode
/// Configuration for an AVR pin.
// TODO: Should we add a `INPUT_PULLUP` mode?
typedef enum {INPUT, OUTPUT} PinMode;

/// pin_mode
/// Setup a pin for `OUTPUT` or `INPUT`.
///
/// A pin in `OUTPUT` mode will either drive current at logic high, or sink
/// current at logic low. Most AVRs can safely provide +/-40mA.
///
/// A pin in `INPUT` mode (the default mode) are in a high impedance state.
/// It only takes a small amount of current to move a pin in this state from
/// reading a logic low to a logic high. Using `pin_out(pin, HIGH)` on a pin
/// in this mode will activate it's internal pullup resistor.
///
/// # Example
///
/// ```
/// // Set the LED pin to OUTPUT mode.
/// pin_mode(13, OUTPUT);
///
/// // Set pin 5 to INPUT mode.
/// pin_mode(5, INPUT);
/// ```
void pin_mode(int pin, PinMode mode);

/// pin_in
/// Read the value on a pin.
///
/// This function is currently only implemented for the ATmega168/328p.
///
/// # Example
///
/// ```
/// // Read the digital state of the 5th pin.
/// bool active = pin_in(5);
/// ```
bool pin_in(int pin);

/// pin_out
/// Sets the digital value of a pin.
///
/// This function is currently only implemented for the ATmega168/328p.
///
/// # Example
///
/// ```
/// // Set pin 0 to logic high.
/// pin_out(0, TRUE);
/// // Set pin 8 to logic low.
/// pin_out(8, FALSE);
/// // Set pin 13 (LED) to logic high (on).
/// pin_out(13, TRUE);
/// ```
void pin_out(int pin, bool value);

#endif
