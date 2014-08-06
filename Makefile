# Microcontroller options.
DF_CPU = 16000000UL
MMCU = atmega328p

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
AVRDUDE_PORT = /dev/tty.usbmodem1421
AVRDUDE_BAUD = 115200

# The program to build.
TARGET = led

upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUD) -U flash:w:$<

clean:
	rm -rf *.asm_obj *.c_obj *.hex $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) -mmcu=$(MMCU) $< -o $@

%.s: %.c
	$(CC) -S $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@

%.asm_obj: %.s
	$(AS) -mmcu=$(MMCU) -o $@ $<

%.c_obj: %.c
	$(CC) $(C_FLAGS) -DF_CPU=$(DF_CPU) -mmcu=$(MMCU) -c $< -o $@

%.hex: %
	$(OBJ_COPY) $(OBJ_COPY_FLAGS) $< $@
