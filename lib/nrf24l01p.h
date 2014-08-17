#ifndef __nRF24L01p_H__
#define __nRF24L01p_H__

// FIFO

#define nRF24L01p_FIFO_TX_COUNT 3
#define nRF24L01p_FIFO_RX_COUNT 3
#define nRF24L01p_FIFO_TX_SIZE  32
#define nRF24L01p_FIFO_RX_SIZE  32

// SPI

#define nRF24L01p_SPI_R_REGISTER         0x00
#define nRF24L01p_SPI_W_REGISTER         0x20
#define nRF24L01p_SPI_R_RX_PAYLOAD       0x61
#define nRF24L01p_SPI_W_TX_PAYLOAD       0xA0
#define nRF24L01p_SPI_FLUSH_TX           0xE1
#define nRF24L01p_SPI_FLUSH_RX           0xE2
#define nRF24L01p_SPI_REUSE_TX_PL        0xE3
#define nRF24L01p_SPI_ACTIVATE           0x50
#define nRF24L01p_SPI_R_RX_PL_WID        0x60
#define nRF24L01p_SPI_W_ACK_PAYLOAD      0xA8
#define nRF24L01p_SPI_W_TX_PAYLOAD_NOACK 0xB0 // data-sheet only has 7 bits...
#define nRF24L01p_SPI_NOP                0xFF

#define nRF24L01p_SPI_RW_REGISTER_MASK   0x1F
#define nRF24L01p_SPI_W_ACK_PAYLOAD_MASK 0x07

#define nRF24L01p_SPI_ACTIVATE_MAGIC 0x73

// Registers

#define nRF24L01p_REGISTER_CONFIG      0x00
#define nRF24L01p_REGISTER_EN_AA       0x01
#define nRF24L01p_REGISTER_EN_RXADDR   0x02
#define nRF24L01p_REGISTER_SETUP_AW    0x03
#define nRF24L01p_REGISTER_SETUP_RETR  0x04
#define nRF24L01p_REGISTER_RF_CH       0x05
#define nRF24L01p_REGISTER_RF_SETUP    0x06
#define nRF24L01p_REGISTER_STATUS      0x07
#define nRF24L01p_REGISTER_OBSERVE_TX  0x08
#define nRF24L01p_REGISTER_CD          0x09
#define nRF24L01p_REGISTER_RX_ADDR_P0  0x0A
#define nRF24L01p_REGISTER_RX_ADDR_P1  0x0B
#define nRF24L01p_REGISTER_RX_ADDR_P2  0x0C
#define nRF24L01p_REGISTER_RX_ADDR_P3  0x0D
#define nRF24L01p_REGISTER_RX_ADDR_P4  0x0E
#define nRF24L01p_REGISTER_RX_ADDR_P5  0x0F
#define nRF24L01p_REGISTER_TX_ADDR     0x10
#define nRF24L01p_REGISTER_RX_PW_P0    0x11
#define nRF24L01p_REGISTER_RX_PW_P1    0x12
#define nRF24L01p_REGISTER_RX_PW_P2    0x13
#define nRF24L01p_REGISTER_RX_PW_P3    0x14
#define nRF24L01p_REGISTER_RX_PW_P4    0x15
#define nRF24L01p_REGISTER_RX_PW_P5    0x16
#define nRF24L01p_REGISTER_FIFO_STATUS 0x17
#define nRF24L01p_REGISTER_DYNPD       0x1C
#define nRF24L01p_REGISTER_FEATURE     0x1D

// Register bit masks

#define nRF24L01p_MASK_CONFIG_PRIM_RX     (0x1 << 0)
#define nRF24L01p_MASK_CONFIG_PWR_UP      (0x1 << 1)
#define nRF24L01p_MASK_CONFIG_CRCO        (0x1 << 2)
#define nRF24L01p_MASK_CONFIG_EN_CRC      (0x1 << 3)
#define nRF24L01p_MASK_CONFIG_MASK_MAX_RT (0x1 << 4)
#define nRF24L01p_MASK_CONFIG_MASK_TX_DS  (0x1 << 5)
#define nRF24L01p_MASK_CONFIG_MASK_RX_DR  (0x1 << 6)

