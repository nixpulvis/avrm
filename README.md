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

    # Compile and upload "blink.c".
    make

    # Compile and upload "foo.c"
    SOURCE = foo.c
    make

    # Assemble and upload "bar.asm"
    SOURCE = bar.asm
    make


Reference
---------

- [avr-libc manual](http://www.nongnu.org/avr-libc/user-manual/pages.html)