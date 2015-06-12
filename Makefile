# Microcontroller options.
DF_CPU ?= 16000000UL
MMCU   ?= atmega328p

# Port to flash to.
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
LIBRARY ?= avr

################################

# Probably shouldn't touch these.

# Source files.
SRCS = $(wildcard lib/*.c)

# Test files.
TESTS = $(wildcard test/*.c)

# The `gcc` executable.
CC = avr-gcc
CFLAGS = -Wall -Werror -pedantic -Os -std=c99 \
         -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) \
         -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib
ifeq ($(LIBRARY),avr)
LDLIBS = -lavr
else
LDLIBS = -lavr -l$(LIBRARY)
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
.INTERMEDIATE: $(SRCS:.c=.o) $(TARGET).hex

################################

# Utility rules (not file based).

# The default task is to build the library.
default: all

# Build the library.
all: lib$(LIBRARY).a($(SRCS:.c=.o))

# Show information about target's size.
size: lib$(LIBRARY).a($(SRCS:.c=.o))
	$(AVRSIZE) $(AVRSIZE_FLAGS) --mcu=$(MMCU) $<

# Remove non-source files.
clean:
	find . -name '*.a' -or -name '*.o' -or -name '*.hex' | xargs rm

# Install this library into PREFIX on this system.
install: all
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	install lib$(LIBRARY).a $(PREFIX)/lib
	install lib/$(LIBRARY).h $(PREFIX)/include
ifeq ($(LIBRARY),avr)
	install Makefile $(PREFIX)
endif

# Remove this library from PREFIX on this system.
uninstall:
	rm -f $(PREFIX)/lib/lib$(LIBRARY).a \
	      $(PREFIX)/include/$(LIBRARY).h \
	      $(PREFIX)/Makefile

# Test this library (must be installed).
test: $(TESTS:.c=)
	rm $?

# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
flash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(PORT) -b $(AVRDUDE_BAUD) -U flash:w:$<

# Open up a screen session for communication with the AVR
# through it's on-board UART.
serial:
	screen $(PORT) $(BAUD)

################################

# File rules.

# *.hex <- *
%.hex: %
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@
