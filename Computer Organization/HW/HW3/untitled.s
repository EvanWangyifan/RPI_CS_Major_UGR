# hw3.s
# NAME: <your-name-here>

        .data
newline:.asciiz "\n"
tab:    .asciiz "\t"
l_brckt:.asciiz "["
r_brckt:.asciiz "]\n"

        # global parameters n, k, and m (e.g., 4x3 multiply by 3x4),
        # which the user inputs in the matrix_size procedure
n:      .word 4
k:      .word 3
m:      .word 4

txt1:   .asciiz "Please enter values for n, k, and m:\n"
txt2:   .asciiz "Please enter values for the first matrix ("
txt3:   .asciiz "Please enter values for the second matrix ("
txt4:   .asciiz "x"
txt5:   .asciiz "):\n"
multiply:
    .asciiz "multiplied by\n"
equal:  .asciiz "equals\n"
test:   .asciiz "test"
################################################################################

        .text
        .globl main
        .globl matrix_multiply
        .globl matrix_print
        .globl matrix_ask_a
        .globl matrix_ask_b
        .globl matrix_sizes

################################################################################

matrix_sizes:
        # Ask the user for the matrix sizes, i.e., n, k, and m,
        # which correspond to multiplying an nxk matrix by a kxm matrix
        li $v0, 4
      la $a0, txt1 # print the inquiry
      syscall
      # get 3 inputs
      li $v0, 5
    syscall
    sw $v0,n

    li $v0, 5
    syscall
    sw $v0,k

    li $v0, 5
    syscall
    sw $v0,m

        jr $ra

################################################################################

matrix_ask_a:
        # Ask the user for the matrix values to store in
        # the memory address indicated by the $a0 register
        
        # Ask for the first matrix
        li $v0, 4
      la $a0, txt2 # print the inquiry
      syscall
      li $v0, 1
      lw $a0, n # print the n size
      syscall
      li $v0, 4
      la $a0, txt4 # print the "x"
      syscall
      li $v0, 1
      lw $a0, k # print the k size
      syscall
      li $v0, 4
      la $a0, txt5 # print the ")"
      syscall

      # get the first matrix
      lw $t0,n
      lw $t1,k
      mul $t0,$t0,$t1 # t0 is the total number of elements in matrix
      add $t1,$s0,$zero # t1 is the offset of the array

forloop1:
    beq $t0,$zero,exit1 # if finish reading, exit
    li $v0, 5
    syscall
    sw $v0,0($t1) # store it into the matrix array
    addi $t1,$t1,4 # add 4 to the array offset
    addi $t0,$t0,-1 # reduce 1 for loop counter
    j forloop1


exit1:
    jr $ra

matrix_ask_b:
    # Ask the user for the matrix values to store in
        # the memory address indicated by the $a0 register

      # Ask for the second matrix
      li $v0, 4
      la $a0, txt3 # print the inquiry
      syscall
      li $v0, 1
      lw $a0, k # print the k size
      syscall
      li $v0, 4
      la $a0, txt4 # print the "x"
      syscall
      li $v0, 1
      lw $a0, m # print the m size
      syscall
      li $v0, 4
      la $a0, txt5 # print the ")"
      syscall

      # get the second matrix
      lw $t0,k
      lw $t1,m
      mul $t0,$t0,$t1 # t0 is the total number of elements in matrix
      add $t1,$s1,$zero # t1 is the offset of the array

forloop2:
    beq $t0,$zero,exit2 # if finish reading, exit
    li $v0, 5
    syscall
    sw $v0,0($t1) # store it into the matrix array
    addi $t1,$t1,4 # add 4 to the array offset
    addi $t0,$t0,-1 # reduce 1 for loop counter
    j forloop2

exit2:
    jr $ra
  

################################################################################

