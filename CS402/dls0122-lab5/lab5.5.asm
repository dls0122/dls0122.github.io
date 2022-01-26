# this is a program used to test memory alignment for data
	.data 0x10000000
char1:	.byte 'a'			# reserve space for a byte
double1:.double 1.1			# reserve space for a double
char2: 	.byte 'b'			# b is 0x62 in ASCII
half1:	.half 0x8001		# reserve space for a half-word(2 bytes)
char3:	.byte 'c'			# c is 0x63 in ASCII
word1:	.word 0x56789abc	# reserve space for a word
char4:	.byte 'd'			# d is 0x64 in ASCII
word2:	.word 0x0			# reserve space for a word
	.text
	.globl main
	
main:	la $s0, word1
		lb $t0, 3($s0)
		lb $t1, 2($s0)
		lb $t2, 1($s0)
		lb $t3, 0($s0)
		lbu $t4, 3($s0)
		lbu $t5, 2($s0)
		lbu $t6, 1($s0)
		lbu $t7, 0($s0)
		lh $t8, half1
		lhu $t9, half1
		la $s1, word2
		sb $t7, 3($s1)
		sb $t6, 2($s1)
		sb $t5, 1($s1)
		sb $t4, 0($s1)

	jr $ra		# return from main