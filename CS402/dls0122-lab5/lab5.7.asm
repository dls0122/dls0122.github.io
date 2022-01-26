	.data 0x10000000
word1:	.word 0x89abcdef
	.text
	.globl main
main:	la $s0, word1
		lwl $t0, 0($s0)
		lwl $t1, 1($s0)
		lwl $t2, 2($s0)
		lwl $t3, 3($s0)
		jr $ra