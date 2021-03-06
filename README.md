# AVRM (M is for minimal)
[![Build Status](https://travis-ci.org/nixpulvis/avrm.svg?branch=master)](https://travis-ci.org/nixpulvis/avrm)

This is a minimal and efficient set of functions for the AVR microcontrollers.
The code is all either C99, or AVR assembly. The main goal of this project is
to write libraries which do not rely on Arduino.

## Usage

It's important to me to make building things easy. I've made a Makefile which
aims to make compiling and flashing AVRs as easy as possible. It tries to help
you out too, for example it will find a `/dev/tty.usb*` device automatically.

```sh
# Compiles the library.
make

# Compiles the library, and installs it on the system.
# NOTE: You may need to run this command with `sudo` if you don't have
# write access to /usr/local.
make install

# Flash each test program to an AVR.
# TODO: Validate output #10.
make test

# Displays size information for a program.
make <program>.size

# Compiles and flashes a program to a connected AVR.
make <program>.flash

# Opens a `screen` session to a connected AVR.
make serial
```

## Getting Started

I'm developing this project on both OS X, and Linux. Windows is simply not on
my radar, and I'm uninterested in trying to support it in the build tooling.

**Note:** I've currently stopped development on macOS. If you're interested in
keeping support for that platform I'll happily take PRs.

### Installing Dependencies

These instructions are for installing the needed dependencies on Arch Linux or
macOS.

```sh
sudo pacman -S avr-libc avr-gcc avr-binutils avrdude screen
```

```sh
brew tap osx-cross/avr
brew install avr-binutils avr-gcc avr-libc avrdude
```

### Install AVRM

```sh
# Install the library. You may need to run this with `sudo`.
make install
```

### Example Firmware.

```sh
# Flash the uart test program. Remember that the baud rate is different for
# various AVRs.
env AVRDUDE_BAUD=57600 make examples/hello_world.flash

# Open a serial connection to the AVR.
make serial

# Remember, `CTRL-a k` to exit screen.
```

## AVRM STD Libraries

AVRM itself is made up of various C99 headers, and their implementations. Each
"library" is meant to be somewhat self contained, and provide access to some
underlying functionality of the AVR. External peripheral support is typically
provided by an external library (see below).

All libraries have their own set of header and source files, with a common name
in the `/lib` directory.

- [avrm](https://github.com/nixpulvis/avrm/blob/master/lib/avrm.h)
- [pin](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/pin.h)
- [i2c](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/i2c.h)
- [spi](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/spi.h)
- [uart](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/uart.h)
- [shift](https://github.com/nixpulvis/avrm/blob/master/lib/avrm/shift.h)

See the [ROADMAP.txt](ROADMAP.txt) for some insight into what's left to
implement, and how things are organized. I generally only get around to writing
things as I need thing however, so don't hold your breath. If you would like
to help, please don't hessitate to open an issue, or jump right in with a PR.

## External Libraries

In addition to the built-in libraries included in AVRM, there are a few more
libraries I've written for specific chips.

- [max7221](https://github.com/nixpulvis/max7221)
- [ds1307](https://github.com/nixpulvis/ds1307)
- [nrf24l01](https://github.com/nixpulvis/nrf24l01)
- [mpu9150](https://github.com/nixpulvis/mpu9150)
- [ws2812](https://github.com/nixpulvis/ws2812)

Writing external libraries is easy with AVRM, since the `Makefile` was designed
to be used in an `include` statement in external libraries as well.

```make
# Describe our custom library.
LIBRARY = some_fun_chip
VERSION = 1.33.7

# Target our installed AVRM.
PREFIX = /usr/local/$(LIBRARY)/$(VERSION)
AVRM = /usr/local/avrm/0.0.5
DEPENDENCIES = $(PREFIX) $(AVRM)

# Include the AVRM Makefile, which will use the variables above to work with
# this library.
include $(AVRM)/Makefile
```

## Projects

The main purpose of this library is to facilitate making cool projects of
course! Here's a list of some of mine.

- [clock](https://github.com/nixpulvis/clock)
- [led-cube](https://github.com/nixpulvis/led-cube)
- [quadcopter](https://github.com/nixpulvis/quadcopter)
- [synth](https://github.com/nixpulvis/synth) TODO: Awaiting ws2812 library.

TODO: #4

```make
AVRM = /usr/local/avrm/0.0.5
DS1307 = /usr/local/ds1307/0.0.3
MAX7221 = /usr/local/max7221/0.0.3
DEPENDENCIES = $(DS1307) $(MAX7221) $(AVRM)
LDLIBS = -lds1307 -lmax7221 -lavrm

include $(AVRM)/Makefile
```

## Reference

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)
- [avrdude manual](https://www.nongnu.org/avrdude/user-manual/avrdude.html)

