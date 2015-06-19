#include <avr.h>
#include <avr/uart.h>

// Forward declarations.
void assert(const char *name, bool test);


int main(void)
{
  // Initialize the UART for communicating via text.
  uart_init();

  // bool test.
  bool false = FALSE;
  bool true = TRUE;
  assert("false test", false == 0);
  assert("true test", true == 1);

  // byte test.
  byte number = 12;
  assert("byte test", sizeof(number) == 1);

  // IO Tests.
  ////////////
  printf("-----\n");

  // printb test.
  printb(&number, sizeof(number));
  printf("\n");

  // delay_ms test.
  printf("Wait a second... ");
  delay_ms(1000);
  printf("done.\n");

  return 0;
}

//
// assert implementation.
//
void assert(const char *name, bool test)
{
  if (test)
    printf("%s PASS\n", name);
  else
    printf("%s FAIL\n", name);
}
