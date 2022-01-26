.data
out_string1:		.asciiz "\nPlease enter an integer:\n "
out_string2:		.asciiz "\nThis is a negative integer.\n "
out_string3:		.asciiz "\nThe result is\n: "
theInput:	.word 0
theOutput:	.word 0
			
	.text
	.globl main
		
main:	
	#get an integer
	li $v0, 4			
	la $a0, out_string1	
	syscall
	
	li $v0, 5			
	syscall
	
	sw $v0, theInput
	
	#Call the factorial function
	lw $a0, theInput
			
			#if the input is less than zero
Loop:			blt $a0, 0, reInput
				
	j Loop
reInput:
	#Display the result
		li $v0, 4
		la $a0, out_string2
		syscall
	#get an integer
		li $v0, 4			
		la $a0, out_string1	
		syscall
		
		li $v0, 5			
		syscall
		
		sw $v0, theInput
	
	#Call the factorial function
		lw $a0, theInput
		jal Factorial
		sw $v0, theOutput
		
		#Display the result
		li $v0, 4
		la $a0, out_string3
		syscall
		
		li $v0, 1
		lw $a0, theOutput
		syscall
		
		li $v0, 10
		syscall


			
.globl	Factorial

Factorial:
	subu $sp, $sp, 8	
	sw $ra, ($sp)		
	sw $s0, 4($sp)		
	
	#base case
	li $v0, 1
	beq $a0, 0, terminate	
	
	move $s0, $a0
	sub $a0, $a0, 1
	jal Factorial
	
	#calculate
	mul $v0, $s0, $v0

	terminate:
				lw $ra, ($sp)		
				lw $s0, 4($sp)		
				addu $sp, $sp, 8	
				
				jr $ra