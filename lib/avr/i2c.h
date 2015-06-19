#ifndef __AVR_I2C_H__
#define __AVR_I2C_H__

#include <avr.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/twi.h>

// I2C Functions
////////////////

// I2C clock freqencies.
#define I2C_SCL_100KHZ 100000UL
#define I2C_SCL_400KHZ 400000UL

#define I2C_READ  0x01
#define I2C_WRITE 0x00

// i2c_init
int i2c_init(long long frequency);

// i2c_start
byte i2c_start(byte address, byte config);

// i2c_write
void i2c_write(byte data);

// i2c_read_ack
byte i2c_read_ack(void);

// i2c_read_nack
byte i2c_read_nack(void);

// i2c_stop
void i2c_stop(void);

#endif
