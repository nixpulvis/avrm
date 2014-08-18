#include "nrf24l01p.h"

//
// nRF24L01p_init implementation.
//
int nRF24L01p_init(int ce, int irq)
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

  // Set all pipes in EN_RXADDR to disabled. We'll enable them
  // with nRF24L01p_enable_pipe.
  nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_ALL, FALSE);

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
  EICRA = (EICRA & ~(1 << ISC00 | 1 << ISC01)) | 0x02;

  // Enable interrupts.
  sei();

  // Power up.
  nRF24L01p_config_power(nRF24L01p_VALUE_CONFIG_PWR_UP);

  // TODO: Plz.
  return 0;
}

// PIPE BUFFERS
///////////////

// RX pipe.
struct nRF24L01p_RX_PIPE
{
  byte *data;
  size_t remaining;
};

// TX pipe.
struct nRF24L01p_TX_PIPE
{
  const byte *data;
  size_t remaining;
  byte pipe;
};

// Global pipes.
static struct nRF24L01p_RX_PIPE nRF24L01p_rx_pipes[6];
static struct nRF24L01p_TX_PIPE nRF24L01p_tx_pipe;


//
// private nRF24L01p_process_tx_payload implementation.
//
void nRF24L01p_process_tx_payload()
{
  // TODO: This is sketchy. Needs thought, I should really draw
  //       a state diagram.
  if (nRF24L01p_tx_pipe.remaining == 0)
  {
    nRF24L01p_disable();
  }
  else
  {
    nRF24L01p_enable();

    byte payload_width = nRF24L01p_payload_width(nRF24L01p_tx_pipe.pipe);

    while (!nRF24L01p_tx_fifo_is_full() && nRF24L01p_tx_pipe.remaining != 0)
    {
      // TODO: Move this logic into nRF24L01p_rx_fifo_write.
      //       nRF24L01p_rx_fifo_write could guarantee count size
      //       data.
      if (nRF24L01p_tx_pipe.remaining < payload_width)
      {
        byte *payload = malloc(payload_width);
        memcpy(payload, nRF24L01p_tx_pipe.data, nRF24L01p_tx_pipe.remaining);
        nRF24L01p_tx_fifo_write(payload, payload_width);
        free(payload);
        nRF24L01p_tx_pipe.remaining = 0;
      }
      else
      {
        nRF24L01p_tx_fifo_write(nRF24L01p_tx_pipe.data, payload_width);
        nRF24L01p_tx_pipe.data = nRF24L01p_tx_pipe.data + payload_width;
        nRF24L01p_tx_pipe.remaining = nRF24L01p_tx_pipe.remaining - payload_width;
      }
    }
  }

  nRF24L01p_status_tx_sent_clear();
}


//
// private nRF24L01p_process_rx_payload implementation.
//
void nRF24L01p_process_rx_payload(byte pipe_number)
{
  byte payload_width = nRF24L01p_payload_width(1 << pipe_number);

  if (nRF24L01p_rx_pipes[pipe_number].remaining == 0)
  {
    // TODO: If this is really what I want to do, this
    //       should be nRF24L01p_rx_fifo_drop. But I
    //       would love to figure out a way to not do this.
    spi_start();
    spi_transfer(nRF24L01p_SPI_R_RX_PAYLOAD);
    for (byte i = 0; i < payload_width; i++)
      spi_transfer(nRF24L01p_SPI_NOP);
    spi_end();
  }
  else
  {
    // TODO: Move this logic into nRF24L01p_rx_fifo_read.
    //       nRF24L01p_rx_fifo_read could guarantee count size
    //       data.
    if (nRF24L01p_rx_pipes[pipe_number].remaining < payload_width)
    {
      byte *payload = malloc(payload_width);
      nRF24L01p_rx_fifo_read(payload, payload_width);
      memcpy(nRF24L01p_rx_pipes[pipe_number].data, payload, nRF24L01p_rx_pipes[pipe_number].remaining);
      free(payload);
      nRF24L01p_rx_pipes[pipe_number].remaining = 0;
    }
    else
    {
      nRF24L01p_rx_fifo_read(nRF24L01p_rx_pipes[pipe_number].data, payload_width);
      nRF24L01p_rx_pipes[pipe_number].data = nRF24L01p_rx_pipes[pipe_number].data + payload_width;
      nRF24L01p_rx_pipes[pipe_number].remaining = nRF24L01p_rx_pipes[pipe_number].remaining - payload_width;
    }
  }
}


// IRQ SERVICE HANDLER
//////////////////////

