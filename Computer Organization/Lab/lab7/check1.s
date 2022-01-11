
		.data
txt1:   .asciiz "Enter positive integer:\n"
txt2:	.asciiz "For $"
dot:	.asciiz "."
txt3:	.asciiz ", you need:\n"
quarters:
		.asciiz "quarters\n"
dime:	.asciiz "dime\n"
pennies:.asciiz "pennies\n"
		.text
		.globl change
		.globl main

		
main:	
		li $v0, 4
  		la $a0, txt1 # print the inquiry
  		syscall
		# ask for total change
		li $v0, 5
		syscall
		move $a1,$v0 # store in a1
		# print the result
		li $v0, 4
  		la $a0, txt2 # print the inquiry
  		syscall

  		li $t0, 100
  		divu $a1, $t0
  		mflo $t1
  		mfhi $t2

  		li $v0, 1
  		move $a0, $t1
  		syscall
  		li $v0, 4
  		la $a0, dot
  		syscall
  		li $v0, 1
  		move $a0, $t2
  		syscall

  		li $v0, 4
  		la $a0, txt3 # print the inquiry
  		syscall

  		move $a0, $a1
		jal change

		jr $ra


change:
		move $t0, $a0
		li $t1, 25 # a quarter
		li $t2, 10 # a dime

		divu $t0, $t1
		mflo $t3
		mfhi $t0

		li $v0, 1
  		move $a0, $t3 # print the quarter
  		syscall
		li $v0, 4
  		la $a0, quarters
  		syscall

  		divu $t0, $t2
		mflo $t3
		mfhi $t0

		li $v0, 1
  		move $a0, $t3 # print the dime
  		syscall
		li $v0, 4
  		la $a0, dime
  		syscall

  		li $v0, 1
  		move $a0, $t0 # print the penny
  		syscall
		li $v0, 4
  		la $a0, pennies
  		syscall

  		jr $ra


