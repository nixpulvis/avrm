#include <stdio.h>
#include <avr.h>

int main(void)
{
  uart_init();
  int number = 12;
  printb(&number, sizeof(number));
  printf("\n");
  return 0;
}