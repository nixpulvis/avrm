/* blink.c
 *
 * Flash the mini built in LED on the avrm test board.
 */
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB |= _BV(DDB5);

  while(1) {
    PORTB |= _BV(DDB5);
    _delay_ms(250);
    PORTB &= ~_BV(DDB5);
    _delay_ms(250);
  }

  return 0;
}
