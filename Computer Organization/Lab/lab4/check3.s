      .data
newline:      .asciiz "\n"
s2:      .asciiz "GCD("
s3:      .asciiz ","
s4:      .asciiz ") is "

		.text
		.globl main
main:
	li $t0, 45    #the first answer
    li $t1, 54     #the second answer

    li $v0, 4
    la $a0, s2
    syscall

    li $v0, 1
    move $a0, $t1
    syscall

    li $v0, 4
    la $a0, s3
    syscall

    li $v0, 1
    move $a0, $t0
    syscall

    li $v0, 4
    la $a0 s4
    syscall




While: 
    beq $t1, $zero, exit
    bgt $t0, $t1, case1
    ble $t0, $t1, case2
 
case1:   
    sub $t0, $t0, $t1
    j While
case2:  
    sub $t1, $t1, $t0
    j While

exit:


  #pirnt=================================================
  li $v0 , 1
  move $a0, $t0
  syscall

  li $v0 , 4
  la $a0 , newline
  syscall
  jr $ra
  #finish print==========================================
