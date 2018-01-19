#include <avrm/uart.h>

int main(void) {
    // Start up the UART and attach it to STDIN/STDOUT.
    uart_init();

    // Write the elequent phrase to STDOUT.
    printf("Hello World!\n");

    return 0;
}
