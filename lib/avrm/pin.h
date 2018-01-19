#ifndef __AVR_PORT_H__
#define __AVR_PORT_H__

#include <avr/io.h>
#include <avrm.h>

void pin_in(void);

// ```
// // Set the ATmega168/328p's port D's 0'th bit.
// pin_out(0, TRUE);
// // Set the ATmega168/328p's port B's 0'th bit.
// pin_out(8, TRUE);
// // Unset the ATmega168/328p's port B's 5'th bit (aka. LED).
// pin_out(0, FALSE);
// ```
void pin_out(int pin, bool value);

#endif
