#include <avr/io.h>

int main(void)
{
  // Set pin 5 of PORTB for output.
  DDRB |= _BV(DDB5);

  // Set pin 2 of PORTB for output.
  DDRB |= _BV(DDB2);

  // Set pin 1 of PORTB for input.
  DDRB &= ~_BV(DDB1);

  // Set pin 1 pull up.
  PORTB |= _BV(PORTB1);

  while(1) {
    // If pin 1 is high, the button is not pressed.
    if (PINB & _BV(PORTB1)) {
      // Set pin 5 low to turn led off.
      PORTB &= ~_BV(PORTB5);
    } else {
      // Set pin 5 high to turn led on.
      PORTB |= _BV(PORTB5);
    }
  }

  return 0;
}
