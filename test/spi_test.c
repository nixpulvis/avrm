#include <avrm/spi.h>

int main(void)
{
  spi_init(SPI_MASTER);

  spi_start();
  spi_transfer(0x03);
  spi_end();

  return 0;
}
