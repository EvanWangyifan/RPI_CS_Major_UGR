		.data
msg:	.asciiz "x ($t0) equals "
x:		.word	3

		.text
		.globl main

main:	lw $t0, x
		li $t1, 5
		li $t2, 3
		blt $t0, $t1, Add
		j Exit

Add:	add $t0, $t0, $t2

Exit:	
       	la $a0, msg     
       	syscall         

       	li $v0, 1
       	move $a0, $t0
       	syscall

       	jr $ra 