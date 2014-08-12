#ifndef __AVR_H__
#define __AVR_H__

#define F_CPU 16000000UL
#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>

// Basic Functions

// UART Functions

void uart_init(void);
int uart_put(char data, FILE *stream);
int uart_get(FILE *stream);

// SPI Functions

void spi_init(void);
void spi_start(void);
void spi_end(void);
unsigned char spi_transfer(unsigned char data);

#endif