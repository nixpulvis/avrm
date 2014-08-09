.global	main

main:
	; set pin 5 of PORTB for output.
	sbi 0x4,5
	; set pin 2 of PORTB for output.
	sbi 0x4,2
	; set pin 1 of PORTB for input.
	cbi 0x4,1
	; set pin 1 pull up.
	sbi 0x5,1

.loop:
	; if pin 1 is high, the button is not pressed.
	sbis 0x3,1
	; jump to the else case.
	rjmp .else
.if:
	; set pin 5 low to turn led off.
	cbi 0x5,5
	; loop.
	rjmp .loop
.else:
	; set pin 5 high to turn led on.
	sbi 0x5,5
	; loop.
	rjmp .loop
