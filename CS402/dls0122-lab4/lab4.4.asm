.data	
out_string: .asciiz "\nLargest number:\n"

.text
.globl main
main: 
	li $v0,5
	syscall
	move $t0,$v0
	li $v0,5
	syscall	
	move $t1,$v0	
	
	addi $sp, $sp, -4 
	sw $ra, 4($sp)	
	addi $sp, $sp, -8 
	sw $t0, 4($sp)	
	sw $t1, 8($sp)	
	jal Largest
	addi $sp, $sp, 8 
	lw $ra, 4($sp)	
	addi $sp, $sp, 4 
	jr $ra				 # return from this procedure

Largest: 
	li $v0,4
	la $a0,out_string
	syscall
	lw $a0, 4($sp)	
	lw $a1, 8($sp)	
	slt $t0,$a0,$a1
	blez $t0,test
	move $a0,$a1
	
test: 
	li $v0,1
	syscall				
	jr $ra 				# return from this procedure