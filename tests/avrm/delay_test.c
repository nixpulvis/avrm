#include <avrm/delay.h>
#include <avrm/uart.h>

// Forward declarations.
void assert(const char *name, bool test);

int main(void)
{
    // Initialize the UART for communicating via text.
    uart_init();

    printf("before");
    delay_ns(200.0);
    printf("after");

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
