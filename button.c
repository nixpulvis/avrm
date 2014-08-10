#include <avr/io.h>

int main(void)
{
  /* set pin 5 of PORTB for output. */
  DDRB |= _BV(DDB5);

  /* set pin 2 of PORTB for output. */
  DDRB |= _BV(DDB2);

  /* set pin 1 of PORTB for input. */
  DDRB &= ~_BV(DDB1);

  /* set pin 1 pull up. */
  PORTB |= _BV(PORTB1);

  while(1) {
    /* if pin 1 is high, the button is not pressed. */
    if (PINB & _BV(PORTB1)) {
      /* set pin 5 low to turn led off. */
      PORTB &= ~_BV(PORTB5);
    } else {
      /* set pin 5 high to turn led on. */
      PORTB |= _BV(PORTB5);
    }
  }

  return 0;
}
