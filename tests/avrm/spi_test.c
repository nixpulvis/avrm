#include <avrm/spi.h>
#include <avrm/uart.h>

// Forward declarations.
void assert(const char *name, bool test);

int main(void) {
    uart_init();
    spi_init(SPI_MASTER);

    spi_start();
    byte send = 0x31;
    byte recv = spi_transfer(send);
    spi_end();

    assert("spi_transfer test", send == recv);

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

