.data 
var1: .word 0 # first digit of your SSN 
var2: .word 6 # second digit of your SSN 
var3: .word -2020

.text
.globl main
main:
	lw $t0, var1($0)
	lw $t1, var2($0)
	slt $t2, $t1, $t0
	blez $t2, else
	
	lw $t2, var3($0)
	sw $t2, var1($0)
	sw $t2, var2($0)
	beq $0, $0, final
	
else:
	move $t2, $t0
	move $t0, $t1
	move $t1, $t2
	sw $t0, var1($0)
	sw $t1, var2($0)
	
final:
	li $v0, 1
	lw $a0, var1($0)
	syscall
	li $v0, 1
	lw $a0, var2($0)
	syscall
	jr $ra # return from main