LIBRARY ?= avrm
VERSION ?= 0.0.4
PREFIX ?= /usr/local/Cellar/$(LIBRARY)/$(VERSION)
DEPENDENCIES ?= $(PREFIX)

# The running speed of the AVR, used for `_delay_ms` time calculations.
F_CPU ?= 16000000UL

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
CFLAGS = -Wall -Werror -pedantic -Os -std=c99 \
         -DF_CPU=$(F_CPU) -mmcu=$(MMCU) \
				 -I. $(DEPENDENCIES:%=-I%/include)
LDFLAGS = -L. $(DEPENDENCIES:%=-L%/lib)
ifeq ($(LIBRARY),avrm)
LDLIBS ?= -lavrm
else
LDLIBS ?= -l$(LIBRARY) -lavrm
endif

# The `obj-copy` executable.
OBJ_COPY = avr-objcopy
OBJ_COPY_FLAGS = -O ihex -R .eeprom

# The `ar` archiver executable.
AR = avr-ar

# The `as` assembeler.
AS = avr-as

# The `avrdude` executable.
AVRDUDE = avrdude
AVRDUDE_FLAGS = -F -V -c arduino -p ATMEGA328P

# The `avr-size` executable.
AVRSIZE = avr-size
AVRSIZE_FLAGS = -C

# These rules are not file based.
.PHONY: install test clean serial

SRCS = $(shell find lib -name '*.c')
TESTS = $(shell find test -name '*.c')

# Mark all .o files as intermediate.
.INTERMEDIATE: $(SRCS:.c=.o) $(TESTS:.c=.hex)

# Build the library.
all: lib$(LIBRARY).a($(SRCS:.c=.o))

install: all
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	install lib$(LIBRARY).a $(PREFIX)/lib
	install lib/$(LIBRARY).h $(PREFIX)/include
# TODO: https://github.com/nixpulvis/avrm/issues/2
ifneq ($(wildcard lib/$(LIBRARY)/*.h),)
	mkdir -p $(PREFIX)/include/$(LIBRARY)
	install lib/$(LIBRARY)/*.h $(PREFIX)/include/$(LIBRARY)
endif
ifeq ($(LIBRARY),avrm)
	install Makefile $(PREFIX)
endif

test: install $(TESTS:.c=.test)

clean:
	rm -f $(SRCS:.c=.o) $(TESTS:.c=.o) lib$(LIBRARY).a

# Open up a screen session for communication with the AVR
# through it's on-board UART.
serial:
	screen $(PORT) $(BAUD)

%.test: %.flash
	@echo "TODO: Read serial and check output"

# Given a binary program display information about how much memory it will
# use to hold the program.
%.size: %
	$(AVRSIZE) $(AVRSIZE_FLAGS) $<

# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
%.flash: %.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(PORT) -b $(AVRDUDE_BAUD) -U flash:w:$<

# *.hex <- *
%.hex: %.bin
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@

%.bin: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# .asm <- .c
%.asm: %.c
	$(CC) $(CFLAGS) -S $(C_HEADERS) -DF_CPU=$(F_CPU) -mmcu=$(MMCU) -c $< -o $@

%.o: %.asm
	$(AS) -mmcu=$(MMCU) -o $@ $<
