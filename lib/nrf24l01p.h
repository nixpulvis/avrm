#ifndef __nRF24L01p_H__
#define __nRF24L01p_H__

// FIFO
// TX three level, 32 byte FIFO
// RX three level, 32 byte FIFO
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

#define nRF24L01p_MASK_EN_AA_ENAA_P0 (0x1 << 0)
#define nRF24L01p_MASK_EN_AA_ENAA_P1 (0x1 << 1)
#define nRF24L01p_MASK_EN_AA_ENAA_P2 (0x1 << 2)
#define nRF24L01p_MASK_EN_AA_ENAA_P3 (0x1 << 3)
#define nRF24L01p_MASK_EN_AA_ENAA_P4 (0x1 << 4)
#define nRF24L01p_MASK_EN_AA_ENAA_P5 (0x1 << 5)

#define nRF24L01p_MASK_EN_RXADDR_ERX_P0 (0x1 << 0)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P1 (0x1 << 1)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P2 (0x1 << 2)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P3 (0x1 << 3)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P4 (0x1 << 4)
#define nRF24L01p_MASK_EN_RXADDR_ERX_P5 (0x1 << 5)

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

#define nRF24L01p_MASK_RX_PW_PX_RX_PW_PX 0x3F // PX represents any pipe.

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

#define nRF24L01p_VALUE_SETUP_AW_AW_3 (0x01 << 0)
#define nRF24L01p_VALUE_SETUP_AW_AW_4 (0x02 << 0)
#define nRF24L01p_VALUE_SETUP_AW_AW_5 (0x03 << 0)

// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_0  0x00
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_1  0x01
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_2  0x02
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_3  0x03
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_4  0x04
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_5  0x05
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_6  0x06
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_7  0x07
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_8  0x08
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_9  0x09
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_10 0x0A
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_11 0x0B
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_12 0x0C
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_13 0x0D
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_14 0x0E
// #define nRF24L01p_REGISTER_SETUP_RETR_ARC_15 0x0F

// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_250  0x00
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_500  0x01
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_750  0x02
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_1000 0x03
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_1250 0x04
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_1500 0x05
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_1750 0x06
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_2000 0x07
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_2250 0x08
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_2500 0x09
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_2750 0x0A
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_3000 0x0B
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_3250 0x0C
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_3500 0x0D
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_3750 0x0E
// #define nRF24L01p_REGISTER_SETUP_RETR_ARD_4000 0x0F

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

// Defaults

#define nRF24L01p_DEFAULT_ADDRESS ((unsigned long long) 0x1337)

#include "avr.h"

// TODO: Determine the proper interface here.
void nRF24L01p_init(void);

// Configuration
////////////////

// nRF24L01p_config_transceiver_mode
// Set the device to be either PRX or PTX. Both
// modes can send and receive, however PTX is essentially
// the master.
//
// value - Either nRF24L01p_VALUE_CONFIG_PRIM_TX or
//                nRF24L01p_VALUE_CONFIG_PRIM_RX
//
int nRF24L01p_config_transceiver_mode(byte value);

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

// nRF24L01p_config_address
//
int nRF24L01p_config_address(byte address_register,
                             long long unsigned int address);

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

// Control
//////////

// nRF24L01p_power_up
// Set the CONFIG_PWR_UP to 1, taking the device out of
// Power Down, into Standby-I
//
void nRF24L01p_power_up(void);

// nRF24L01p_power_down
// Set the CONFIG_PWR_UP to 0, taking the device in to
// Power Down.
//
void nRF24L01p_power_down(void);

// nRF24L01p_enable
// Set the CE pin high, putting the device into either
// RX Mode or TX Mode depending on the value of CONFIG_PRIM_RX.
//
void nRF24L01p_enable(void);

// nRF24L01p_disable
// Set the CE pin low, putting the device into Standby-I.
//
void nRF24L01p_disable(void);

// IO
/////

// nRF24L01p_read
// Read data over the air.
//
// pipe - The data pipe to read from (0-5).
// dst - Pointer to storage.
// count - Number of bytes to read (1-32).
//
// Returns the number of bytes actually read. -1 on error.
//
int nRF24L01p_read(char *dst, byte count, byte pipe);

// nRF24L01p_write
// Write data over the air.
//
// src - Pointer to storage to write.
// count - Number of bytes to write (1-32).
//
// Returns the number of bytes actually written. -1 on error.
//
int nRF24L01p_write(char *src, byte count);

// Utility
//////////

// nRF24L01p_get_register8
// Get the value of an 8 register. This operation should only
// be done while in Power Down or Standby-I. TODO: verify.
//
// address - The address of the register to read.
//
// Returns the value of the register.
//
byte nRF24L01p_get_register8(byte address);

// nRF24L01p_set_register8
// Set the value of an 8 register. This operation should only
// be done while in Power Down or Standby-I. TODO: verify.
//
// address - The address of the register to write.
// data - The value to set the register to.
//
void nRF24L01p_set_register8(byte address, byte data);

// nRF24L01p_set_register8_bits
// Set the bits of a masked region of a register. It's important
// to note that the value must be aligned properly. For example
// with a mask of 00001100 the value would need to be 00001000 to
// update the left most bit within the range of the mask.
//
// address - The address of the register to write.
// mask - A value with bits set to 1 where we are writing.
// value - The value to write into the masked region.
//
void nRF24L01p_set_register8_bits(byte address, byte mask, byte value);

// nRF24L01p_get_register40
// Get the value of a 40 bit register. This operation should only
// be done while in Power Down or Standby-I. TODO: verify.
//
// address - The address of the register to read.
//
// Returns the value of the register.
//
long long unsigned int nRF24L01p_get_register40(byte address);

// nRF24L01p_set_register40
// Set the value of a 40 bit register. This operation should only
// be done while in Power Down or Standby-I. TODO: verify.
//
// address - The address of the register to write.
// data - The value to set the register to.
//
void nRF24L01p_set_register40(byte address, long long unsigned int data);

#endif
