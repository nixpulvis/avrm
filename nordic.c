/* Here in lies a concerted and complete effort to get my Nordic
 * nRF24L01+ working. I've had this thing collecting dust for
 * about a year, and it's time to buckle down and figure it out.
 */

#define AM_RX 1

#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"
#include "nRF24L01p.h"

int main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_init();

  // Setup the nRF24L01p.
  nRF24L01p_init();

  // Power up the nRF24L01p.
  nRF24L01p_power_up();

  if (AM_RX)
  {
    // Set RX mode.
    nRF24L01p_prx();

    // Set RX address.
    unsigned long long address = nRF24L01p_DEFAULT_ADDRESS;
    spi_start();
    spi_transfer(nRF24L01p_SPI_W_REGISTER |
                 (nRF24L01p_REGISTER_TX_ADDR & nRF24L01p_SPI_RW_REGISTER_MASK));
    for (byte i = 0; i < 5; i++)
    {
      spi_transfer(address & 0xFF);
      address >>= 8;
    }
    spi_end();

    // Enable the nRF24L01p.
    nRF24L01p_enable();

    while (1)
    {
      char string[4];
      int out = nRF24L01p_read(string, 4, 0);
      printf("Read %d bytes -> %s.\n", out, string);
      _delay_ms(500);
    }
  }
  else
  {
    // Set TX mode.
    nRF24L01p_ptx();

    // Set TX address.
    unsigned long long address = nRF24L01p_DEFAULT_ADDRESS;
    spi_start();
    spi_transfer(nRF24L01p_SPI_W_REGISTER |
                 (nRF24L01p_REGISTER_RX_ADDR_P0 & nRF24L01p_SPI_RW_REGISTER_MASK));
    for (byte i = 0; i < 5; i++)
    {
      spi_transfer(address & 0xFF);
      address >>= 8;
    }
    spi_end();

    // Enable the nRF24L01p.
    nRF24L01p_enable();

    while (1)
    {
      char string[4] = "foo";
      int out = nRF24L01p_write(string, 4);
      printf("Wrote %d bytes.\n", out);
      _delay_ms(500);
    }
  }

  return 0;
}
