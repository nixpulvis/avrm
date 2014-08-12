AVR
===

Bare metal AVR tinkering using both C and Assembly.


Getting Started on OSX
----------------------

Make sure you have the following packages installed with Homebrew.

- avr-binutils
- avr-gcc
- avr-libc
- avrdude

Once you have these packages, you will have `avr-gcc`, `avr-as`, `avr-objcopy`,
and `avrdude`. These are the requirements of the Makefile in this project.

```sh
# Compile and upload "blink.c".
# By default it sets the BAUD to be compatible with Arduino Uno's
# at 115200.
TARGET=blink make

# Compile and upload "led_matrix.c"
# This will upload to a Arduino mini pro.
TARGET=led_matrix BAUD=57600 make

# Assemble and upload "hand_coded_blink.asm"
TARGET=hand_coded_blink LANGUAGE=asm make
```

The Makefile is pretty smart, and will find the correct /dev/tty.usb* device automatically. You can overwrite it by setting `PORT` to the file path.


Reference
---------

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)