ISR (INT0_vect)
{
  nRF24L01p_status_fetch();

  if (nRF24L01p_status_tx_sent())
  {
    printf("TX SENT\n");

    nRF24L01p_process_tx_payload();
  }

  if (nRF24L01p_status_rx_ready())
  {
    printf("RX RECEIVED\n");

    // TODO: This whole thing should just be
    //       nRF24L01p_process_rx_payload(pipe_number)
    //       since I'm 99% sure we can't trust updated
    //       pipe numbers in the same interrupt anyway.

    while (!nRF24L01p_rx_fifo_is_empty())
    {
      byte pipe_number = nRF24L01p_status_pipe_ready();
      if (pipe_number <= 5) nRF24L01p_process_rx_payload(pipe_number);
      nRF24L01p_status_rx_ready_clear();
    }
  }

  // TODO: Implement advice from Appendix E for automatic
  //       channel switching using carrier detect.
  if (nRF24L01p_status_max_retries())
  {
    byte lost = nRF24L01p_packets_lost();
    printf("TX DROPPED with %d lost in total.\n", lost);

    nRF24L01p_tx_fifo_flush();
    nRF24L01p_status_max_retries_clear();
  }

  // TODO: Think this case through.
  if (nRF24L01p_status_tx_full())
  {
    printf("TX FIFO FULL\n");
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
int nRF24L01p_config_address(byte reg, long long unsigned int address)
{
  if (!(reg == nRF24L01p_REGISTER_RX_ADDR_P1 ||
        reg == nRF24L01p_REGISTER_RX_ADDR_P2 ||
        reg == nRF24L01p_REGISTER_RX_ADDR_P3 ||
        reg == nRF24L01p_REGISTER_RX_ADDR_P4 ||
        reg == nRF24L01p_REGISTER_RX_ADDR_P5 ||
        reg == nRF24L01p_REGISTER_TX_ADDR))
    return -1;

  // TODO: error checking for bad addresses.

  // TODO: This is broken, need to set 8 bit registers for
  // nRF24L01p_REGISTER_RX_ADDR_P(2-5)
  nRF24L01p_set_register40(reg, address);

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


// PIPE HELPER FUNCTIONS
////////////////////////


//
// nRF24L01p_payload_width implementation.
//
byte nRF24L01p_payload_width(byte pipe)
{
  switch (pipe)
  {
    case nRF24L01p_PIPE_0:
      return nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P0);
    case nRF24L01p_PIPE_1:
      return nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P1);
    case nRF24L01p_PIPE_2:
      return nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P2);
    case nRF24L01p_PIPE_3:
      return nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P3);
    case nRF24L01p_PIPE_4:
      return nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P4);
    default:
      return nRF24L01p_get_register8(nRF24L01p_REGISTER_RX_PW_P5);
  }
}


//
// nRF24L01p_address implementation.
//
long long unsigned int nRF24L01p_address(byte pipe)
{
  // TODO: This is broken, need to get 8 bit registers for
  // nRF24L01p_REGISTER_RX_ADDR_P(2-5)
  switch (pipe)
  {
    case nRF24L01p_PIPE_0:
      return nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P0);
    case nRF24L01p_PIPE_1:
      return nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P1);
    case nRF24L01p_PIPE_2:
      return nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P2);
    case nRF24L01p_PIPE_3:
      return nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P3);
    case nRF24L01p_PIPE_4:
      return nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P4);
    default:
      return nRF24L01p_get_register40(nRF24L01p_REGISTER_RX_ADDR_P5);
  }
}


//
// nRF24L01p_enable_pipe implementation.
//
int nRF24L01p_enable_pipe(byte pipe,
                          long long unsigned int address,
                          byte payload_width)
{
  switch (pipe)
  {
    case nRF24L01p_PIPE_0:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P0, TRUE);
      nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P0, address);
      nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P0,
                                     payload_width);
      return 0;
    case nRF24L01p_PIPE_1:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P1, TRUE);
      nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P1, address);
      nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P1,
                                     payload_width);
      return 0;
    case nRF24L01p_PIPE_2:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P2, TRUE);
      nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P2, address);
      nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P2,
                                     payload_width);
      return 0;
    case nRF24L01p_PIPE_3:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P3, TRUE);
      nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P3, address);
      nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P3,
                                     payload_width);
      return 0;
    case nRF24L01p_PIPE_4:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P4, TRUE);
      nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P4, address);
      nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P4,
                                     payload_width);
      return 0;
    case nRF24L01p_PIPE_5:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P5, TRUE);
      nRF24L01p_config_address(nRF24L01p_REGISTER_RX_ADDR_P5, address);
      nRF24L01p_config_payload_width(nRF24L01p_REGISTER_RX_PW_P5,
                                     payload_width);
      return 0;
  }

  return -1;
}


