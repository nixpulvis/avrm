#include <avrm.h>
#include <avrm/pin.h>

#define LED_PIN 13

int main(void) {
    pin_mode(LED_PIN, OUTPUT);

    for (;;) {
        pin_out(LED_PIN, TRUE);
        delay_ms(250);
        pin_out(LED_PIN, FALSE);
        delay_ms(250);
    }

    return 0;
}