#define nRF24L01p_MASK_EN_AA_ENAA_P0  (0x1 << 0)
#define nRF24L01p_MASK_EN_AA_ENAA_P1  (0x1 << 1)
#define nRF24L01p_MASK_EN_AA_ENAA_P2  (0x1 << 2)
#define nRF24L01p_MASK_EN_AA_ENAA_P3  (0x1 << 3)
#define nRF24L01p_MASK_EN_AA_ENAA_P4  (0x1 << 4)
#define nRF24L01p_MASK_EN_AA_ENAA_P5  (0x1 << 5)
#define nRF24L01p_MASK_EN_AA_ENAA_ALL (0x3F << 0)

#define nRF24L01p_MASK_EN_RXADDR_ERX_P0  (0x1 << 0)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P1  (0x1 << 1)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P2  (0x1 << 2)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P3  (0x1 << 3)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P4  (0x1 << 4)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P5  (0x1 << 5)
#define nRF24L01p_MASK_EN_RXADDR_ERX_ALL (0x3F << 0)

#define nRF24L01p_MASK_SETUP_AW_AW 0x03

#define nRF24L01p_MASK_SETUP_RETR_ARC 0x0F
#define nRF24L01p_MASK_SETUP_RETR_ARD 0xF0

#define nRF24L01p_MASK_RF_CH_RF_CH 0x7F

#define nRF24L01p_MASK_RF_SETUP_LNA_HCURR (0x1 << 0)
#define nRF24L01p_MASK_RF_SETUP_RF_PWR    (0x3 << 1)
#define nRF24L01p_MASK_RF_SETUP_RF_DR     (0x1 << 3)
#define nRF24L01p_MASK_RF_SETUP_PLL_LOCK  (0x1 << 4)

#define nRF24L01p_MASK_STATUS_TX_FULL (0x1 << 0)
#define nRF24L01p_MASK_STATUS_RX_P_NO (0x7 << 1)
#define nRF24L01p_MASK_STATUS_MAX_RT  (0x1 << 4)
#define nRF24L01p_MASK_STATUS_TX_DS   (0x1 << 5)
#define nRF24L01p_MASK_STATUS_RX_DR   (0x1 << 6)

#define nRF24L01p_MASK_OBSERVE_TX_ARC_CNT  0x0F
#define nRF24L01p_MASK_OBSERVE_TX_PLOS_CNT 0xF0

#define nRF24L01p_MASK_CD_CD 0x1

#define nRF24L01p_MASK_RX_PW 0x3F

#define nRF24L01p_MASK_FIFO_STATUS_RX_EMPTY (0x1 << 0)
#define nRF24L01p_MASK_FIFO_STATUS_RX_FULL  (0x1 << 1)
#define nRF24L01p_MASK_FIFO_STATUS_TX_EMPTY (0x1 << 4)
#define nRF24L01p_MASK_FIFO_STATUS_TX_FULL  (0x1 << 5)
#define nRF24L01p_MASK_FIFO_STATUS_TX_REUSE (0x1 << 6)

#define nRF24L01p_MASK_DYNPD_DPL_P0 (0x1 << 0)
#define nRF24L01p_MASK_DYNPD_DPL_P1 (0x1 << 1)
#define nRF24L01p_MASK_DYNPD_DPL_P2 (0x1 << 2)
#define nRF24L01p_MASK_DYNPD_DPL_P3 (0x1 << 3)
#define nRF24L01p_MASK_DYNPD_DPL_P4 (0x1 << 4)
#define nRF24L01p_MASK_DYNPD_DPL_P5 (0x1 << 5)

#define nRF24L01p_MASK_FEATURE_EN_DYN_ACK (0x1 << 0)
#define nRF24L01p_MASK_FEATURE_EN_ACK_PAY (0x1 << 1)
#define nRF24L01p_MASK_FEATURE_EN_DPL     (0x1 << 2)

// Register values

#define nRF24L01p_VALUE_CONFIG_PRIM_TX (0x0 << 0)
#define nRF24L01p_VALUE_CONFIG_PRIM_RX (0x1 << 0)

#define nRF24L01p_VALUE_CONFIG_PWR_DOWN (0x0 << 1)
#define nRF24L01p_VALUE_CONFIG_PWR_UP   (0x1 << 1)

