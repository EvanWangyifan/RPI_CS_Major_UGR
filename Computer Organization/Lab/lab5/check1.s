		.data
odd:	.asciiz "ODD\n"
even:	.asciiz "EVEN\n"

		.text
		.globl main

main:	li $v0, 5
		syscall
		jal isodd
		li $v0, 10
		syscall


isodd:	
		move $a0, $v0
		li $t1, 2
		div $a0, $t1
		mfhi $v1
		beq $v1, $zero, L1
		j L2


L1:		li $v0, 4
		la $a0, even
		syscall
		jr $ra

L2:		li $v0, 4
		la $a0, odd
		syscall
		jr $ra












