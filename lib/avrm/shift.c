#include "shift.h"

void shift(int data_pin, int clock_pin, uint8_t n) {
    for (int i = 0; i < 8; i++) {
        // Write the current bit to the SER.
        if (n & _BV(7)) {
            pin_out(data_pin, TRUE);
        } else {
            pin_out(data_pin, FALSE);
        }

        // Toggle the SRCLK.
        pin_out(clock_pin, TRUE);
        pin_out(clock_pin, FALSE);

        // Advance the value to shift out.
        n <<= 1;
    }
}

void shift_latch(int data_pin, int clock_pin, int latch_pin, uint8_t n) {
    // Begin the shift by latching RCLK (logic LOW).
    pin_out(latch_pin, FALSE);
    // Shift out the value of `i`.
    shift(data_pin, clock_pin, n);
    // End the shift be releasing RCLK (logic HIGH).
    pin_out(latch_pin, TRUE);
}
