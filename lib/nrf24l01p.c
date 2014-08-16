#include "nrf24l01p.h"

//
// nRF24L01p_init implementation.
//
void nRF24L01p_init(void)
{
  // Start up the SPI bus.
  spi_init();

  // Set CE for output.
  DDRB |= _BV(DDB0);

  // Enable the CRC and set it to use 1 bit.
  nRF24L01p_config_crc(nRF24L01p_VALUE_CONFIG_CRC_ENABLE);
  nRF24L01p_config_crc_count(nRF24L01p_VALUE_CONFIG_CRCO_1);

  // Configure address width.
  nRF24L01p_config_address_width(nRF24L01p_VALUE_SETUP_AW_AW_5);

  // Disable all pipes except P0.
  // TODO:

  // Configure default address for TX and pipe 0.
  nRF24L01p_config_address(nRF24L01p_REGISTER_TX_ADDR,
                           nRF24L01p_DEFAULT_ADDRESS);
  nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P0,
                           nRF24L01p_DEFAULT_ADDRESS);

  // Configure payload width.
  nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P0,
                                 nRF24L01p_PAYLOAD_WIDTH);

  // Configure RF channel.
  nRF24L01p_config_channel(2);

  // Configure air data rate.
  nRF24L01p_config_air_data_rate(nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps);

  // Configure output power.
  nRF24L01p_config_output_power(nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm);

  // Enable auto acknowledgment.
  nRF24L01p_config_auto_ack(nRF24L01p_MASK_EN_AA_ENAA_ALL, TRUE);

  // Configure retransmit.
  nRF24L01p_config_retransmit_count(nRF24L01p_VALUE_SETUP_RETR_ARC_3);
  nRF24L01p_config_retransmit_delay(nRF24L01p_VALUE_SETUP_RETR_ARD_250);

  // Flush the FIFOs
  nRF24L01p_tx_fifo_flush();
  nRF24L01p_rx_fifo_flush();

  // Clear the interrupts.
  nRF24L01p_status_rx_ready_clear();
  nRF24L01p_status_tx_sent_clear();
  nRF24L01p_status_max_retries_clear();

  // Enable external interrupt 0.
  // TODO: Make this configurable.
  EIMSK |= (1 << INT0);
  EICRA &= ~(1 << ISC00);

  // Enable interrupts.
  sei();

  // Power up.
  nRF24L01p_config_power(nRF24L01p_VALUE_CONFIG_PWR_UP);
}

// IRQ SERVICE HANDLER
//////////////////////

ISR (INT0_vect)
{
  nRF24L01p_status_fetch();
  if (nRF24L01p_status_rx_ready())
  {
    printf("RX READY\n");

    // TODO: Read into a buffer?

    // HACK: This was commented out to allow read_sync to
    // handle this interrupt in a while(1) loop. When we
    // handle read for pipes we'll need to allow this to
    // handle all rx_ready calls.
    //
    // nRF24L01p_status_rx_ready_clear();
  }
  if (nRF24L01p_status_tx_sent())
  {
    printf("TX SENT\n");

    // TODO: Figure out if we need to do anything here.

    nRF24L01p_status_tx_sent_clear();
  }
  if (nRF24L01p_status_max_retries())
  {
    printf("MAX RETRIES\n");

    // TODO: Handle link loss.
    nRF24L01p_tx_fifo_flush();
    _delay_ms(100);

    nRF24L01p_status_max_retries_clear();
  }

  // if (nRF24L01p_status_pipe_ready())

  if (nRF24L01p_status_tx_full())
  {
    printf("TX FIFO FULL\n");

    // TODO: Think this case through.
  }
}

// Configuration
////////////////


//
// nRF24L01p_config_rx_dr_interrupt implementation.
//
int nRF24L01p_config_rx_dr_interrupt(bool value)
{
  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
                               nRF24L01p_MASK_CONFIG_MASK_RX_DR,
                               value ? 0x00 : 0xFF);
  return 0;
}


