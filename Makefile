# Microcontroller options.
DF_CPU ?= 16000000UL
MMCU   ?= atmega328p

# Port to flash to.
PORT ?= /dev/$(shell ls /dev/ | grep "tty\.usb" | sed -n 1p)

# UART baud rate.
# 115200 - Arduino Uno
# 57600  - Arduino Mini Pro
BAUD ?= 115200

# The location for our library archives and headers.
PREFIX ?= /usr/local/avr

# The name of this library.
LIBRARY = avr

################################

# Probably shouldn't touch these.

# Source files.
SRCS = $(wildcard lib/*.c)

# Test files.
TESTS = $(wildcard test/*.c)

# Libraries needed.
LIBS = -l$(LIBRARY)

# The `gcc` executable.
CC = avr-gcc
C_FLAGS = -Wall -Werror -pedantic -Os -std=c99 -DF_CPU=$(DF_CPU) -mmcu=$(MMCU)

# C flags for compiling with this library.
C_AVR_INCLUDES = -I$(PREFIX)/include
C_AVR_LIBS = -L$(PREFIX)/lib $(LIBS)

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
.INTERMEDIATE: $(SRCS:.c=.o) $(TARGET).hex $(TARGET).bin

################################

# Utility rules (not file based).

# The default task is to build the library.
default: all

# Build the library.
all: lib$(LIBRARY).a

# Show information about target's size.
size: lib$(LIBRARY).a
	$(AVRSIZE) $(AVRSIZE_FLAGS) --mcu=$(MMCU) $<

# Remove non-source files.
clean:
	find . -name '*.a' -or -name '*.o' -or -name '*.bin' -or -name '*.hex' | xargs rm

# Install this library into PREFIX on this system.
install: all
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	install lib$(LIBRARY).a $(PREFIX)/lib
	install lib/$(LIBRARY).h $(PREFIX)/include

# Remove this library from PREFIX on this system.
uninstall:
	rm $(PREFIX)/lib/lib$(LIBRARY).a $(PREFIX)/include/$(LIBRARY).h

# Test this library (must be installed).
test: $(TESTS:.c=.bin)
	find . -name '*.bin' | xargs rm

# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
flash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(PORT) -b $(BAUD) -U flash:w:$<

# Open up a screen session for communication with the AVR
# through it's on-board UART.
serial:
	screen $(PORT) $(BAUD)

################################

# File rules.

# lib$(LIBRARY).a <- SRCS
lib$(LIBRARY).a: $(SRCS:.c=.o)
	$(AR) rcs $@ $?

# .o <- .c
%.o: %.c
	$(CC) $(C_FLAGS) $(C_AVR_INCLUDES) -c $< -o $@

# .bin <- .o
%.bin: %.o
	$(CC) $(C_FLAGS) $(C_AVR_INCLUDES) $? -o $@ $(C_AVR_LIBS)

# .hex <- .bin
%.hex: %.bin
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@
