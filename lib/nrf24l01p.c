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

  // Configure address width.
  nRF24L01p_config_address_width(nRF24L01p_VALUE_SETUP_AW_AW_5);

  // Configure default address for TX and pipe 0.
  nRF24L01p_config_address(nRF24L01p_REGISTER_TX_ADDR,
                           nRF24L01p_DEFAULT_ADDRESS);
  nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P0,
                           nRF24L01p_DEFAULT_ADDRESS);

  // Configure payload width.
  nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P0,
                                 nRF24L01p_PAYLOAD_WIDTH);

  // Configure air data rate.
  nRF24L01p_config_air_data_rate(nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps);

  // Configure output power.
  nRF24L01p_config_output_power(nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm);

  // Configure retransmit.
  nRF24L01p_config_retransmit_count(nRF24L01p_VALUE_SETUP_RETR_ARC_3);
  nRF24L01p_config_retransmit_delay(nRF24L01p_VALUE_SETUP_RETR_ARD_250);

  // Configure RF channel.
  nRF24L01p_config_channel(2);

  // Power up.
  nRF24L01p_config_power(nRF24L01p_VALUE_CONFIG_PWR_UP);
}


// Configuration
////////////////


//
// nRF24L01p_config_power implementation.
//
int nRF24L01p_config_power(byte value)
{
  if (!(value == nRF24L01p_VALUE_CONFIG_PWR_UP ||
        value == nRF24L01p_VALUE_CONFIG_PWR_DOWN))
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
                              nRF24L01p_MASK_CONFIG_PWR_UP,
                              value);

  return 0;
}


//
// nRF24L01p_config_transceiver_mode implementation.
//
int nRF24L01p_config_transceiver_mode(byte value)
{
  if (!(value == nRF24L01p_VALUE_CONFIG_PRIM_TX ||
        value == nRF24L01p_VALUE_CONFIG_PRIM_RX))
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
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

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_SETUP_AW,
                              nRF24L01p_MASK_SETUP_AW_AW,
                              value);
  return 0;
}


//
// nRF24L01p_config_retransmit_delay implementation.
//
int nRF24L01p_config_retransmit_delay(byte value)
{
  if (value <= 0x0F)
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_SETUP_RETR,
                              nRF24L01p_MASK_SETUP_RETR_ARD,
                              value);
  return 0;
}


//
// nRF24L01p_config_retransmit_count implementation.
//
int nRF24L01p_config_retransmit_count(byte value)
{
  if (value > 0x0F)
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_SETUP_RETR,
                              nRF24L01p_MASK_SETUP_RETR_ARC,
                              value);
  return 0;
}


//
// nRF24L01p_config_channel implementation.
//
int nRF24L01p_config_channel(byte value)
{
  if (value > 127)
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_RF_CH,
                              nRF24L01p_MASK_RF_CH_RF_CH,
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

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_RF_SETUP,
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

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_RF_SETUP,
                              nRF24L01p_MASK_RF_SETUP_RF_PWR,
                              value);
  return 0;
}


//
// nRF24L01p_config_address implementation.
//
int nRF24L01p_config_address(byte address_register,
                             long long unsigned int address)
{
  // TODO error checking for bad addresses.

  nRF24L01p_set_register40(address_register, address);

  return 0;
}


//
// nRF24L01p_config_payload_width implementation.
//
int nRF24L01p_config_payload_width(byte address, byte width)
{
  if (!(address == nRF24L01p_REGISTER_RX_PW_P0 ||
        address == nRF24L01p_REGISTER_RX_PW_P1 ||
        address == nRF24L01p_REGISTER_RX_PW_P2 ||
        address == nRF24L01p_REGISTER_RX_PW_P3 ||
        address == nRF24L01p_REGISTER_RX_PW_P4 ||
        address == nRF24L01p_REGISTER_RX_PW_P5))
    return -1;

  nRF24L01p_set_register8_bits(address,
                               nRF24L01p_MASK_RX_PW,
                               width);

  return 0;
}


// FIFO STATUS
//////////////

//
// nRF24L01p_tx_fifo_is_reuse
//
bool nRF24L01p_tx_fifo_is_reuse(void)
{
  return nRF24L01p_get_register8(nRF24L01p_REGISTER_FIFO_STATUS) &
         nRF24L01p_MASK_FIFO_STATUS_TX_REUSE;
}