#define nRF24L01p_VALUE_CONFIG_CRCO_1 (0x0 << 2)
#define nRF24L01p_VALUE_CONFIG_CRCO_2 (0x1 << 2)

#define nRF24L01p_VALUE_CONFIG_CRC_DISABLE (0x0 << 3)
#define nRF24L01p_VALUE_CONFIG_CRC_ENABLE  (0x1 << 3)

#define nRF24L01p_VALUE_SETUP_AW_AW_3 (0x01 << 0)
#define nRF24L01p_VALUE_SETUP_AW_AW_4 (0x02 << 0)
#define nRF24L01p_VALUE_SETUP_AW_AW_5 (0x03 << 0)

#define nRF24L01p_VALUE_SETUP_RETR_ARC_0  (0x00 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_1  (0x01 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_2  (0x02 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_3  (0x03 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_4  (0x04 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_5  (0x05 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_6  (0x06 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_7  (0x07 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_8  (0x08 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_9  (0x09 << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_10 (0x0A << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_11 (0x0B << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_12 (0x0C << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_13 (0x0D << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_14 (0x0E << 0)
#define nRF24L01p_VALUE_SETUP_RETR_ARC_15 (0x0F << 0)

#define nRF24L01p_VALUE_SETUP_RETR_ARD_250  (0x00 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_500  (0x01 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_750  (0x02 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_1000 (0x03 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_1250 (0x04 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_1500 (0x05 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_1750 (0x06 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_2000 (0x07 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_2250 (0x08 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_2500 (0x09 << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_2750 (0x0A << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_3000 (0x0B << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_3250 (0x0C << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_3500 (0x0D << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_3750 (0x0E << 4)
#define nRF24L01p_VALUE_SETUP_RETR_ARD_4000 (0x0F << 4)

#define nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_18dBm (0x0 << 1)
#define nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_12dBm (0x1 << 1)
#define nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_6dBm  (0x2 << 1)
#define nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm      (0x3 << 1)

#define nRF24L01p_VALUE_RF_SETUP_RF_DR_1Mbps (0x0 << 3)
#define nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps (0x1 << 3)

// Timings

#define nRF24L01p_TIMING_INITIAL_US   10300 // 10.3ms
#define nRF24L01p_TIMING_TPD2STBY_US  1500  // 1.5ms
#define nRF24L01p_TIMING_TSTBY2A_US   130   // 130us
#define nRF24L01p_TIMING_THCE_US      10    // 10us
#define nRF24L01p_TIMING_TPECE2CSN_US 4     // 4us

// Pipes

#define nRF24L01p_PIPE_0 (1 << 0)
#define nRF24L01p_PIPE_1 (1 << 1)
#define nRF24L01p_PIPE_2 (1 << 2)
#define nRF24L01p_PIPE_3 (1 << 3)
#define nRF24L01p_PIPE_4 (1 << 4)
#define nRF24L01p_PIPE_5 (1 << 5)

// Defaults

#define nRF24L01p_DEFAULT_ADDRESS 0xE7E7E7E7E7

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "avr.h"

// nRF24L01p_init
// Setup the default configuration for the device.
//
// For the sake of brevity I'll let you read the source
// code to see what the defaults are.
//
// ce - CE pin. TODO: figure out how to represent this.
// irq - IRQ pin. TODO: figure out how to represent this.
//
// Returns 0 if successful, -1 otherwise.
//
int nRF24L01p_init(int ce, int irq);

// Configuration
////////////////

// CONFIG

// nRF24L01p_config_rx_dr_interrupt
// Enables or disables the RX data received interrupt.
//
int nRF24L01p_config_rx_dr_interrupt(bool value);

// nRF24L01p_config_tx_ds_interrupt
// Enables or disables the TX data sent interrupt.
//
int nRF24L01p_config_tx_ds_interrupt(bool value);

// nRF24L01p_config_max_rt_interrupt
// Enables or disables the max retries interrupt.
//
int nRF24L01p_config_max_rt_interrupt(bool value);

