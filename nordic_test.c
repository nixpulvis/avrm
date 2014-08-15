/* Tests for the nrf24l01p lib. */

#include <avr/io.h>
#include <util/delay.h>
#include "lib/avr.h"
#include "lib/nRF24L01p.h"

// Forward declarations.
void assert(const char *name, bool test);

int main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_init();

  // Wait for the nRF24L01p to be ready.
  _delay_us(nRF24L01p_TIMING_INITIAL_US);

  // Initialize the nRF24L01p.
  nRF24L01p_init();

  // Common storage for register values.
  byte reg;
  long long unsigned int reg40;

  printf("\nRunning Tests\n");

  // Configuration Tests
  //////////////////////

  // Configure power test.
  nRF24L01p_config_power(nRF24L01p_VALUE_CONFIG_PWR_UP);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_CONFIG);
  assert("config_power_up", (reg & nRF24L01p_MASK_CONFIG_PWR_UP)
                     == nRF24L01p_VALUE_CONFIG_PWR_UP);

  nRF24L01p_config_power(nRF24L01p_VALUE_CONFIG_PWR_DOWN);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_CONFIG);
  assert("config_power_down", (reg & nRF24L01p_MASK_CONFIG_PWR_UP)
                       == nRF24L01p_VALUE_CONFIG_PWR_DOWN);

  // Transceiver mode test.
  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_RX);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_CONFIG);
  assert("config_transceiver_mode_rx", (reg & nRF24L01p_MASK_CONFIG_PRIM_RX)
                                       == nRF24L01p_VALUE_CONFIG_PRIM_RX);

  nRF24L01p_config_transceiver_mode(nRF24L01p_VALUE_CONFIG_PRIM_TX);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_CONFIG);
  assert("config_transceiver_mode_tx", (reg & nRF24L01p_MASK_CONFIG_PRIM_RX)
                                       == nRF24L01p_VALUE_CONFIG_PRIM_TX);

  // EN_AA

  // EN_RXADDR

  // SETUP_AW

  // Configure address width test.
  nRF24L01p_config_address_width(nRF24L01p_VALUE_SETUP_AW_AW_3);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_SETUP_AW);
  assert("config_address_width_3", (reg & nRF24L01p_MASK_SETUP_AW_AW)
                                   == nRF24L01p_VALUE_SETUP_AW_AW_3);

  nRF24L01p_config_address_width(nRF24L01p_VALUE_SETUP_AW_AW_5);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_SETUP_AW);
  assert("config_address_width_5", (reg & nRF24L01p_MASK_SETUP_AW_AW)
                                   == nRF24L01p_VALUE_SETUP_AW_AW_5);

  // SETUP_RETR

  // Configure retransmit delay test.
  nRF24L01p_config_retransmit_delay(nRF24L01p_VALUE_SETUP_RETR_ARD_750);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_SETUP_RETR);
  assert("config_retransmit_delay_750", (reg & nRF24L01p_MASK_SETUP_RETR_ARD)
                                      == nRF24L01p_VALUE_SETUP_RETR_ARD_750);

  nRF24L01p_config_retransmit_delay(nRF24L01p_VALUE_SETUP_RETR_ARD_4000);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_SETUP_RETR);
  assert("config_retransmit_delay_15", (reg & nRF24L01p_MASK_SETUP_RETR_ARD)
                                      == nRF24L01p_VALUE_SETUP_RETR_ARD_4000);

  // Configure retransmit count test.
  nRF24L01p_config_retransmit_count(nRF24L01p_VALUE_SETUP_RETR_ARC_0);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_SETUP_RETR);
  assert("config_retransmit_count_0", (reg & nRF24L01p_MASK_SETUP_RETR_ARC)
                                      == nRF24L01p_VALUE_SETUP_RETR_ARC_0);

  nRF24L01p_config_retransmit_count(nRF24L01p_VALUE_SETUP_RETR_ARC_15);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_SETUP_RETR);
  assert("config_retransmit_count_15", (reg & nRF24L01p_MASK_SETUP_RETR_ARC)
                                      == nRF24L01p_VALUE_SETUP_RETR_ARC_15);

  // RF_CH

  // Configure channel test.
  nRF24L01p_config_channel(0);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RF_CH);
  assert("config_channel_0", (reg & nRF24L01p_MASK_RF_CH_RF_CH) == 0);

  nRF24L01p_config_channel(97);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RF_CH);
  assert("config_channel_97", (reg & nRF24L01p_MASK_RF_CH_RF_CH) == 97);

  // RF_SETUP

  // Configure air data rate test.
  nRF24L01p_config_air_data_rate(nRF24L01p_VALUE_RF_SETUP_RF_DR_1Mbps);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RF_SETUP);
  assert("config_air_data_rate_1Mbps", (reg & nRF24L01p_MASK_RF_SETUP_RF_DR)
                                       == nRF24L01p_VALUE_RF_SETUP_RF_DR_1Mbps);

  nRF24L01p_config_air_data_rate(nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RF_SETUP);
  assert("config_air_data_rate_2Mbps", (reg & nRF24L01p_MASK_RF_SETUP_RF_DR)
                                       == nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps);

  // Configure output power test.
  nRF24L01p_config_output_power(nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_12dBm);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RF_SETUP);
  assert("config_output_power_-12dBm", (reg & nRF24L01p_MASK_RF_SETUP_RF_PWR)
                                       == nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_12dBm);

  nRF24L01p_config_output_power(nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RF_SETUP);
  assert("config_output_power_0dBm", (reg & nRF24L01p_MASK_RF_SETUP_RF_PWR)
                                     == nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm);

  // CD

  // RX_ADDR_P(0-5) and TX_ADDR

  // Configure address test.
  nRF24L01p_config_address(nRF24L01p_REGISTER_TX_ADDR, 0x374DFE620B);
  reg40 = nRF24L01p_get_register40(nRF24L01p_REGISTER_TX_ADDR);
  assert("config_address_tx", reg40 == 0x374DFE620B);

  nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P0, 0x374DFE620B);
  reg40 = nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P0);
  assert("config_address_rx_p0", reg40 == 0x374DFE620B);

  // RX_PW_P(0-5)

  // Configure payload width test.
  nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P0, 17);
  reg = nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P0);
  assert("config_payload_width", (reg & nRF24L01p_MASK_RX_PW) == 17);

  // FIFO STATUS TESTS
  ////////////////////

  // FIFO status tests.
  spi_start();
  spi_transfer(nRF24L01p_SPI_FLUSH_TX);
  spi_end();
  spi_start();
  spi_transfer(nRF24L01p_SPI_FLUSH_RX);
  spi_end();

  bool empty, full;
  full = nRF24L01p_tx_fifo_is_full();
  assert("tx_fifo_is_not_full", !full);
  empty = nRF24L01p_tx_fifo_is_empty();
  assert("tx_fifo_is_empty", empty);

  spi_start();
  spi_transfer(nRF24L01p_SPI_W_TX_PAYLOAD);
  for (byte i = 0; i < nRF24L01p_FIFO_TX_SIZE; i++)
    spi_transfer(0xB7);
  spi_end();

  full = nRF24L01p_tx_fifo_is_full();
  assert("tx_fifo_is_still_not_full", !full);
  empty = nRF24L01p_tx_fifo_is_empty();
  assert("tx_fifo_is_not_empty", !empty);

  spi_start();
  spi_transfer(nRF24L01p_SPI_W_TX_PAYLOAD);
  for (byte i = 0; i < (nRF24L01p_FIFO_TX_COUNT - 1) *
                       nRF24L01p_FIFO_TX_SIZE; i++)
    spi_transfer(0xB7);
  spi_end();

  full = nRF24L01p_tx_fifo_is_full();
  assert("tx_fifo_is_full", full);
  empty = nRF24L01p_tx_fifo_is_empty();
  assert("tx_fifo_is_still_not_empty", !empty);

  return 0;
}


//
// assert implementation.
//
void assert(const char *name, bool test)
{
  if (test)
    printf("%s PASS\n", name);
  else
    printf("%s FAIL\n", name);
}
