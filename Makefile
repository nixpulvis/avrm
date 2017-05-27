LIBRARY = avrm

# The running speed of the AVR, used for `_delay_ms` time calculations.
DF_CPU ?= 16000000UL

# AVR MCU type, see https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html.
MMCU ?= atmega328p

# The system path to communicate via serial, used for both flashing and serial
# monitoring. Defaults to the first port in /dev containing "tty.usb".
PORT ?= /dev/$(shell ls /dev/ | grep "tty\.usb" | sed -n 1p)

# Flashing baud rate.
# 115200 - Arduino Uno
# 57600  - Arduino Mini Pro
AVRDUDE_BAUD ?= 57600

# UART baud default baud rate.
BAUD ?= 9600

CC = avr-gcc
CFLAGS = -Wall -Werror -pedantic -Os -std=c99 -DF_CPU=$(DF_CPU) -mmcu=$(MMCU)

# The `obj-copy` executable.
OBJ_COPY = avr-objcopy
OBJ_COPY_FLAGS = -O ihex -R .eeprom

# The `ar` archiver executable.
AR = avr-ar

# The `avrdude` executable.
AVRDUDE = avrdude
AVRDUDE_FLAGS = -F -V -c arduino -p ATMEGA328P

# The `avr-size` executable.
AVRSIZE = avr-size
AVRSIZE_FLAGS = -C

# These rules are not file based.
.PHONY: install test clean serial

# Mark all .o files as intermediate.
# .INTERMEDIATE: $(SRCS:.c=.o) $(TESTS:.c=)

# Build the library.
SRCS = $(shell find lib -name '*.c')
all: lib$(LIBRARY).a($(SRCS:.c=.o))

install:
	@echo "TODO"

TESTS = $(shell find test -name '*.c')
test:
	@echo "TODO"

clean:
	@echo "TODO"

# Open up a screen session for communication with the AVR
# through it's on-board UART.
serial:
	screen $(PORT) $(BAUD)

# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
%.flash: %.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(PORT) -b $(AVRDUDE_BAUD) -U flash:w:$<

# *.hex <- *
%.hex: %
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@
