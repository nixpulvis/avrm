# Configuration variables.

# Microcontroller options.
DF_CPU ?= 16000000UL
MMCU   ?= atmega328p

# Port to flash to.
PORT ?= /dev/$(shell ls /dev/ | grep "tty\.usb" | sed -n 1p)

# Flash rate.
# 115200 - Arduino Uno
# 57600  - Arduino Mini Pro
BAUD ?= 115200

# The language we're building from (C or Assembly), default is C.
LANGUAGE ?= c

################################

# Probably should touch these.

# The `gcc` executable.
CC = avr-gcc
C_FLAGS = -Os -std=c99

# The `as` executable.
AS = avr-as

# The `obj-copy` executable.
OBJ_COPY = avr-objcopy
OBJ_COPY_FLAGS = -O ihex -R .eeprom

# The `avrdude` executable.
AVRDUDE = avrdude
AVRDUDE_FLAGS = -F -V -c arduino -p ATMEGA328P

# The source file.
ifneq ($(TARGET),)
SOURCE = $(TARGET).$(LANGUAGE)
endif

################################

# The default task is to flash.
default: flash

################################

# Pseudo rules.

# These rules are not file based.
.PHONY: flash serial clean

# Mark the hex file as intermediate.
.INTERMEDIATE: $(TARGET).hex

################################

# Utility rules (not file based).

# flash
#
# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
#
# This process does not involve setting fuse bits on the AVR, and does
# not affect the bootloader.
flash: check_target $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(PORT) -b $(BAUD) -U flash:w:$(word 2,$^)

# serial
#
# Open up a screen session for communication with the AVR
# through it's on-board UART.
serial:
	screen $(PORT) $(BAUD)

# check_target
#
# Ensure there is a valid target.
check_target:
	@$ [ -f ./$(SOURCE) ] || (echo "Invalid TARGET \"$(TARGET)\"" && false)

# Remove non-source files.
clean:
	rm -rf *.o *.bin *.hex

################################

# File rules.

# .bin <- .o
ifeq ($(LANGUAGE), c)
%.bin: %.o avr.o
	$(CC) -mmcu=$(MMCU) $? -o $@
else
%.bin: %.o
	$(CC) -mmcu=$(MMCU) $? -o $@
endif

# .asm <- .c
%.asm: %.c
	$(CC) -S $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@

# .o <- (.c | .asm)
ifeq ($(LANGUAGE), c)
%.o: %.c
	$(CC) $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@
else
%.o: %.asm
	$(AS) -mmcu=$(MMCU) -o $@ $<
endif

# .hex <- .bin
%.hex: %.bin
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@
