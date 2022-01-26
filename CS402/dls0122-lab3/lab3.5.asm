.data 
msg1: .asciiz "I'm far away"
msg2: .asciiz "I'm nearby"
far: .word 2

.text
.globl main

main:
	li $v0, 5
	syscall
	move $t0, $v0
	li $v0, 5
	syscall
	move $t1, $v0
	bne $t0, $t1, near
	li $v0, 4
	la $a0, msg1
	syscall
	b far
near:
	li $v0, 4
	la $a0, msg2
	syscall
	jr $ra # return from main