//
// nRF24L01p_tx_fifo_is_full
//
bool nRF24L01p_tx_fifo_is_full(void)
{
  return nRF24L01p_get_register8(nRF24L01p_REGISTER_FIFO_STATUS) &
         nRF24L01p_MASK_FIFO_STATUS_TX_FULL;
}


//
// nRF24L01p_tx_fifo_is_empty
//
bool nRF24L01p_tx_fifo_is_empty(void)
{
  return nRF24L01p_get_register8(nRF24L01p_REGISTER_FIFO_STATUS) &
         nRF24L01p_MASK_FIFO_STATUS_TX_EMPTY;
}


//
// nRF24L01p_tx_fifo_write implementation.
//
int nRF24L01p_tx_fifo_write(byte *payload, byte size)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_W_TX_PAYLOAD);
  for (byte i = 0; i < size; i++)
    spi_transfer(*payload++);
  spi_end();

  return size; // TODO: catch FIFO full IRQ.
}


//
// nRF24L01p_tx_fifo_flush implementation.
//
void nRF24L01p_tx_fifo_flush(void)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_FLUSH_TX);
  spi_end();
}


//
// nRF24L01p_rx_fifo_is_full
//
bool nRF24L01p_rx_fifo_is_full(void)
{
  return nRF24L01p_get_register8(nRF24L01p_REGISTER_FIFO_STATUS) &
         nRF24L01p_MASK_FIFO_STATUS_RX_FULL;
}


//
// nRF24L01p_rx_fifo_is_empty
//
bool nRF24L01p_rx_fifo_is_empty(void)
{
  return nRF24L01p_get_register8(nRF24L01p_REGISTER_FIFO_STATUS) &
         nRF24L01p_MASK_FIFO_STATUS_RX_EMPTY;
}


//
// nRF24L01p_rx_fifo_flush implementation.
//
void nRF24L01p_rx_fifo_flush(void)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_FLUSH_RX);
  spi_end();
}


// IO
/////


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
// nRF24L01p_get_register8 implementation.
//
byte nRF24L01p_get_register8(byte address)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_R_REGISTER |
               (address & nRF24L01p_SPI_RW_REGISTER_MASK));
  byte response = spi_transfer(nRF24L01p_SPI_NOP);
  spi_end();

  return response;
}


//
// nRF24L01p_set_register8 implementation.
//
int nRF24L01p_set_register8(byte address, byte data)
{
  if (address >= 0x18 && address <= 0x1B)
    return -1;

  spi_start();
  spi_transfer(nRF24L01p_SPI_W_REGISTER |
               (address & nRF24L01p_SPI_RW_REGISTER_MASK));
  spi_transfer(data);
  spi_end();

  return 0;
}


//
// nRF24L01p_set_register8_bits implementation.
//
int nRF24L01p_set_register8_bits(byte address, byte mask, byte value)
{
  byte reg = nRF24L01p_get_register8(address);
  reg = (reg & ~mask) | value;
  return nRF24L01p_set_register8(address, reg);
}


//
// nRF24L01p_get_register40 implementation.
//
long long unsigned int nRF24L01p_get_register40(byte address)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_R_REGISTER |
               (address & nRF24L01p_SPI_RW_REGISTER_MASK));
  long long unsigned int response = 0;
  for (byte i = 0; i < 5; i++)
  {
    long long unsigned int chunk = spi_transfer(nRF24L01p_SPI_NOP);
    response |= chunk << (8 * i);
  }
  spi_end();

  return response;
}


//
// nRF24L01p_set_register40 implementation.
//
int nRF24L01p_set_register40(byte address, long long unsigned int data)
{
  if (address >= 0x18 && address <= 0x1B)
    return -1;

  spi_start();
  spi_transfer(nRF24L01p_SPI_W_REGISTER |
               (address & nRF24L01p_SPI_RW_REGISTER_MASK));
  for (byte i = 0; i < 5; i++)
  {
    spi_transfer(data & 0xFF);
    data >>= 8;
  }
  spi_end();

  return 0;
}
