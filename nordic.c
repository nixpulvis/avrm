/* Here in lies a concerted and complete effort to get my Nordic
 * nRF24L01+ working. I've had this thing collecting dust for
 * about a year, and it's time to buckle down and figure it out.
 */

#define AM_RX 1

#include <avr/io.h>
#include <util/delay.h>
#include "lib/avr.h"
#include "lib/nRF24L01p.h"

// assumes little endian
void printb(void const * const ptr, size_t const size)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--)
  for (j = 7; j >= 0; j--)
  {
    byte = b[i] & (1 << j);
    byte >>= j;
    printf("%u", byte);
  }
}

void assert(const char *name, bool test)
{
  if (test)
    printf("%s PASSED\n", name);
  else
    printf("%s FAILED\n", name);
}

void test(void)
{
  byte config;

  // Power up test.
  nRF24L01p_power_up();
  config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  assert("power_up", config & nRF24L01p_MASK_CONFIG_PWR_UP);

  nRF24L01p_power_down();
  config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  assert("power_down", !(config & nRF24L01p_MASK_CONFIG_PWR_UP));

  // Transceiver mode test.
  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_RX);
  config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  assert("config_transceiver_mode_rx", config & nRF24L01p_MASK_CONFIG_PRIM_RX);

  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_TX);
  config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  assert("config_transceiver_mode_tx", !(config & nRF24L01p_MASK_CONFIG_PRIM_RX));
}

int main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_init();

  // Setup the nRF24L01p.
  nRF24L01p_init();

  // Power up the nRF24L01p.
  nRF24L01p_power_up();

  /// TODO: Just for testing.
  test();
  ///

  // if (AM_RX)
  // {
  //   // Set RX mode.
  //   nRF24L01p_config_transceiver_mode(nRF24L01p_CONFIG_TRANSCEIVER_MODE_RX);

  //   // Set RX address.
  //   unsigned long long address = nRF24L01p_DEFAULT_ADDRESS;
  //   spi_start();
  //   spi_transfer(nRF24L01p_SPI_W_REGISTER |
  //                (nRF24L01p_REGISTER_TX_ADDR & nRF24L01p_SPI_RW_REGISTER_MASK));
  //   for (byte i = 0; i < 5; i++)
  //   {
  //     spi_transfer(address & 0xFF);
  //     address >>= 8;
  //   }
  //   spi_end();

  //   // Enable the nRF24L01p.
  //   nRF24L01p_enable();

  //   while (1)
  //   {
  //     char string[4];
  //     int out = nRF24L01p_read(string, 4, 0);
  //     printf("Read %d bytes -> %s.\n", out, string);
  //     _delay_ms(500);
  //   }
  // }
  // else
  // {
  //   // Set TX mode.
  //   nRF24L01p_config_transceiver_mode(nRF24L01p_CONFIG_TRANSCEIVER_MODE_TX);

  //   // Set TX address.
  //   unsigned long long address = nRF24L01p_DEFAULT_ADDRESS;
  //   spi_start();
  //   spi_transfer(nRF24L01p_SPI_W_REGISTER |
  //                (nRF24L01p_REGISTER_RX_ADDR_P0 & nRF24L01p_SPI_RW_REGISTER_MASK));
  //   for (byte i = 0; i < 5; i++)
  //   {
  //     spi_transfer(address & 0xFF);
  //     address >>= 8;
  //   }
  //   spi_end();

  //   // Enable the nRF24L01p.
  //   nRF24L01p_enable();

  //   while (1)
  //   {
  //     char string[4] = "foo";
  //     int out = nRF24L01p_write(string, 4);
  //     printf("Wrote %d bytes.\n", out);
  //     _delay_ms(500);
  //   }
  // }

  return 0;
}
