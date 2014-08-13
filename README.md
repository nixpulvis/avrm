# AVR from Scratch

Bare metal AVR tinkering using both C and Assembly. This project is a collection of some libraries and code for actual projects. The code is written in either C99, or AVR assembly. One of the main goals of this project is to teach myself the needed concepts to write AVR code without the Arduino libraries. If it seems like I'm reinventing the wheel here, that's exactly the point. The other main goal is to make some cool projects, that actually do things, the `led_matrix.c` for example plays [Conway's Game of Life](http://en.wikipedia.org/wiki/Conway's_Game_of_Life) on an 8x8 LED matrix.

## Getting Started

I'm doing all of this excursively on OSX, so I can only speak from experience to what is needed for this operating system.

### Installing Dependencies

Make sure you have the following packages installed with Homebrew.

- avr-binutils
- avr-gcc
- avr-libc
- avrdude

### Make

One thing that is very important to me is ease of building things. I've made a Makefile which aims to make compiling and flashing AVRs as easy as possible. It tries to help you out as much as possible, for example it will find the correct /dev/tty.usb* device automatically. You can overwrite it by setting `PORT` to the file path.

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

## Libraries

I'm building some libraries as a part of this endeavor, as I need them. Anything possibly needed in more than one project will be broken out into a library.

All Libraries have their own set of header and source files, with a common name. Currently all projects get built with all libraries, but this is something that I aim to improve as I continue refining the Makefile.

These are the current libraries in this repository.

- [avr](#avr)
- [nrf24l01p](#nrf24l01p)

### avr

A collection of basic functions for the AVR. These functions are meant to work in harmony with the functions included in the AVR toolkit.

To view the API for this library read the [header file](https://github.com/nixpulvis/avr/blob/master/avr.h).

### nrf24l01p (not functional yet.)

A set of functions for using the [Nordic Transceiver nRF24L01+ Module with RP-SMA](https://www.sparkfun.com/products/705).

To view the API for this library read the [header file](https://github.com/nixpulvis/avr/blob/master/nrf24l01p.h).

## Reference

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)