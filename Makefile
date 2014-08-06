# Microcontroller options.
DF_CPU ?= 16000000UL
MMCU   ?= atmega328p

# Compiler options.
CC = avr-gcc
C_FLAGS = -Os

AS = avr-as

# The `obj-copy` executable.
OBJ_COPY = avr-objcopy
OBJ_COPY_FLAGS = -O ihex -R .eeprom

# The `avrdude` executable.
AVRDUDE = avrdude
AVRDUDE_FLAGS = -F -V -c arduino -p ATMEGA328P
AVRDUDE_PORT ?= /dev/tty.usbmodem1421
AVRDUDE_BAUD ?= 115200

# The program to build, by default "led.c".
SOURCE ?= led.c

# The target name is the source file name without the extension.
ifneq (, $(findstring .c,$(SOURCE)))
	TARGET = $(SOURCE:.c=)
else
	TARGET = $(SOURCE:.s=)
endif

# upload
#
# Given a hex file using `avrdude` this target flashes the AVR with the
# new program contained in the hex file.
#
# This process does not involve setting fuse bits on the AVR, and does
# not affect the bootloader.
#
upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUD) -U flash:w:$<

# Generate a .bin file from a .o file.
%.bin: %.o
	$(CC) -mmcu=$(MMCU) $< -o $@

# Compile the assembly for a .c file.
%.s: %.c
	$(CC) -S $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@

ifneq (, $(findstring .c,$(SOURCE)))
# Generate a .o file from a .c file, when the source is a .c file.
%.o: %.c
	$(CC) $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@
else
# Generate a .o file from a .s file, when the source is a .s file.
%.o: %.s
	$(AS) -mmcu=$(MMCU) -o $@ $<
endif

# Generate a .hex file from a .bin file.
%.hex: %.bin
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@

# Remove non-source files.
clean:
	rm -rf *.o *.bin *.hex

# Mark the hex file as intermediate.
.INTERMEDIATE: $(TARGET).hex