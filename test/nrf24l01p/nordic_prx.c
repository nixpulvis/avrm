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
  // TODO: Actual info for arguments.
  nRF24L01p_init(0, 0);

  // Enable pipe 0.
  nRF24L01p_config_pipe(nRF24L01p_PIPE_0, 0xA7A7A7A7A7, 32);
  nRF24L01p_config_pipe(nRF24L01p_PIPE_1, 0xD5D5D5D5D5, 32);

  // Set RX mode.
  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_RX);

  byte str0[5];
  nRF24L01p_read(str0, 5, nRF24L01p_PIPE_0);

  byte str1[50];
  nRF24L01p_read(str1, 50, nRF24L01p_PIPE_1);

  while (1)
  {
    if (nRF24L01p_read_status(nRF24L01p_PIPE_0))
    {
      printf("%s\n", str0);
      nRF24L01p_read(str0, 5, nRF24L01p_PIPE_0);
    }

    if (nRF24L01p_read_status(nRF24L01p_PIPE_1))
    {
      printf("%s\n", str1);
      nRF24L01p_read(str1, 50, nRF24L01p_PIPE_1);
    }
  }

  return 0;
}
