# AVRM (M is for minimal)

[![Build Status](https://travis-ci.org/nixpulvis/avrm.svg?branch=master)](https://travis-ci.org/nixpulvis/avrm)

This is a minimal and efficient set of functions for the AVR microcontrollers. Unlike the popular Arduino libraries which abstract away a lot of lower level details, this library doesn't do too much for you. The code is all either C99, or AVR assembly. One of the main goals of this project is to teach myself the needed concepts to write AVR code without the Arduino libraries. If it seems like I'm reinventing the wheel here, that's exactly the point. The other main goal is to write libraries which do not rely on Arduino for some parts.

## Getting Started

I'm developing this project on both OS X, and Linux. Windows is simply not on my radar, and I'm uninterested in trying to support it in the build tooling.

### Installing Dependencies

These instructions are for installing the needed dependencies on OS X, Linux documentation is coming soon™. Make sure you have these packages installed with Homebrew:

- avr-binutils
- avr-gcc
- avr-libc
- avrdude

### Make

It's important to me to make building things easy. I've made a Makefile which aims to make compiling and flashing AVRs as easy as possible. It tries to help you out too, for example it will find a `/dev/tty.usb*` device automatically.

TODO: Finish this.

## Libraries

I'm building some libraries as a part of this repository, as I need them. Anything possibly needed in more than one project is it's own library. Some of the libraries are for functionality of the AVR itself.

All Libraries have their own set of header and source files, with a common name in the `/lib` directory.

- [avrm](https://github.com/nixpulvis/avr/blob/master/lib/avrm.h)
- [i2c](https://github.com/nixpulvis/avr/blob/master/lib/avrm/i2c.h)
- [spi](https://github.com/nixpulvis/avr/blob/master/lib/avrm/spi.h)
- [uart](https://github.com/nixpulvis/avr/blob/master/lib/avrm/i2c.h)

## Other Firmware Libraries

In addition to the functions included in this library here are a few more libraries I've written for specific chip.

- [max7221](https://github.com/nixpulvis/max7221)
- [nrf24l01](https://github.com/nixpulvis/nrf24l01)
- [ds1307](https://github.com/nixpulvis/ds1307)
- [mpu9150](https://github.com/nixpulvis/mpu9150)

## Reference

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)
