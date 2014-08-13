#include "nrf24l01p.h"

//
// nRF24L01p_init implementation.
//
void nRF24L01p_init(void)
{
  // TODO

  // Start up the SPI bus.
  spi_init();

  // Set CE for output.
  DDRB |= _BV(DDB0);
}


// Configuration
////////////////


//
// nRF24L01p_config_transceiver_mode implementation.
//
int nRF24L01p_config_transceiver_mode(byte value)
{
  if (!(value == nRF24L01p_VALUE_CONFIG_PRIM_TX ||
        value == nRF24L01p_VALUE_CONFIG_PRIM_RX))
    return -1;

  nRF24L01p_set_register_bits(nRF24L01p_REGISTER_CONFIG,
                              nRF24L01p_MASK_CONFIG_PRIM_RX,
                              value);
  return 0;
}


//
// nRF24L01p_config_address_width implementation.
//
int nRF24L01p_config_address_width(byte value)
{
  if (!(value == nRF24L01p_VALUE_SETUP_AW_AW_3 ||
        value == nRF24L01p_VALUE_SETUP_AW_AW_4 ||
        value == nRF24L01p_VALUE_SETUP_AW_AW_5))
    return -1;

  nRF24L01p_set_register_bits(nRF24L01p_REGISTER_SETUP_AW,
                              nRF24L01p_MASK_SETUP_AW_AW,
                              value);
  return 0;
}


//
// nRF24L01p_config_air_data_rate implementation.
//
int nRF24L01p_config_air_data_rate(byte value)
{
  if (!(value == nRF24L01p_VALUE_RF_SETUP_RF_DR_1Mbps ||
        value == nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps))
    return -1;

  nRF24L01p_set_register_bits(nRF24L01p_REGISTER_RF_SETUP,
                              nRF24L01p_MASK_RF_SETUP_RF_DR,
                              value);
  return 0;
}


//
// nRF24L01p_config_output_power implementation.
//
int nRF24L01p_config_output_power(byte value)
{
  if (!(value == nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_18dBm ||
        value == nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_12dBm ||
        value == nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_6dBm ||
        value == nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm))
    return -1;

  nRF24L01p_set_register_bits(nRF24L01p_REGISTER_RF_SETUP,
                              nRF24L01p_MASK_RF_SETUP_RF_PWR,
                              value);
  return 0;
}


// Control
//////////


//
// nRF24L01p_power_up implementation.
//
void nRF24L01p_power_up(void)
{
  nRF24L01p_set_register_bits(nRF24L01p_REGISTER_CONFIG,
                              nRF24L01p_MASK_CONFIG_PWR_UP,
                              nRF24L01p_VALUE_CONFIG_PWR_UP);
}


//
// nRF24L01p_power_down implementation.
//
void nRF24L01p_power_down(void)
{
  nRF24L01p_set_register_bits(nRF24L01p_REGISTER_CONFIG,
                              nRF24L01p_MASK_CONFIG_PWR_UP,
                              nRF24L01p_VALUE_CONFIG_PWR_DOWN);
}


//
// nRF24L01p_enable implementation.
//
void nRF24L01p_enable(void)
{
  // Pull CE high.
  PORTB |= _BV(PORTB0);
}


//
// nRF24L01p_disable implementation.
//
void nRF24L01p_disable(void)
{
  // Pull CE high.
  PORTB &= ~_BV(PORTB0);
}


// IO
/////


//
// nRF24L01p_read implementation.
//
int nRF24L01p_read(char *dst, byte count, byte pipe)
{
  // Ensure pipe is in [0,5].
  if (pipe < 0 || pipe > 5)
  {
    return -1;
  }

  // Return early if we were told to read nothing.
  if (count == 0)
    return 0;

  // Read in chunks of max size nRF24L01p_FIFO_RX_SIZE.
  byte chunk = count > nRF24L01p_FIFO_RX_SIZE ? nRF24L01p_FIFO_RX_SIZE : count;

  // The number of bytes left to read.
  // byte remaining = count - chunk;

  // TODO: check (nRF24L01p_rx_available(pipe))

  // TODO: Check for need to flush FIFO.

  // TODO: Chuck by nRF24L01p_SPI_R_RX_PL_WID.

  spi_start();
  spi_transfer(nRF24L01p_SPI_R_RX_PAYLOAD);
  for (byte i = 0; i < chunk; i++)
    *dst++ = spi_transfer(nRF24L01p_SPI_NOP);
  spi_end();

  return chunk; // TODO: This is very clearly wrong.
}


//
// nRF24L01p_write implementation.
//
int nRF24L01p_write(char *src, byte count)
{
  // Return early if we were told to write nothing.
  if (count == 0)
    return 0;

  // Write in chunks of max size nRF24L01p_FIFO_TX_SIZE.
  byte chunk = count > nRF24L01p_FIFO_TX_SIZE ? nRF24L01p_FIFO_TX_SIZE : count;

  // The number of bytes left to read.
  // byte remaining = count - chunk;

  // TODO: check (nRF24L01p_tx_available(pipe))

  // TODO: Check for need to flush FIFO.

  // TODO: Chuck by nRF24L01p_SPI_R_RX_PL_WID. ???

  spi_start();
  spi_transfer(nRF24L01p_SPI_W_TX_PAYLOAD);
  for (byte i = 0; i < chunk; i++ )
    spi_transfer(*src++);
  spi_end();

  return chunk; // TODO: This is very clearly wrong.
}


// Utility
//////////


//
// nRF24L01p_get_register implementation.
//
byte nRF24L01p_get_register(byte address)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_R_REGISTER |
               (address & nRF24L01p_SPI_RW_REGISTER_MASK));
  byte response = spi_transfer(nRF24L01p_SPI_NOP);
  spi_end();

  return response;
}


//
// nRF24L01p_set_register implementation.
//
void nRF24L01p_set_register(byte address, byte data)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_W_REGISTER |
               (address & nRF24L01p_SPI_RW_REGISTER_MASK));
  spi_transfer(data);
  spi_end();
}


//
// nRF24L01p_set_register_bits implementation.
//
void nRF24L01p_set_register_bits(byte address, byte mask, byte value)
{
  byte reg = nRF24L01p_get_register(address);
  reg = (reg & ~mask) | value;
  nRF24L01p_set_register(address, reg);
}
