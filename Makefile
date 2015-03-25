# Microcontroller options.
DF_CPU ?= 16000000UL
MMCU   ?= atmega328p

LIBS = $(wildcard lib/*.c)

PREFIX ?= /usr/local

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
.PHONY: install uninstall test size clean

# Mark all .o files as intermediate.
.INTERMEDIATE: $(LIBS:.c=.o)

################################

# Utility rules (not file based).

# The default task is to build all projects.
default: all

# Build the library.
all: libavr.a

# size
#
# Show information about target's size.
size: libavr.a
	$(AVRSIZE) $(AVRSIZE_FLAGS) --mcu=$(MMCU) $<

# Remove non-source files.
clean:
	rm -rf libavr.a $(wildcard **/*.o)

test:
	$(CC) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) test/hello_world.c -lavr
	rm a.out

install: libavr.a
	mkdir -p $(PREFIX)/avr/lib $(PREFIX)/avr/include
	install $? $(PREFIX)/avr/lib
	install lib/avr.h $(PREFIX)/avr/include

uninstall:
	rm $(PREFIX)/avr/lib/libavr.a $(PREFIX)/avr/include/avr.h

################################

# File rules.

# libavr.a <- LIBS
libavr.a: $(LIBS:.c=.o)
	$(AR) rcs $@ $?

# .o <- .c
%.o: %.c
	$(CC) $(C_HEADERS) $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@
