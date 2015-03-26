#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

int main(void)
{
  // Set pin 5 of PORTB for output.
  DDRB |= _BV(DDB5);

  while(1) {
    // Set pin 5 high to turn led on.
    PORTB |= _BV(PORTB5);
    _delay_ms(BLINK_DELAY_MS);

    // Set pin 5 low to turn led off.
    PORTB &= ~_BV(PORTB5);
    _delay_ms(BLINK_DELAY_MS);
  }

  return 0;
}
