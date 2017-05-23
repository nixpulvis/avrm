#include <avr/io.h>

int main(void)
{
  DDRD |= _BV(DDD7);
  DDRB |= _BV(DDB0);

  DDRD |= _BV(DDB4);
  DDRD |= ~_BV(DDB5);
  PORTD |= _BV(PORTD5);

  while(1) {
    if (PIND & _BV(DDD5)) {
      PORTD &= ~_BV(DDD7);
    } else {
      PORTD |= _BV(DDD7);
    }
  }

  return 0;
}
