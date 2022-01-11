		.data
list: 	.word 11, 100 ,101, 5, 123456789, 18, 3
		.text


main:	
		li $s0, 2 #save word of divisor 2
		la $a0 , list #loading the list
		li $t1 , 0 # count of odd
		li $a1 , 7 #number of emlements

		bne $a1, $zero, numodds


numodds:
		beq $a1, $zero, exit
		lw $t0, 0($a0)
		div $t0, $s0 #reminder
		mfhi $t2
		addi $a0, $a0, 4
		addi $a1, $a1, -1
		beq $t2, 1, isodd
		j numodds


isodd:
		add $t1, $t1, 1
		j numodds
		j exit

exit:



		li $v0, 1
		move $a0, $t1
        syscall
        jr $ra	
