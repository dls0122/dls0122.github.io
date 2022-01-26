.data 0x10010000
	var1: .word 29	# var1 is a word (32 bit) with the initial value 29
	var2: .word 54	# var2 is a word (32 bit) with the initial value 54
.extern ext1 4		# ext1 is a word (32 bit) in the extern space
.extern	ext2 6		# ext2 is a word (32 bit) in the extern space
	
.text
.globl main	

main:	addu $s0, $ra, $0	# save $31 in $16
		li $t0, 6
		move $t1, $t0
		
		la $t2, var1	# get var1 address
		sw $t2, var1   # store original var1 in $t2 address
		la $t3, ext1
		addu $t3,$t3,$t2
		sw  $t3,var1
		sw	$t3, ext1
		sw	$t3, 0($gp)
		
		la $t4, var2	# get var2 address
		sw $t4, var2   # store original var2 in $t4 address
		la $t5, ext2
		addu $t5, $t5,$t4
		sw  $t5,var2
		sw	$t5, ext2
		sw	$t5, 0($gp)
# restore now the return address in $ra and return from main
		addu $ra, $0, $s0 	# return address back in $31
		jr $ra 		# return from main