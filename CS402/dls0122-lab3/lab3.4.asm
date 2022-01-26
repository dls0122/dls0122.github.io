.data 
my_array: .space 40   #10 words, 40 bytes
initial_value: .word 0 # first digit of your SSN 

.text
.globl main

main:
	lw $t1, initial_value($0)
	la $t0, my_array
	addi $a1, $t0, 40
loop:
	ble $a1, $t0 exit
	sw $t1, 0($t0)
	addi $t1, $t1, 1
	addi $t0, $t0, 4
	j loop
exit:
	jr $ra # return from main