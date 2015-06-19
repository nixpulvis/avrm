#include <stdio.h>
#include <avr/uart.h>

int main(void)
{
  uart_init();
  printf("Hello World!\n");
  return 0;
}