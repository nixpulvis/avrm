#include "port.h"

void pin_out(int pin, bool value) {
    if (pin <= 7) {
        if (value) {
            PORTD |= _BV(pin);
        } else {
            PORTD &= ~_BV(pin);
        }
    } else if (pin <= 13) {
        if (value) {
            PORTB |= _BV(pin - 8);
        } else {
            PORTB &= ~_BV(pin - 8);
        }
    }
}

