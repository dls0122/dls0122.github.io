.data
out_string1: .asciiz "x: "
out_string2: .asciiz "y: "
out_string3: .asciiz "Result: "
	.text
	.globl main
main:
	add $sp, $sp, -4 	
	sw $ra, 4($sp) 		

	j Input
Input:
	#get an integer x
	li $v0, 4
	la $a0, out_string1
	syscall

	li $v0, 5
	syscall
	move $t0,$v0

	#get an integer y
	li $v0, 4
	la $a0, out_string2
	syscall

	li $v0, 5
	syscall
	move $t1,$v0
	
	#check the x or y bigger than 0 or not
	move $a0,$t0 			
	blt $t0, 0, Input		
	blt $t1, 0, Input		

	#Exit part
	li $v0,4
	la $a0,out_string3
	syscall
	jal Ackermann

	move $t0,$v0
	li $v0,1			
	move $a0,$t0		
	syscall

	lw $ra, 4($sp)		
	addu $sp, $sp, 4	
	jr $ra				

Ackermann:
	subu $sp, $sp, 4
	sw $ra, 4($sp) 		

	beq $t0, 0, exit 	
	beq $t1, 0, elseif 	
	
	subu $sp, $sp, 4 	
	sw $t0, 4($sp) 		
	sub $t1, $t1, 1 	
	jal Ackermann 		

	lw $t0, 4($sp) 		
	
	subu $sp, $sp, -4
	sub $t0, $t0, 1 	
	move $t1, $v0
	jal Ackermann

	lw $ra, 4($sp) 		
	subu $sp, $sp, -4 	
	jr $ra

elseif:
	sub $t0, $t0, 1		
	li $t1, 1			
	jal Ackermann		
	lw $ra, 4($sp)		
	addu $sp, $sp, 4	
	jr $ra
exit:
	addi $v0, $t1, 1 	
	lw $ra, 4($sp)		
	addu $sp, $sp, 4	
	jr $ra