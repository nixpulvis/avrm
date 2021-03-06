#include "pin.h"

void pin_mode(int pin, PinMode mode) {
    if (pin <= 7) {
        if (mode == OUTPUT) {
            DDRD |= _BV(pin);
        } else {
            DDRD &= ~_BV(pin);
        }
    } else if (pin <= 13) {
        if (mode == OUTPUT) {
            DDRB |= _BV(pin - 8);
        } else {
            DDRB &= ~_BV(pin - 8);
        }
    }
}

bool pin_in(int pin) {
    if (pin <= 7) {
        return PIND & _BV(pin);
    } else if (pin <= 13) {
        return PINB & _BV(pin);
    }

    return FALSE;
}

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

