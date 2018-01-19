#include <avrm/pin.h>
#include <avrm/uart.h>

// Forward declarations.
void assert(const char *name, bool test);

int main(void)
{
    // Initialize the UART for communicating via text.
    uart_init();

    // Set a pin to OUTPUT.
    pin_mode(13, OUTPUT);

    // Set a pin to INPUT.
    pin_mode(7, INPUT);

    // Drive logic high to the OUTPUT pin.
    pin_out(13, TRUE);

    // Pullup the INPUT pin.
    pin_out(7, TRUE);

    // Ensure the INPUT pin in held up.
    assert("pin_in test", pin_in(7));

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
