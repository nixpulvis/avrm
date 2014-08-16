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

  // Wait for the nRF24L01p to be ready.
  _delay_us(nRF24L01p_TIMING_INITIAL_US);

  // Initialize the nRF24L01p.
  nRF24L01p_init();

  // Set TX mode.
  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_TX);

  // Enable the nRF24L01p.
  nRF24L01p_enable();

  _delay_ms(3000);

  while (1)
  {
    byte payload[nRF24L01p_PAYLOAD_WIDTH] = "Hello World!";
    nRF24L01p_tx_fifo_write(payload, nRF24L01p_PAYLOAD_WIDTH);
    _delay_ms(1000);
  }

  return 0;
}