// nRF24L01p_config_crc
// Sets the device to use or not use the CRC.
//
// value - Either nRF24L01p_VALUE_CONFIG_CRC_ENABLE or
//                nRF24L01p_VALUE_CONFIG_CRC_DISABLE
//
int nRF24L01p_config_crc(byte value);

// nRF24L01p_config_crc_count
// Set the number of bits for the CRC.
//
// value - Either nRF24L01p_VALUE_CONFIG_CRCO_1 or
//                nRF24L01p_VALUE_CONFIG_CRCO_2
//
int nRF24L01p_config_crc_count(byte value);

// nRF24L01p_config_power
// Set the device into either Power Down or into Standby-I.
//
// value - Either nRF24L01p_VALUE_CONFIG_PWR_UP or
//                nRF24L01p_VALUE_CONFIG_PWR_DOWN
//
int nRF24L01p_config_power(byte value);

// nRF24L01p_config_transceiver_mode
// Set the device to be either PRX or PTX. Both
// modes can send and receive, however PTX is essentially
// the master.
//
// value - Either nRF24L01p_VALUE_CONFIG_PRIM_TX or
//                nRF24L01p_VALUE_CONFIG_PRIM_RX
//
int nRF24L01p_config_transceiver_mode(byte value);

// EN_AA

// nRF24L01p_config_auto_ack
// Enable or disable auto acknowledgment of packets.
//
// mask - One of nRF24L01p_MASK_EN_AA_ENAA_P(0-5) or
//               nRF24L01p_MASK_EN_AA_ENAA_ALL.
// value - Either TRUE or FALSE.
//
int nRF24L01p_config_auto_ack(byte mask, bool value);

// EN_RXADDR

// nRF24L01p_config_rx
// Enable or disable RX pipes.
//
// mask - One of nRF24L01p_MASK_EN_RXADDR_ERX_P(0-5) or
//               nRF24L01p_MASK_EN_RXADDR_ERX_ALL
// value - Either TRUE or FALSE.
//
int nRF24L01p_config_rx(byte mask, bool value);

// SETUP_AW

// nRF24L01p_config_address_width
// Set the width for the address that identifies packets
// so the receiver can map them to the correct pipe, or
// drop them.
//
// value - One of nRF24L01p_VALUE_SETUP_AW_AW_3 or
//                nRF24L01p_VALUE_SETUP_AW_AW_4 or
//                nRF24L01p_VALUE_SETUP_AW_AW_5
//
int nRF24L01p_config_address_width(byte value);

// SETUP_RETR

// nRF24L01p_config_retransmit_delay
// Set the delay from the end of last transmission to the
// start of next transmission for auto retries.
//
// value - One of nRF24L01p_VALUE_SETUP_RETR_ARD_250 through
//                nRF24L01p_VALUE_SETUP_RETR_ARD_4000
//
int nRF24L01p_config_retransmit_delay(byte value);

// nRF24L01p_config_retransmit_count
// Set the number of auto retries before giving up.
//
// value - One of nRF24L01p_VALUE_SETUP_RETR_ARC_0 through
//                nRF24L01p_VALUE_SETUP_RETR_ARC_15
//
int nRF24L01p_config_retransmit_count(byte value);

// RF_CH

// nRF24L01p_config_channel
// Sets the frequency to 2400 + value(MHz), both RX and
// TX must share the same channel.
//
// value - Megahertz offset from 2.4GHz, (0-127)
//
int nRF24L01p_config_channel(byte value);

// RF_SETUP

// TODO: PLL_LOCK

// nRF24L01p_config_air_data_rate
// Set the over air data rate to either 1Mbps or 2Mbps.
// Higher data rate uses less current, and avoids more
// collisions.
//
// value - Either nRF24L01p_VALUE_RF_SETUP_RF_DR_1Mbps or
//                nRF24L01p_VALUE_RF_SETUP_RF_DR_2Mbps
//
int nRF24L01p_config_air_data_rate(byte value);

// nRF24L01p_config_output_power
// Set the RF power.
//
// value - One of nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_18dBm or
//                nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_12dBm or
//                nRF24L01p_VALUE_RF_SETUP_RF_PWR_NEG_6dBm or
//                nRF24L01p_VALUE_RF_SETUP_RF_PWR_0dBm
//
int nRF24L01p_config_output_power(byte value);

