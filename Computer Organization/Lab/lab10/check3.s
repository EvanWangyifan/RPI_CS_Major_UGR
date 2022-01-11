# lab10.s

	.data
array:	.word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
tab:	.asciiz "\t"
newline:.asciiz "\n"

	.text
	.globl main

main:
	la $s0,array
	# 64 = 16 * 4
	li $s1,64
	add $s1,$s0,$s1
	j loop_header

loop_header:
	blt $s0,$s1,loop_body
	j loop_exit

loop_body:
	lw $a0,0($s0)
	lw $a1, 4($s0)
	lw $a2, 8($s0)
	lw $a3, 12($s0)
	lw $t0, 0($s0)
	lw $t1, 4($s0)
	lw $t2, 8($s0)
	lw $t3, 12($s0)
	
	# 1
	addi $t4,$t0,7
	
	# 2
	addi $t5,$t1,7
	
	# 3
	addi $t6,$t2,7
	
	# 4
	addi $t7,$t3,7
	
	sw $t4, 0($s0)
	sw $t5, 4($s0)
	sw $t6, 8($s0)
	sw $t7, 12($s0)
	
	# 1
	move $a0, $t0
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	move $a0, $t4
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	
	# 2
	move $a0, $t1
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	move $a0, $t5
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	
	# 3
	move $a0, $t2
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	move $a0, $t6
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	
	# 4
	move $a0, $t3
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	move $a0, $t7
	li $v0,1
	syscall
	li $v0,4
	la $a0,newline
	syscall
	j loop_latch

loop_latch:
	add $s0,$s0,16
	j loop_header

loop_exit:
	li $v0, 10
	syscall		# return to caller
