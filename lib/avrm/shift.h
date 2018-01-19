#ifndef __AVR_SHIFT_H__
#define __AVR_SHIFT_H__

#include <stdint.h>
#include <avr/io.h>
#include <avrm.h>
#include <avrm/pin.h>

void shift(int data_pin, int clock_pin, uint8_t n);
void shift_latch(int data_pin, int clock_pin, int latch_pin, uint8_t n);

#endif