//
// nRF24L01p_disable_pipe implementation.
//
int nRF24L01p_disable_pipe(byte pipe)
{
  switch (pipe)
  {
    case nRF24L01p_PIPE_0:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P0, FALSE);
      return 0;
    case nRF24L01p_PIPE_1:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P1, FALSE);
      return 0;
    case nRF24L01p_PIPE_2:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P2, FALSE);
      return 0;
    case nRF24L01p_PIPE_3:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P3, FALSE);
      return 0;
    case nRF24L01p_PIPE_4:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P4, FALSE);
      return 0;
    case nRF24L01p_PIPE_5:
      nRF24L01p_config_rx(nRF24L01p_MASK_EN_RXADDR_ERX_P5, FALSE);
      return 0;
  }

  return -1;
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
int nRF24L01p_tx_fifo_write(const byte *payload, byte size)
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

  return size;
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


// OBSERVE
//////////


//
// nRF24L01p_packets_lost implementation.
//
byte nRF24L01p_packets_lost(void)
{
  return (nRF24L01p_get_register8(nRF24L01p_REGISTER_OBSERVE_TX) &
         nRF24L01p_MASK_OBSERVE_TX_PLOS_CNT) >> 4;
}


//
// nRF24L01p_packets_retransmitted implementation.
//
byte nRF24L01p_packets_retransmitted(void)
{
  return nRF24L01p_get_register8(nRF24L01p_REGISTER_OBSERVE_TX) &
         nRF24L01p_MASK_OBSERVE_TX_ARC_CNT;
}


// IO
/////


//
// nRF24L01p_read implementation.
//
int nRF24L01p_read(byte *restrict dst, size_t count, byte pipe)
{
  // TODO: Check enabled.

  if (!nRF24L01p_select(pipe))
    return -2;

  switch (pipe)
  {
    case nRF24L01p_PIPE_0:
      nRF24L01p_rx_pipes[0].data = dst;
      nRF24L01p_rx_pipes[0].remaining = count;
      break;
    case nRF24L01p_PIPE_1:
      nRF24L01p_rx_pipes[1].data = dst;
      nRF24L01p_rx_pipes[1].remaining = count;
      break;
    case nRF24L01p_PIPE_2:
      nRF24L01p_rx_pipes[2].data = dst;
      nRF24L01p_rx_pipes[2].remaining = count;
      break;
    case nRF24L01p_PIPE_3:
      nRF24L01p_rx_pipes[3].data = dst;
      nRF24L01p_rx_pipes[3].remaining = count;
      break;
    case nRF24L01p_PIPE_4:
      nRF24L01p_rx_pipes[4].data = dst;
      nRF24L01p_rx_pipes[4].remaining = count;
      break;
    case nRF24L01p_PIPE_5:
      nRF24L01p_rx_pipes[5].data = dst;
      nRF24L01p_rx_pipes[5].remaining = count;
      break;
  }

  // Start receiving if we weren't already.
  nRF24L01p_enable();

  return 0;
}


//
// nRF24L01p_write implementation.
//
int nRF24L01p_write(const byte *restrict src, size_t count, byte pipe)
{
  // TODO: Dynamic width.
  nRF24L01p_config_address(nRF24L01p_REGISTER_TX_ADDR, nRF24L01p_address(pipe));

  // TODO: Check enabled.
  // TODO: Check finished.

  nRF24L01p_tx_pipe.data = src;
  nRF24L01p_tx_pipe.remaining = count;
  nRF24L01p_tx_pipe.pipe = pipe;

  // TODO: Fill TX FIFO, then start transmitting.

  return 0;
}


//
// nRF24L01p_select implementation.
//
int nRF24L01p_select(byte pipe)
{
  // TODO: This needs to handle writes too.
  switch (pipe)
  {
    case nRF24L01p_PIPE_0:
      return nRF24L01p_rx_pipes[0].remaining == 0;
    case nRF24L01p_PIPE_1:
      return nRF24L01p_rx_pipes[1].remaining == 0;
    case nRF24L01p_PIPE_2:
      return nRF24L01p_rx_pipes[2].remaining == 0;
    case nRF24L01p_PIPE_3:
      return nRF24L01p_rx_pipes[3].remaining == 0;
    case nRF24L01p_PIPE_4:
      return nRF24L01p_rx_pipes[4].remaining == 0;
    case nRF24L01p_PIPE_5:
      return nRF24L01p_rx_pipes[5].remaining == 0;
  }

  return -1;
}


// Utility
//////////


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
