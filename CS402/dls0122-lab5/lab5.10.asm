	.data 0x10000000
	.align 0
ch1:	.byte 'a'
word1:	.word 0x89abcdef
ch2:	.byte 'b'
word2:	.word 0
	.text
	.globl main
main:	
	lui $t0, 0x1000
	ori $s0, $t0, 0x1
	lui $t0, 0x1000
	ori $s1, $t0, 0x6	 	
	lbu $t1, 3($s0)
	lbu $t2, 2($s0)
	sll $t1, $t1, 8		 
		sll $0, $0, 0		 
	or $t0, $t2,$t1		 
	sw $t0, 2($s1)		 
	lwl $t3, 1($s0)
		sll $0, $0, 0		 
	srl $t3, $t3, 16
	swr  $t3, 0($s1)		 

	ori $v0, $0, 10
	syscall