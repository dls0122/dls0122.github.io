.text
.globl main
main: 
	addi $sp, $sp, -4 
	sw $ra, 4($sp)	
	jal test 			# call ‘test’ with no parameters
	nop 				# execute this after ‘test’ returns
	lw $ra, 4($sp)
	addi $sp, $sp, 4 
	jr $ra 				# return from main
# The procedure ‘test’ does not call any other procedure. Therefore $ra
# does not need to be saved. Since ‘test’ uses no registers there is
# no need to save any registers.
test: 
	nop 				# this is the procedure named ‘test’
	jr $ra 				# return from this procedure