main:
        # Initialize stack as necessary

        # Obtain user input values n, k, and m
        jal matrix_sizes

        # Allocate space for matrices
        lw $t2,n
        lw $t3,k
        li $t4,4
        mul $t0,$t2,$t3
        mul $t0,$t0,$t4 # t0 has the size of the first matrix

        move $a0,$t0
        li $v0,9
        syscall
        move $s0,$v0 # space for the first matrix

  
        lw $t2,k
        lw $t3,m
        li $t4,4
        mul $t1,$t2,$t3
        mul $t1,$t1,$t4 # t0 has the size of the second matrix 

        move $a0, $t1
        li $v0, 9
        syscall
        move $s1, $v0 # the second matrix

        # Get input for matrix A
        jal matrix_ask_a

        # Get input for matrix B
        jal matrix_ask_b

        # Output result
        lw $a1,n
        lw $a2,k      # load size and array
        move $a0,$s0
        jal matrix_print

        li $v0,4
        la $a0,multiply # print "multiply"
        syscall

        lw $a1,k
        lw $a2,m      # load size and array
        move $a0,$s1
        jal matrix_print

        li $v0,4
        la $a0,equal # print "equals to"
        syscall

        # Perform multiplication to get matrix C

        # calculating size needed for the result matrix
        lw $t1,n
        lw $t2,m
        mul $t0,$t1,$t2
        li $t3,4
        mul $t0,$t0,$t3
        # create the result matrix with calculated space
        move $a0,$t0
        li $v0,9
        syscall
        move $s2,$v0





        # passing arguments into the procedure
        move $a0,$s0 # matrix 1
        move $a1,$s1 # matrix 2
        move $a2,$s2 # result matrix


        # do multiplication and print result
        jal matrix_multiply



        move $a0,$s2
        lw $a1,n
        lw $a2,m
        jal matrix_print




        # Cleanup stack and return
        li $v0, 10
        syscall
        #jr $ra


################################################################################

matrix_multiply:
        # multiply matrix A (address $a0) by  matrix B (address $a1),
        # storing the result in matrix C (address $a2)
        move $t2,$a2 # result matrix
        lw $s3,n # row1
        lw $s4,k # col1 & row2
        lw $s5,m # col2
        # t3,t4,t5 are free to use
         # store ra

        li $t6,0 # initialize i
        j loop1

loop1:  
    beq $t6,$s3,EXIT # loop condition


    li $t7,0 # initialize j
    j loop2

back1:  addi $t6,$t6,1 # increment index
    j loop1 # loop back

loop2:  beq $t7,$s5,back1

    li $t8,0 # initialize k
    li $t9,0 # !!!tmp storing the result of an element
    j loop3

back2:  mul $t3,$t6,$s5
    add $t3,$t3,$t7 # index of result[i][j]
    li $t5,4
    mul $t3,$t3,$t5 # offset
    add $t3,$t3,$t2 # actual address

    sw $t9,0($t3) # result[i][j]=tmp
    addi $t7,$t7,1
    j loop2

loop3:  beq $t8,$s4,back2     
        move $t0,$a0 # matrix A
        move $t1,$a1 # matrix B

        mul $t3,$t6,$s4 #
        add $t3,$t3,$t8 # calculating index of matrix1[i][k]
        li $t5,4
        mul $t3,$t3,$t5 # offset of matrix1[i][k]
        add $t0,$t0,$t3 # actual address

        mul $t4,$t8,$s5 #
        add $t4,$t4,$t7 # calculating index of matrix2[k][j]
        mul $t4,$t4,$t5 # offset of matrix2[k][j]
        add $t1,$t1,$t4 # actual address

        lw $s6,0($t0)
        lw $s7,0($t1)

        mul $t3,$s6,$s7 # matrix1[i][k]*matrix2[k][j]
        add $t9,$t9,$t3 # temp += xxx

        addi $t8,$t8,1
        j loop3


EXIT:   




        jr $ra

################################################################################

matrix_print:
        # print matrix (address $a0)
        addi $sp,$sp,-4
        sw $ra,0($sp)
        move $t1,$a1 # row num
        move $t0,$a0 # array address

for_1:      
    beq $t1,$zero,exit_ # if print all rows

    li $v0, 4
      la $a0, l_brckt # print the "["
      syscall
      
      move $t2,$a2 # col num

    jal for_2 # Go print a row
    
    li $v0, 4
      la $a0, r_brckt # print "]"
      syscall

    addi $t1,$t1,-1 # count how many rows left
    j for_1
for_2:
    beq $t2,$zero,back # if print all cols in a row
    lw $t4,0($t0) # load the element

    li $v0, 1
      move $a0, $t4 # print the element in matrix
      syscall

      li $v0, 4
      la $a0, tab # print "\t"
      syscall
    
    addi $t0,$t0,4 # increment offset
    addi $t2,$t2,-1 # count how many cols left
    j for_2 # continue in loop2

back: jr $ra  # back to loop1

exit_:
    lw $ra,0($sp)
    addi $sp,$sp,4
        jr $ra