	.data 0x10000000
user1:	.word 0x0
user2:	.word 0x0
msg:	.asciiz "If bytes were layed in reverse order the number would be:\n"
msg2:	.asciiz	"Integer:\n"
	.text
	.globl main

main:	add $sp, $sp, -4
	sw $ra, 4($sp)
	li $v0, 4
	la $a0, msg2
	syscall

	li $v0, 5
	syscall
	sw $v0, user1
	jal Reverse_bytes

	lw $ra, 4($sp)
	add $sp, $sp, 4
	jr $ra
	
Reverse_bytes:	
	la $s0, user1
	lb $t0, 3($s0)
	lb $t1, 2($s0)
	lb $t2, 1($s0)
	lb $t3, 0($s0)
	lbu $t4, 3($s0)
	lbu $t5, 2($s0)
	lbu $t6, 1($s0)
	lbu $t7, 0($s0)
	la $s1, user2
	sb $t7, 3($s1)
	sb $t6, 2($s1)
	sb $t5, 1($s1)
	sb $t4, 0($s1)
	
	li $v0, 4
	la $a0, msg
	syscall
	li $v0, 1
	lw $a0, user2
	syscall

	jr $ra		# return from method