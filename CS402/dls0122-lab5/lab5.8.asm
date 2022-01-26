	.data 0x10000000
word1:	.word 0x89abcdef
	.text
	.globl main
main:	la $s0, word1
		lwr $t4, 0($s0)
		lwr $t5, 1($s0)
		lwr $t6, 2($s0)
		lwr $t7, 3($s0)
		jr $ra