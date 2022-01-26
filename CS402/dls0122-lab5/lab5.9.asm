	.data 0x10000000
	.align 0
char1:	.byte 'a'
double1:.double 1.1
char2:	.byte 'b'
half1:	.half 0x8001
char3:	.byte 'c'
word1:	.word 0x56789abc
char4:	.byte 'd'

	.text
	.globl main
main:
	lui $t0, 0x1000
	ori $s0, $t0, 0x000d
	lwl $t3, 3($s0)	 
	lwr $t4, 0($s0)
	add $s0, $t3, $t4	# the result is in the register $s0
	jr $ra