#include <stdio.h>
#include <util/delay.h>
#include "lib/avr.h"

int main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_init();

  for (;;) {
    printf("Hello World!\n");
    _delay_ms(1000);
  }

  return 0;
}