//
// nRF24L01p_config_tx_ds_interrupt implementation.
//
int nRF24L01p_config_tx_ds_interrupt(bool value)
{
  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
                               nRF24L01p_MASK_CONFIG_MASK_TX_DS,
                               value ? 0x00 : 0xFF);
  return 0;
}


//
// nRF24L01p_config_max_rt_interrupt implementation.
//
int nRF24L01p_config_max_rt_interrupt(bool value)
{
  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
                               nRF24L01p_MASK_CONFIG_MASK_MAX_RT,
                               value ? 0x00 : 0xFF);
  return 0;
}


//
// nRF24L01p_config_crc implementation.
//
int nRF24L01p_config_crc(byte value)
{
  if (!(value == nRF24L01p_VALUE_CONFIG_CRC_ENABLE ||
        value == nRF24L01p_VALUE_CONFIG_CRC_DISABLE))
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
                               nRF24L01p_MASK_CONFIG_EN_CRC,
                               value);

  return 0;
}


//
// nRF24L01p_config_crc_count implementation.
//
int nRF24L01p_config_crc_count(byte value)
{
  if (!(value == nRF24L01p_VALUE_CONFIG_CRCO_1 ||
        value == nRF24L01p_VALUE_CONFIG_CRCO_2))
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_CONFIG,
                               nRF24L01p_MASK_CONFIG_CRCO,
                               value);

  return 0;
}


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

  if (value == nRF24L01p_VALUE_CONFIG_PWR_UP)
    _delay_us(nRF24L01p_TIMING_TPD2STBY_US);

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
// nRF24L01p_config_auto_ack implementation.
//
int nRF24L01p_config_auto_ack(byte mask, bool value)
{
  if (!(mask == nRF24L01p_MASK_EN_AA_ENAA_P0 ||
        mask == nRF24L01p_MASK_EN_AA_ENAA_P1 ||
        mask == nRF24L01p_MASK_EN_AA_ENAA_P2 ||
        mask == nRF24L01p_MASK_EN_AA_ENAA_P3 ||
        mask == nRF24L01p_MASK_EN_AA_ENAA_P4 ||
        mask == nRF24L01p_MASK_EN_AA_ENAA_P5 ||
        mask == nRF24L01p_MASK_EN_AA_ENAA_ALL))
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_EN_AA,
                               mask, value ? 0xFF : 0x00);
  return 0;
}


//
// nRF24L01p_config_rx implementation.
//
int nRF24L01p_config_rx(byte mask, bool value)
{
  if (!(mask == nRF24L01p_MASK_EN_RXADDR_ERX_P0 ||
        mask == nRF24L01p_MASK_EN_RXADDR_ERX_P1 ||
        mask == nRF24L01p_MASK_EN_RXADDR_ERX_P2 ||
        mask == nRF24L01p_MASK_EN_RXADDR_ERX_P3 ||
        mask == nRF24L01p_MASK_EN_RXADDR_ERX_P4 ||
        mask == nRF24L01p_MASK_EN_RXADDR_ERX_P5 ||
        mask == nRF24L01p_MASK_EN_RXADDR_ERX_ALL))
    return -1;

  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_EN_RXADDR,
                               mask, value ? 0xFF : 0x00);
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


// STATUS
/////////

static byte nRF24L01p_status = 0x0E;

//
// nRF24L01p_status_fetch
//
void nRF24L01p_status_fetch(void)
{
  spi_start();
  nRF24L01p_status = spi_transfer(nRF24L01p_SPI_NOP);
  spi_end();
}


//
// nRF24L01p_status_rx_ready implementation.
//
bool nRF24L01p_status_rx_ready(void)
{
  return nRF24L01p_status & nRF24L01p_MASK_STATUS_RX_DR;
}


//
// nRF24L01p_status_rx_ready_clear implementation.
//
void nRF24L01p_status_rx_ready_clear(void)
{
  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_STATUS,
                               nRF24L01p_MASK_STATUS_RX_DR, 0xFF);
}


