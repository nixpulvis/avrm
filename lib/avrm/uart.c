#include "uart.h"

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
