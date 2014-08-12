/* Here in lies a concerted and complete effort to get my Nordic
 * nRF24L01+ working. I've had this thing collecting dust for
 * about a year, and it's time to buckle down and figure it out.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"

typedef unsigned char byte;

void main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_begin();

  char input;
  while(1) {
    puts("Hello world!");
    input = getchar();
    printf("You wrote %c\n", input);
  }
}
