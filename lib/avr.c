#include "avr.h"

// General Functions
////////////////////

//
// printb implementation.
//
void printb(void const * const ptr, size_t const size)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--)
  for (j = 7; j >= 0; j--)
  {
    byte = b[i] & (1 << j);
    byte >>= j;
    printf("%u", byte);
  }
}

//
// bcd_to_dec implementation.
//
byte bcd_to_dec(byte data)
{
  // TODO: Bound checking.
  return (((data & 0xF0) >> 4) * 10) + (data & 0x0F);
}

//
// dec_to_bcd implementation.
//
byte dec_to_bcd(byte data)
{
  // TODO: Bound checking.
  return (((data / 10) << 4) & 0xF0) | (data % 10);
}


// UART Functions
/////////////////

// Forward declarations of needed read and write functions.
int uart_put(char data, FILE *stream);
int uart_get(FILE *stream);

//
// uart_init implementation.
//
void uart_init(void) {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  // Double transmission speed.
  UCSR0A |= _BV(U2X0);
#else
  // Not double transmission speed.
  UCSR0A &= ~(_BV(U2X0));
#endif
  // 8-bit data.
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  // Enable RX and TX.
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);

  // The UART io.
  static FILE uart_io = FDEV_SETUP_STREAM(uart_put, uart_get, _FDEV_SETUP_RW);

  // Set this UART as stdout and stdin.
  stdout = &uart_io;
  stdin = &uart_io;
}


//
// private uart_put implementation.
//
int uart_put(char data, FILE *stream) {
  // force adding a carriage return after newline.
  if (data == '\n')
    uart_put('\r', stream);
  // Wait until data register is empty.
  loop_until_bit_is_set(UCSR0A, UDRE0);
  // Send data.
  UDR0 = data;
  return 0;
}


//
// private uart_get implementation.
//
int uart_get(FILE *stream) {
  // Wait for data.
  loop_until_bit_is_set(UCSR0A, RXC0);
  // Return the data.
  return UDR0;
}


// SPI Functions
////////////////

//
// spi_init implementation.
//
void spi_init(void)
{
  // SS is output.
  PORTB |= _BV(PORTB2);
  DDRB |= _BV(DDB2);

  // Set MOSI, SCK as output.
  DDRB |= _BV(DDB3);
  DDRB |= _BV(DDB5);

  // Enable SPI, and set as master.
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);
}


//
// spi_start implementation.
//
void spi_start(void)
{
  PORTB &= ~_BV(PORTB2);
}


//
// spi_end implementation.
//
void spi_end(void)
{
  PORTB |= _BV(PORTB2);
}


//
// spi_transfer implementation.
//
byte spi_transfer(byte data)
{
  // Load data into the buffer.
  SPDR = data;

  // Wait until transmission complete.
  while (!(SPSR & _BV(SPIF)));

  // Return received data.
  return SPDR;
}

// I2C Functions
////////////////

int i2c_init(byte frequency)
{
  if (frequency != I2C_SCL_100KHZ &&
      frequency != I2C_SCL_400KHZ)
    return -1;
  // TODO: Set prescaler to 0.
  TWBR = ((F_CPU / frequency) - 16) / 2;
}

byte i2c_start(byte address, byte config){
  // reset TWI control register
  TWCR = 0;
  // transmit START condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );

  // check if the start condition was successfully transmitted
  if((TWSR & 0xF8) != TW_START){ return 1; }

  // load slave address into data register
  TWDR = ((address << 1) & 0xFE) | (config & 0x01);
  // start transmission of address
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );

  // check if the device has acknowledged the READ / WRITE mode
  byte twst = TW_STATUS & 0xF8;
  if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

  return 0;
}

void i2c_write(byte data){
  // load data into data register
  TWDR = data;
  // start transmission of data
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );

  if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
    return;

  return;
}

byte i2c_read_ack(void){

  // start TWI module and acknowledge data after reception
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );
  // return received data from TWDR
  return TWDR;
}

byte i2c_read_nack(void){

  // start receiving without acknowledging reception
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );
  // return received data from TWDR
  return TWDR;
}

void i2c_stop(void){
  // transmit STOP condition
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
