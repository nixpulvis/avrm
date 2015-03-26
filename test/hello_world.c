#include <stdio.h>
#include <avr.h>

int main(void)
{
  uart_init();
  printf("Hello World!\n");
  return 0;
}