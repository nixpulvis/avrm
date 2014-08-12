#include "avr.h"

// UART Functions

void uart_begin(void) {
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

int uart_put(char data, FILE *stream) {
  // force adding a carriage return after newline.
  if (data == '\n')
    uart_put('\r', stream);
  // Wait until data register is empty.
  loop_until_bit_is_set(UCSR0A, UDRE0);
  // Send data.
  UDR0 = data;
}

int uart_get(FILE *stream) {
  // Wait for data.
  loop_until_bit_is_set(UCSR0A, RXC0);
  // Return the data.
  return UDR0;
}

// SPI Functions

void spi_begin(void)
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

unsigned char spi_transfer(unsigned char data)
{
  // Load data into the buffer.
  SPDR = data;

  // Wait until transmission complete.
  while (!(SPSR & _BV(SPIF)));

  // Return received data.
  return SPDR;
}
