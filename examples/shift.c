#include <avr/io.h>
#include <avrm.h>
#include <avrm/pin.h>
#include <avrm/shift.h>

static const ShiftLatchConfig config = { 11, 12, 8 };

int main(void) {
    // Initalize the shift/latch interface.
    shift_latch_init(config);

    for (;;) {
        // Shift out the binary from 0 to 255 each 100ms.
        for (byte i = 0; i < 256; i++) {
            byte buff[2] = { i, 255-i };
            shift_latch(config, buff, 2);
            delay_ms(100);
        }
    }

    return 0;
}