// TODO: LNA_HCURR

// STATUS - Not used for configuration, see status functions.
// OBSERVE_TX - Not used for configuration, see observe functions.

// CD

// TODO: CD

// RX_ADDR_P(0-5) and TX_ADDR

// nRF24L01p_config_address
// Set the address of a given RX pipe, or TX. The amount
// of the given address that is used for the address is based
// on the setting on the address width.
//
// Using addresses with long sequences of 0s or 1s is inadvisable,
// as it can conflict with radio preambles.
//
// reg - nRF24L01p_REGISTER_RX_ADDR_P0 or
//       nRF24L01p_REGISTER_RX_ADDR_P1 or
//       nRF24L01p_REGISTER_RX_ADDR_P2 or
//       nRF24L01p_REGISTER_RX_ADDR_P3 or
//       nRF24L01p_REGISTER_RX_ADDR_P4 or
//       nRF24L01p_REGISTER_RX_ADDR_P5 or
//       nRF24L01p_REGISTER_TX_ADDR.
// address - A value to set the address to.
//
int nRF24L01p_config_address(byte reg, long long unsigned int address);

// RX_PW_P(0-5)

// nRF24L01p_config_payload_width
// Set the payload width for use in RX mode. This value on a
// pipe MUST match the width of the data sent from the
// transmitter.
//
// reg - One of nRF24L01p_REGISTER_RX_PW_P0 or
//              nRF24L01p_REGISTER_RX_PW_P1 or
//              nRF24L01p_REGISTER_RX_PW_P2 or
//              nRF24L01p_REGISTER_RX_PW_P3 or
//              nRF24L01p_REGISTER_RX_PW_P4 or
//              nRF24L01p_REGISTER_RX_PW_P5
// width - The number of bytes in the payload for this pipe
//         value can be between 0 and 32.
//
int nRF24L01p_config_payload_width(byte reg, byte width);

// FIFO_STATUS - Not used for configuration, see fifo_status functions.

// DYNPD

// TODO: DPL_P(0-5)

// FEATURE

// TODO: EN_DPL
// TODO: EN_ACK_PAY
// TODO: EN_DYN_ACK

// PIPE HELPER FUNCTIONS
////////////////////////

// nRF24L01p_payload_width
// Returns the number of bytes the given pipe's payloads are.
//
// pipe - One of nRF24L01p_PIPE_0 or
//               nRF24L01p_PIPE_1 or
//               nRF24L01p_PIPE_2 or
//               nRF24L01p_PIPE_3 or
//               nRF24L01p_PIPE_4 or
//               nRF24L01p_PIPE_5.
//
byte nRF24L01p_payload_width(byte pipe);

// nRF24L01p_address
// Returns the address of the given pipe.
//
// pipe - One of nRF24L01p_PIPE_0 or
//               nRF24L01p_PIPE_1 or
//               nRF24L01p_PIPE_2 or
//               nRF24L01p_PIPE_3 or
//               nRF24L01p_PIPE_4 or
//               nRF24L01p_PIPE_5.
//
long long unsigned int nRF24L01p_address(byte pipe);

// nRF24L01p_enable_pipe
// Enable a pipe to read from the given address.
//
// TODO:
// It's worth mentioning that this function function allocates
// a receiving buffer, so don't use more pipes than you need.
//
// pipe - One of nRF24L01p_PIPE_0 or
//               nRF24L01p_PIPE_1 or
//               nRF24L01p_PIPE_2 or
//               nRF24L01p_PIPE_3 or
//               nRF24L01p_PIPE_4 or
//               nRF24L01p_PIPE_5.
// address - The address to read from, matching the address set
//           by the TX using nRF24L01p_config_address.
// payload_width - A value between 1 and 32. TODO: Add dynamic option.
//
int nRF24L01p_enable_pipe(byte pipe,
                          long long unsigned int address,
                          byte payload_width);

