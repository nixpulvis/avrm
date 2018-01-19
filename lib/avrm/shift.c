#include "shift.h"

void shift_init(ShiftConfig config) {
    pin_mode(config.data_pin, OUTPUT);
    pin_mode(config.clock_pin, OUTPUT);
}

void shift_byte(ShiftConfig config, byte b) {
    for (int i = 0; i < 8; i++) {
        // Write the current bit to the SER.
        if (b & _BV(7)) {
            pin_out(config.data_pin, TRUE);
        } else {
            pin_out(config.data_pin, FALSE);
        }

        // Toggle the SRCLK.
        pin_out(config.clock_pin, TRUE);
        pin_out(config.clock_pin, FALSE);

        // Advance the value to shift out.
        b <<= 1;
    }
}

void shift(ShiftConfig config, byte* buff, unsigned int n) {
    // Shift every byte, up to n bytes.
    for (int i = 0; i < n; i++) {
        shift_byte(config, buff[i]);
    }
}

void shift_latch_init(ShiftLatchConfig config) {
    pin_mode(config.data_pin, OUTPUT);
    pin_mode(config.clock_pin, OUTPUT);
    pin_mode(config.latch_pin, OUTPUT);
}

void shift_latch(ShiftLatchConfig config, byte* buff, unsigned int n) {
    // Create a config for the underlying shifts.
    ShiftConfig cfg = { config.data_pin, config.clock_pin };

    // Begin the shift by latching RCLK (logic LOW).
    pin_out(config.latch_pin, FALSE);

    // Shift out the value of `i`.
    shift(cfg, buff, n);

    // End the shift be releasing RCLK (logic HIGH).
    pin_out(config.latch_pin, TRUE);
}

