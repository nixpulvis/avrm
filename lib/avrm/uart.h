#ifndef __AVR_UART_H__
#define __AVR_UART_H__

#define BAUD 9600

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/setbaud.h>

#include "../avrm.h"

/// uart_init
/// Initialize the UART bus. The bus is bound to STDIN and STDOUT, so
/// access to the UART is done through the standard stdio functions.
///
/// # Example
///
/// ```
/// // Setupt the UART.
/// uart_init();
///
/// // Print something over the serial UART.
/// printf("ping\n");
/// ```
void uart_init(void);

#endif
