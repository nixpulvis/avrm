#include <avrm/i2c.h>

int main(void) {
    i2c_init(I2C_SCL_100KHZ);

    // TODO: This needs to be thought out a little bit.

    return 0;
}
