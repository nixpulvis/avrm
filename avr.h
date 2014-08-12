#ifndef __AVR_H__
#define __AVR_H__

#define F_CPU 16000000UL
#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>

// Basic Functions

// UART Functions

void uart_begin(void);
int uart_put(char data, FILE *stream);
int uart_get(FILE *stream);

static FILE uart_io = FDEV_SETUP_STREAM(uart_put, uart_get, _FDEV_SETUP_RW);

// SPI Functions

void spi_begin(void);
unsigned char spi_transfer(unsigned char data);

#endif