// nRF24L01p_disable_pipe
// Disable a pipe, freeing allocated memory. This function does
// not reset the address of the disabled pipe.
//
// pipe - One of nRF24L01p_PIPE_0 or
//               nRF24L01p_PIPE_1 or
//               nRF24L01p_PIPE_2 or
//               nRF24L01p_PIPE_3 or
//               nRF24L01p_PIPE_4 or
//               nRF24L01p_PIPE_5.
//
int nRF24L01p_disable_pipe(byte pipe);

// STATUS
/////////

// nRF24L01p_fetch_status
// Updates the values stored in nRF24L01p_status.
//
void nRF24L01p_status_fetch(void);

// nRF24L01p_status_rx_ready
// Returns TRUE when there is a payload in the RX FIFO.
//
bool nRF24L01p_status_rx_ready(void);

// nRF24L01p_status_rx_ready_clear
// Clear the RX data ready interrupt.
//
void nRF24L01p_status_rx_ready_clear(void);

// nRF24L01p_status_tx_sent
// Returns TRUE when a payload was sent, if AUTO_ACK is
// set only asserted when acknowledgment is received.
//
bool nRF24L01p_status_tx_sent(void);

// nRF24L01p_status_tx_sent_clear
// Clear the TX data sent interrupt.
//
void nRF24L01p_status_tx_sent_clear(void);

// nRF24L01p_status_max_retries
// Returns TRUE when the device hits max number of retires.
// When this happens the interrupt must be cleared before
// more packets can be sent.
//
bool nRF24L01p_status_max_retries(void);

// nRF24L01p_status_max_retries_clear
// Clear the max retries interrupt.
//
void nRF24L01p_status_max_retries_clear(void);

// nRF24L01p_status_pipe_ready
// Returns the pipe number of the top payload in the RX FIFO.
//
byte nRF24L01p_status_pipe_ready(void);

// nRF24L01p_status_tx_full
// Returns TRUE when the TX FIFO is full.
//
bool nRF24L01p_status_tx_full(void);

// FIFO
///////

// nRF24L01p_tx_fifo_is_reuse
// Returns true when the device is configured to reuse
// the last transmitted data packet. Sending continuously,
// as long as CE is high.
//
bool nRF24L01p_tx_fifo_is_reuse(void);

// nRF24L01p_tx_fifo_is_full
// Return true when there is no room in the TX FIFO,
// false when there is room.
//
bool nRF24L01p_tx_fifo_is_full(void);

// nRF24L01p_tx_fifo_is_empty
// Return true when there is nothing in the TX FIFO.
//
bool nRF24L01p_tx_fifo_is_empty(void);

// nRF24L01p_tx_fifo_write
// Write a payload to the TX FIFO. This payload must be
// equivalent to the value set to nRF24L01p_config_payload_width
// unless dynamic payload width is enabled.
//
// payload - An array of bytes to write to the TX FIFO.
// size - The number of bytes from payload to write.
//        (0-nRF24L01p_FIFO_TX_SIZE)
//
// Returns the number of bytes written.
//
int nRF24L01p_tx_fifo_write(const byte *payload, byte size);

// nRF24L01p_tx_fifo_flush
// Flush the TX FIFO, clearing it of all payloads.
void nRF24L01p_tx_fifo_flush(void);

// nRF24L01p_rx_fifo_is_full
// Return true when there is no room in the RX FIFO,
// false when there is room.
//
bool nRF24L01p_rx_fifo_is_full(void);

// nRF24L01p_rx_fifo_is_empty
// Return true when there is nothing in the RX FIFO.
//
bool nRF24L01p_rx_fifo_is_empty(void);

// nRF24L01p_rx_fifo_read
// Read the top payload from the RX FIFO.
//
// payload - Buffer to read data into.
// size - number of bytes to read. (0-nRF24L01p_FIFO_RX_SIZE)
//
// Returns number of bytes read.
//
int nRF24L01p_rx_fifo_read(byte *payload, byte size);

// nRF24L01p_rx_fifo_flush
// Flush the RX FIFO, clearing it of all payloads.
//
void nRF24L01p_rx_fifo_flush(void);

// OBSERVE
//////////

// nRF24L01p_packets_lost
// Returns the number of packets lost since last setting of
// the radio channel.
//
// This is a 4 bit field in the OBSERVE_TX register, so the
// value maxes out at 15.
//
byte nRF24L01p_packets_lost(void);