//
// nRF24L01p_status_tx_sent implementation.
//
bool nRF24L01p_status_tx_sent(void)
{
  return nRF24L01p_status & nRF24L01p_MASK_STATUS_TX_DS;
}


//
// nRF24L01p_status_tx_sent_clear implementation.
//
void nRF24L01p_status_tx_sent_clear(void)
{
  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_STATUS,
                               nRF24L01p_MASK_STATUS_TX_DS, 0xFF);
}


//
// nRF24L01p_status_max_retries implementation.
//
bool nRF24L01p_status_max_retries(void)
{
  return nRF24L01p_status & nRF24L01p_MASK_STATUS_MAX_RT;
}


//
// nRF24L01p_status_max_retries_clear implementation.
//
void nRF24L01p_status_max_retries_clear(void)
{
  nRF24L01p_set_register8_bits(nRF24L01p_REGISTER_STATUS,
                               nRF24L01p_MASK_STATUS_MAX_RT, 0xFF);
}


//
// nRF24L01p_status_pipe_ready implementation.
//
byte nRF24L01p_status_pipe_ready(void)
{
  return nRF24L01p_status & nRF24L01p_MASK_STATUS_RX_P_NO;
}


//
// nRF24L01p_status_tx_full implementation.
//
bool nRF24L01p_status_tx_full(void)
{
  return nRF24L01p_status & nRF24L01p_MASK_STATUS_TX_FULL;
}


// FIFO
///////

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
// nRF24L01p_rx_fifo_read implementation.
//
int nRF24L01p_rx_fifo_read(byte *payload, byte size)
{
  spi_start();
  spi_transfer(nRF24L01p_SPI_R_RX_PAYLOAD);
  for (byte i = 0; i < size; i++)
    *payload++ = spi_transfer(nRF24L01p_SPI_NOP);
  spi_end();

  return size; // TODO: catch FIFO full IRQ.
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
  _delay_us(nRF24L01p_TIMING_TSTBY2A_US);
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
// nRF24L01p_read_sync implementation.
//
int nRF24L01p_read_sync(byte *dst, byte count)
{
  // TODO: Use R_RX_PL_WID to decide width of read payload.
  // TODO: Pipes.

  while (count > 0)
  {
    nRF24L01p_status_fetch();
    if (nRF24L01p_status_rx_ready())
    {
      if (count < nRF24L01p_PAYLOAD_WIDTH)
      {
        byte *payload = malloc(nRF24L01p_PAYLOAD_WIDTH);
        nRF24L01p_rx_fifo_read(payload, nRF24L01p_PAYLOAD_WIDTH);
        memcpy(dst, payload, count);
        free(payload);
        count = 0;
      }
      else
      {
        nRF24L01p_rx_fifo_read(dst, nRF24L01p_PAYLOAD_WIDTH);
        dst = dst + nRF24L01p_PAYLOAD_WIDTH;
        count = count - nRF24L01p_PAYLOAD_WIDTH;
      }
      nRF24L01p_status_rx_ready_clear(); // HACK: remove when we implement pipes.
    }
  }

  return count;
}


//
// nRF24L01p_write_sync implementation.
//
int nRF24L01p_write_sync(byte *src, byte count)
{
  // TODO: Add setting for tx packet width.
  // TODO: Dynamic!

  while (count > 0)
  {
    if (!nRF24L01p_tx_fifo_is_full())
    {
      if (count < nRF24L01p_PAYLOAD_WIDTH)
      {
        byte *payload = malloc(nRF24L01p_PAYLOAD_WIDTH);
        memcpy(payload, src, count);
        nRF24L01p_tx_fifo_write(payload, nRF24L01p_PAYLOAD_WIDTH);
        free(payload);
        count = 0;
      }
      else
      {
        nRF24L01p_tx_fifo_write(src, nRF24L01p_PAYLOAD_WIDTH);
        src = src + nRF24L01p_PAYLOAD_WIDTH;
        count = count - nRF24L01p_PAYLOAD_WIDTH;
      }
    }
  }


  return count;
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
