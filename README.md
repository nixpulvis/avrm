# AVRM (M is for minimal)

[![Build Status](https://travis-ci.org/nixpulvis/avrm.svg?branch=master)](https://travis-ci.org/nixpulvis/avrm)

This is a minimal and efficient set of functions for the AVR microcontrollers. Unlike the popular Arduino libraries which abstract away a lot of lower level details, this library doesn't do too much for you.

Bare metal AVR tinkering using both C and Assembly. This project is a collection of some libraries and code for actual projects. The code is all either C99, or AVR assembly. One of the main goals of this project is to teach myself the needed concepts to write AVR code without the Arduino libraries. If it seems like I'm reinventing the wheel here, that's exactly the point. The other main goal is to make some cool projects, [game_of_life.c](https://github.com/nixpulvis/avr/blob/master/projects/game_of_life.c) for example plays [Conway's Game of Life](http://en.wikipedia.org/wiki/Conway's_Game_of_Life) on an 8x8 LED matrix.

## Getting Started

I'm developing this project on OS X. If you are using another system, this project should still work for you. You will need to install the needed system dependencies yourself.

### Installing Dependencies

Make sure you have these packages installed with Homebrew:

- avr-binutils
- avr-gcc
- avr-libc
- avrdude

### Make

It's important to me to make building things easy. I've made a Makefile which aims to make compiling and flashing AVRs as easy as possible. It tries to help you out too, for example it will find a `/dev/tty.usb*` device automatically.

```sh
# Compile and upload "projects/blink.c".
# By default it sets the BAUD to be compatible with Arduino Uno's
# at 115200.
TARGET=projects/blink make flash

# Compile and upload "projects/game_of_life.c".
# This will upload to a Arduino mini pro.
TARGET=projects/game_of_life BAUD=57600 make flash

# Assemble and upload "projects/hand_coded_blink.asm".
TARGET=projects/hand_coded_blink LANGUAGE=asm make flash

# Make led_matrix.bin with only lib/avr and lib/max7221.
LIBS="lib/avr lib/max7221" make projects/game_of_life.bin
```

TODO: Add info on all Makefile configuration ENV variables.

## Projects

TODO: LED Matrix, Conway's Game of Life.

## Libraries

I'm building some libraries as a part of this repository, as I need them. Anything possibly needed in more than one project is it's own library. Some of the libraries are for functionality of the AVR itself, and some are for external devices.

All Libraries have their own set of header and source files, with a common name in the `/lib` directory.

### avr

A collection of basic functions for the AVR. These functions work in harmony with the functions included in the AVR toolkit.

To view the API for this library read the [header file](https://github.com/nixpulvis/avr/blob/master/lib/avr.h).

## Other Libraries

In addition to the functions included in this library here are a few more libraries I've written for specific chip.

TODO: Links.

### nrf24l01p

A set of functions for using the [Nordic Transceiver nRF24L01+ Module with RP-SMA](https://www.sparkfun.com/products/705).

### max7219

A set of functions for using the [MAX7219 LED Display Driver](https://www.sparkfun.com/products/9622). This chip can control up to 8 digit 7-segment numeric LED displays, LED bar graphs, or 64 individual LEDs.

### ds1307

A set of functions for using the [DS1307 RTC](http://datasheets.maximintegrated.com/en/ds/DS1307.pdf).

## Reference

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)