// nRF24L01p_packets_retransmitted
// Returns the number of packets retransmitted. This value
// is reset at the start of a new packet transmission.
//
byte nRF24L01p_packets_retransmitted(void);

// IO
/////

// nRF24L01p_enable
// Set the CE pin high, putting the device into either
// RX Mode or TX Mode depending on the value of CONFIG_PRIM_RX.
//
void nRF24L01p_enable(void);

// nRF24L01p_disable
// Set the CE pin low, putting the device into Standby-I.
//
void nRF24L01p_disable(void);

// nRF24L01p_read_sync
// Read data over the air. Blocking until the data is all
// read.
//
// TODO: Timeouts... etc.
//
// dst - Pointer to data.
// count - Number of bytes to read.
// pipe - One of nRF24L01p_PIPE_0 or
//               nRF24L01p_PIPE_1 or
//               nRF24L01p_PIPE_2 or
//               nRF24L01p_PIPE_3 or
//               nRF24L01p_PIPE_4 or
//               nRF24L01p_PIPE_5.
//
// Returns the number of bytes actually read. -1 on error.
//
// TODO: Add pipe argument.
int nRF24L01p_read_sync(byte *restrict dst, size_t count, byte pipe);

// nRF24L01p_write
// Write data over the air. This function doesn't block while
// the data is being sent, use nRF24L01p_write_poll() to
// get the status of the write.
//
// Calls to nRF24L01p_write while there is a non finished
// write in progress will fail and return -1.
//
// src - Pointer to the data to write.
// count - Number of bytes to write.
// pipe - One of nRF24L01p_PIPE_0 or
//               nRF24L01p_PIPE_1 or
//               nRF24L01p_PIPE_2 or
//               nRF24L01p_PIPE_3 or
//               nRF24L01p_PIPE_4 or
//               nRF24L01p_PIPE_5.
//
int nRF24L01p_write(const byte *restrict src, size_t count, byte pipe);

// nRF24L01p_write_sync
// Write data over the air. Blocking until the data is all
// sent.
//
// TODO: Timeouts... etc.
//
// src - Pointer to data to write.
// count - Number of bytes to write.
//
// Returns the number of bytes actually written. -1 on error.
//
int nRF24L01p_write_sync(const byte *restrict src, size_t count, byte pipe);

// Utility
//////////

// nRF24L01p_get_register8
// Get the value of an 8 register.
//
// address - The address of the register to read.
//
// Returns the value of the register.
//
byte nRF24L01p_get_register8(byte address);

// nRF24L01p_set_register8
// Set the value of an 8 register. This operation should only
// be done while in Power Down or Standby-I.
//
// Note: Addresses (18-1B) are reserved, and cannot be set
//       using these functions.
//
// address - The address of the register to write.
// data - The value to set the register to.
//
// Returns 0 when successful, -1 otherwise.
//
int nRF24L01p_set_register8(byte address, byte data);

// nRF24L01p_set_register8_bits
// Set the bits of a masked region of a register. It's important
// to note that the value must be aligned properly. For example
// with a mask of 00001100 the value would need to be 00001000 to
// update the left most bit within the range of the mask. This
// operation should only be done while in Power Down or Standby-I.
//
// Note: Addresses (18-1B) are reserved, and cannot be set
//       using these functions.
//
// address - The address of the register to write.
// mask - A value with bits set to 1 where we are writing.
// value - The value to write into the masked region.
//
// Returns 0 when successful, -1 otherwise.
//
int nRF24L01p_set_register8_bits(byte address, byte mask, byte value);

// nRF24L01p_get_register40
// Get the value of a 40 bit register.
//
// address - The address of the register to read.
//
// Returns the value of the register.
//
long long unsigned int nRF24L01p_get_register40(byte address);

// nRF24L01p_set_register40
// Set the value of a 40 bit register. This operation should only
// be done while in Power Down or Standby-I.
//
// Note: Addresses (18-1B) are reserved, and cannot be set
//       using these functions.
//
// address - The address of the register to write.
// data - The value to set the register to.
//
// Returns 0 when successful, -1 otherwise.
//
int nRF24L01p_set_register40(byte address, long long unsigned int data);

#endif
