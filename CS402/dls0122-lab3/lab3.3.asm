.data 
var1: .word 0 # first digit of your SSN 

.text
.globl main
main:
	lw $t1, var1($0)
	move $t0, $t1
	li $a1,100
loop:
	ble $a1, $t0 exit
	addi $t1, $t1, 1
	addi $t0, $t0, 1
	j loop
exit:
	sw $t1, var1($0)
	li $v0,1
	lw $a0, var1($0)
	syscall
	jr $ra # return from main