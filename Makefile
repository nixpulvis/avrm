# AVR
#
# This Makefile is designed to be used by both this library and any other
# library or project targeting AVRs. There are a few configuration variables
# which allow users to specify functionality of this Makefile.
#
# To use this makefile in other libraries or projects create a Makefile with
# the contents:
#
# LIBRARY = <name>
# include $(PREFIX)/Makefile

################################

# Configuration variables.

# The running speed of the AVR, used for `_delay_ms` time calculations.
DF_CPU ?= 16000000UL

# AVR MCU type, see https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html.
MMCU   ?= atmega328p

# The system path to communicate via serial, used for both flashing and serial
# monitoring. Defaults to the first port in /dev containing "tty.usb".
PORT ?= /dev/$(shell ls /dev/ | grep "tty\.usb" | sed -n 1p)

# Flashing baud rate.
# 115200 - Arduino Uno
# 57600  - Arduino Mini Pro
AVRDUDE_BAUD ?= 115200

# UART baud default baud rate.
BAUD ?= 9600

# The location for our library archives and headers.
PREFIX ?= /usr/local/avr

# The name of this library.
LIBRARY ?= avrm

################################

# Probably shouldn't touch these.

# Source files.
SRCS = $(shell find lib -name '*.c')

# Test files.
TESTS = $(shell find test -name '*.c')

# The `gcc` executable.
CC = avr-gcc
CFLAGS = -Wall -Werror -pedantic -Os -std=c99 \
         -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) \
         -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib
ifeq ($(LIBRARY),avrm)
LDLIBS = -lavrm
else
LDLIBS = -l$(LIBRARY) -lavrm
endif

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

################################

# Pseudo rules.

# These rules are not file based.
.PHONY: install uninstall test size clean flash serial

# Mark all .o files as intermediate.
.INTERMEDIATE: $(SRCS:.c=.o) $(TARGET).hex $(TARGET)

################################

# Utility rules (not file based).

# The default task is to build the library.
default: all

# Build the library.
all: lib$(LIBRARY).a($(SRCS:.c=.o))
	@echo $(SRCS)

# Show information about target's size.
size: lib$(LIBRARY).a($(SRCS:.c=.o))
	$(AVRSIZE) $(AVRSIZE_FLAGS) --mcu=$(MMCU) lib$(LIBRARY).a

# Remove non-source files.
clean:
	find . -name '*.a' -or -name '*.o' -or -name '*.hex' | xargs rm

# Install this library into PREFIX on this system.
install: all
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	install lib$(LIBRARY).a $(PREFIX)/lib
	install lib/$(LIBRARY).h $(PREFIX)/include
ifneq ($(wildcard lib/$(LIBRARY)/*.h),)
	mkdir -p $(PREFIX)/include/$(LIBRARY)
	install lib/$(LIBRARY)/*.h $(PREFIX)/include/$(LIBRARY)
endif
ifeq ($(LIBRARY),avrm)
	install Makefile $(PREFIX)
endif

# Remove this library from PREFIX on this system.
uninstall:
	rm -f $(PREFIX)/lib/lib$(LIBRARY).a
	rm -f $(PREFIX)/include/$(LIBRARY).h
	rm -rf $(PREFIX)/include/$(LIBRARY)/
ifeq ($(LIBRARY),avrm)
	rm -f $(PREFIX)/Makefile
endif

# Test this library (must be installed).
test: install $(TESTS:.c=)
	rm $(TESTS:.c=)

# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
flash: install $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(PORT) -b $(AVRDUDE_BAUD) -U flash:w:$(TARGET).hex

# Open up a screen session for communication with the AVR
# through it's on-board UART.
serial:
	screen $(PORT) $(BAUD)

################################

# File rules.

# *.hex <- *
%.hex: %
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@
