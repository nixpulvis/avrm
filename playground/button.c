#include <avrm/pin.h>

int main(void) {
    // Setup the external LED.
    pin_mode(7, OUTPUT);
    pin_mode(8, OUTPUT);

    // Setup the input switch. Moving the switch to connect 4 and 5 will
    // pull the value of 5 low, it's normally held high by the pull-up.
    pin_mode(4, OUTPUT);
    pin_mode(5, INPUT);
    pin_out(5, TRUE);

    for (;;) {
        // If the switch is "off", the LED is on.
        if (pin_in(5)) {
            pin_out(7, FALSE);
        } else {
            pin_out(7, TRUE);
        }
    }

    return 0;
}
