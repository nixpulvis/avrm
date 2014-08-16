/* Here in lies a concerted and complete effort to get my Nordic
 * nRF24L01+ working. I've had this thing collecting dust for
 * about a year, and it's time to buckle down and figure it out.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lib/avr.h"
#include "lib/nRF24L01p.h"

int main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_init();

  // Set an LED.
  DDRD |= _BV(DDB6);
  DDRD |= _BV(DDB7);
  PORTD &= ~_BV(PORTB6);

  // Wait for the nRF24L01p to be ready.
  _delay_us(nRF24L01p_TIMING_INITIAL_US);

  // Initialize the nRF24L01p.
  nRF24L01p_init();

  // Set RX mode.
  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_RX);

  // Enable the nRF24L01p.
  nRF24L01p_enable();

  byte state = 0;
  while (1)
  {
    nRF24L01p_read_sync(&state, 1);
    if (state)
      PORTD |= _BV(PORTB7);
    else
      PORTD &= ~_BV(PORTB7);
    printf("%d\n", state);
  }

  return 0;
}
