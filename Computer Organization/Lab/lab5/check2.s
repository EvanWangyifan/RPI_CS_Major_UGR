		.data
s1:	.asciiz "byte #1: "
s2:	.asciiz "byte #2: "
s3:	.asciiz "byte #3: "
s4:	.asciiz "byte #4: "
s:  .asciiz "\n"
num:	.word 0x4a889cf1
		.text
		.globl main

main:	jal P1
		jal P2
		li $v0, 10
		syscall

P1:		lw $t1, num
		srl $t0, $t1, 24
		li $v0, 4
		la $a0, s1
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall

		lw $t1, num
		sll $t0, $t1, 8
		srl $t0, $t0, 24
		li $v0, 4
		la $a0, s2
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall

		lw $t1, num
		sll $t0, $t1, 16
		srl $t0, $t0, 24
		li $v0, 4
		la $a0, s3
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall

		lw $t1, num
		sll $t0, $t1, 24
		srl $t0, $t0, 24
		li $v0, 4
		la $a0, s4
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall
		jr $ra


P2:		lw $t1, num
		li $t2, 0xff000000
		and $t0, $t2, $t1
		srl $t0, $t1, 24
		li $v0, 4
		la $a0, s1
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall

		lw $t1, num
		li $t2, 0x00ff0000
		and $t0, $t2, $t1
		srl $t0, $t0, 16
		li $v0, 4
		la $a0, s2
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall

		lw $t1, num
		li $t2, 0x0000ff00
		and $t0, $t2, $t1
		srl $t0, $t0, 8
		li $v0, 4
		la $a0, s3
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall

		lw $t1, num
		li $t2, 0x000000ff
		and $t0, $t2, $t1
		li $v0, 4
		la $a0, s4
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
		li $v0, 4
		la $a0, s
		syscall
		jr $ra
