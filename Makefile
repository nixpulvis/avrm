# Microcontroller options.
DF_CPU ?= 16000000UL
MMCU   ?= atmega328p

LIBS = $(wildcard lib/*.c)

################################

# The `gcc` executable.
CC = avr-gcc
C_FLAGS = -Wall -Werror -pedantic -Os -std=c99
C_HEADERS = -I.

# The `as` executable.
AS = avr-as

# The `ar` archiver executable.
AR = avr-ar

# The `avr-size` executable.
AVRSIZE = avr-size
AVRSIZE_FLAGS = -C

################################

# Pseudo rules.

# These rules are not file based.
.PHONY: install size clean

# Mark all .o files as intermediate.
.INTERMEDIATE: $(LIBS:.c=.o)

################################

# Utility rules (not file based).

# The default task is to build all projects.
default: all

# Build the library.
all: avr.a

# size
#
# Show information about target's size.
size: avr.a
	$(AVRSIZE) $(AVRSIZE_FLAGS) --mcu=$(MMCU) $<

# Remove non-source files.
clean:
	rm -rf avr.a $(wildcard **/*.o)

################################

# File rules.

# avr.a <- LIBS
avr.a: $(LIBS:.c=.o)
	$(AR) rcs $@ $?

# .o <- .c
%.o: %.c
	$(CC) $(C_HEADERS) $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@
