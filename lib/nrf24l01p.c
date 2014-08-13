#include "nrf24l01p.h"

//
// nRF24L01p_init implementation.
//
void nRF24L01p_init(void)
{
  // TODO
  // Start up the SPI bus.
  spi_init();
}


// Configuration
////////////////


//
// nRF24L01p_config_transceiver_mode implementation.
//
void nRF24L01p_config_transceiver_mode(bool mode)
{
  byte config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  if (mode == nRF24L01p_CONFIG_TRANSCEIVER_MODE_RX)
    config |= nRF24L01p_REGISTER_CONFIG_PRIM_RX;
  else
    config &= ~nRF24L01p_REGISTER_CONFIG_PRIM_RX;
  nRF24L01p_set_register(nRF24L01p_REGISTER_CONFIG, config);
}


// Control
//////////


//
// nRF24L01p_power_up implementation.
//
void nRF24L01p_power_up(void)
{
  byte config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  config |= nRF24L01p_REGISTER_CONFIG_PWR_UP;
  nRF24L01p_set_register(nRF24L01p_REGISTER_CONFIG, config);
}


//
// nRF24L01p_power_down implementation.
//
void nRF24L01p_power_down(void)
{
  byte config = nRF24L01p_get_register(nRF24L01p_REGISTER_CONFIG);
  config &= ~nRF24L01p_REGISTER_CONFIG_PWR_UP;
  nRF24L01p_set_register(nRF24L01p_REGISTER_CONFIG, config);
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
