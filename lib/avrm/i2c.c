#include "i2c.h"

int i2c_init(long long frequency)
{
  if (frequency != I2C_SCL_100KHZ &&
      frequency != I2C_SCL_400KHZ)
    return -1;
  // TODO: Set prescaler to 0.
  TWBR = ((F_CPU / frequency) - 16) / 2;
  return 0;
}

byte i2c_start(byte address, byte config){
  // reset TWI control register
  TWCR = 0;
  // transmit START condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  // wait for end of transmission
  while (!(TWCR & (1 << TWINT)));

  // check if the start condition was successfully transmitted
  if ((TWSR & 0xF8) != TW_START)
    return 1;

  // load slave address into data register
  TWDR = ((address << 1) & 0xFE) | (config & 0x01);
  // start transmission of address
  TWCR = (1 << TWINT) | (1 << TWEN);
  // wait for end of transmission
  while (!(TWCR & (1 << TWINT)));

  // check if the device has acknowledged the READ / WRITE mode
  byte twst = TW_STATUS & 0xF8;
  if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
    return 1;

  return 0;
}

void i2c_write(byte data){
  // load data into data register
  TWDR = data;
  // start transmission of data
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while (!(TWCR & (1 << TWINT)));

  // TODO: ?
  // if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
  //   return;

  return;
}

byte i2c_read_ack(void){
  // start TWI module and acknowledge data after reception
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
  // wait for end of transmission
  while (!(TWCR & (1 << TWINT)));
  // return received data from TWDR
  return TWDR;
}

byte i2c_read_nack(void){
  // start receiving without acknowledging reception
  TWCR = (1 << TWINT) | (1 << TWEN);
  // wait for end of transmission
  while (!(TWCR & (1 << TWINT)));
  // return received data from TWDR
  return TWDR;
}

void i2c_stop(void){
  // transmit STOP condition
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
