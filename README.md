# AVRM (M is for minimal)

[![Build Status](https://travis-ci.org/nixpulvis/avrm.svg?branch=master)](https://travis-ci.org/nixpulvis/avrm)

This is a minimal and efficient set of functions for the AVR microcontrollers. The code is all either C99, or AVR assembly. The main goal of this project is to write libraries which do not rely on Arduino.

## Getting Started

I'm developing this project on both OS X, and Linux. Windows is simply not on my radar, and I'm uninterested in trying to support it in the build tooling.

### Installing Dependencies

These instructions are for installing the needed dependencies on OS X, Linux documentation is coming soon™. Make sure you have these packages installed with Homebrew:

```sh
brew tap osx-cross/avr
brew install avr-binutils avr-gcc avr-libc avrdude
```

<!-- ### Make

It's important to me to make building things easy. I've made a Makefile which aims to make compiling and flashing AVRs as easy as possible. It tries to help you out too, for example it will find a `/dev/tty.usb*` device automatically.

```sh
make            # Compiles the library.
make install    # Compiles the library, and installs it on the system.
make test       # Compiles the library, installs it, and compiles the tests.
make clean      # Removes compiled artifacts.
make uninstall  # Removes the library from the system.

make size             # Displays size information.
make <program>.flash  # Compiles and flashes the <program> to a connected AVR.
make serial           # Opens a `screen` session to a connected AVR.
```

Below is an example of how to get started.

```sh
# Install the library.
make install
# Flash the uart test program. Remember that the baud rate is different for
# various AVRs.
env AVRDUDE_BAUD=57600 make test/uart_test.flash
# Open a serial connection to the AVR.
make serial
``` -->

## Libraries

I'm building some facility libraries as a part of this repository, as I need them. Anything possibly needed in more than one project is it's own library. Some of the libraries are for functionality of the AVR itself.

All Libraries have their own set of header and source files, with a common name in the `/lib` directory.

- [avrm](https://github.com/nixpulvis/avrm/blob/master/lib/avrm.h)
- [i2c](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/i2c.h)
- [spi](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/spi.h)
- [uart](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/uart.h)

## Other Firmware Libraries

In addition to the functions included in this library here are a few more libraries I've written for specific chip.

- [max7221](https://github.com/nixpulvis/max7221)
- [nrf24l01](https://github.com/nixpulvis/nrf24l01)
- [ds1307](https://github.com/nixpulvis/ds1307)
- [mpu9150](https://github.com/nixpulvis/mpu9150)

## Reference

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)
