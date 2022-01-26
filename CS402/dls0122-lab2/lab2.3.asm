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

		lui $t0, 4097		#load var1 to $t1
		lw	$t1, 0($t0)
		
		lui $t0, 4097		#load var2 to $t2
		lw	$t2, 4($t0)
		
		lui $t0, 4097		#load var3 to $t3
		lw	$t3, 8($t0)
		
		lui $t0, 4097		#load var4 to $t4
		lw	$t4, 12($t0)
		
		lui $t0, 4097		#store t4 to var1
		sw	$t4, 0($t0)
		
		lui $t0, 4097		#store t3 to var2
		sw	$t3, 4($t0)
		
		lui $t0, 4097		#store t2 to var3
		sw	$t2, 8($t0)
		
		lui $t0, 4097		#store t1 to var4
		sw	$t1, 12($t0)

# restore now the return address in $ra and return from main
		addu $ra, $0, $s0 	# return address back in $31
		jr $ra 		# return from main