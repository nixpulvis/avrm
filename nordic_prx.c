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

  // Set RX mode.
  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_RX);

  // Enable the nRF24L01p.
  nRF24L01p_enable();

  while (1)
  {
    nRF24L01p_status_fetch();
    if (nRF24L01p_status_rx_ready())
    {
      byte payload[nRF24L01p_PAYLOAD_WIDTH];
      nRF24L01p_rx_fifo_read(payload, nRF24L01p_PAYLOAD_WIDTH);
      printf("READ: %s\n", payload);
      nRF24L01p_disable();
      nRF24L01p_status_rx_ready_clear();
      nRF24L01p_enable();
    }
  }

  return 0;
}
