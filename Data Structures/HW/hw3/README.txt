HOMEWORK 3: RESIZABLE TABLE


NAME:  < Yifan Wang >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< https://www.cs.rpi.edu/academics/courses/spring19/csci1200/lectures/07_vector_implementation.pdf >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 15 >



ORDER NOTATION:

Write each answer in terms of n = the number of rows and m = the
number of columns.  You should assume that calling new [] or delete []
on an array will take time proportional to the number of elements in
the array.

get				O(1)

set				O(1)

numRows			O(1)

numColumns			O(1)

push_back_row			O(N^2)

push_back_column		O(N^2)

pop_back_row			O(N^2)

pop_back_column		O(N^2)

print				O(N^2)


TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your Table class design & implementation?

For debugging, I used Valgrind as my memory debugger after finish the HW.
At the same time, I use the print function to print the table after every change
I made to the table to check if all the implementation of the member functions work correctly. Moreover, I added test cases in main.cpp to check that int and short string also works in the table. The sample output is attached with the submission.


EXTRA CREDIT: 
Indicate here if you implemented push_back for multiple rows or
multiple columns and/or rewrote your code using pointers instead of
subscripts for extra credit.  Also document the order notation for
your multi-row and multi-column push_back functions.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

