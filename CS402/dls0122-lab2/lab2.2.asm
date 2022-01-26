		.data 0x10010000
var1: 	.word 0x10  		
var2: 	.word 0x20 
var3:	.word 0x30 
var4:	.word 0x40 

first:	.byte 'S'
last:	.byte 'D'


		.text
		.globl main

main:	addu $s0, $ra, $0	# save $31 in $16

		lw $t0, var1	# load var1 to register $t0
		lw $t1, var2	# load var2 to register $t1
		lw $t2, var3	# load var3 to register $t2
		lw $t3, var4	# load var4 to register $t3
		
		lb $t4, first	# load first to register $t4
		lb $t5, last	# load last to register $t5
		
		sw $t3, var1	# store the value in register $t3 to var1
		sw $t2, var2	# store the value in register $t2 to var2
		
		sw $t1, var3	# store the value in register $t1 to var3
		sw $t0, var4	# store the value in register $t0 to var4

# restore now the return address in $ra and return from main
		addu $ra, $0, $s0 	# return address back in $31
		jr $ra 		# return from main