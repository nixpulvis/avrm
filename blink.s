.global	main

main:
	# set pin 5 of PORTB for output.
	sbi 0x4,5

.loop:
	# set pin 5 high to turn led on.
	sbi 0x5,5

	# set r23-25 to 500,000.
	ldi r23,lo8(500000)
	ldi r24,hi8(500000)
	ldi r25,hlo8(500000)

	# decrement r23-25.
	1:
		subi r23,1
		sbci r24,0
		sbci r25,0

	# branch to label 1 before this point if r23-25 != 0.
	brne 1b

	# set pin 5 low to turn led off.
	cbi 0x5,5

	# set r23-25 to 500,000.
	ldi r23,lo8(500000)
	ldi r24,hi8(500000)
	ldi r25,hlo8(500000)

	# decrement r23-25.
	1:
		subi r23,1
		sbci r24,0
		sbci r25,0

	# branch to label 1 before this point if r23-25 != 0.
	brne 1b

	# jump back to the start of the loop.
	rjmp .loop
