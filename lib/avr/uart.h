#ifndef __AVR_UART_H__
#define __AVR_UART_H__

#define BAUD 9600

#include <avr/io.h>
#include <avr.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/setbaud.h>

// UART Functions
/////////////////

// uart_init
// Initialize the UART bus. The bus is bound to STDIN and STDOUT, so
// access to the UART is done through the standard stdio functions.
//
void uart_init(void);

#endif