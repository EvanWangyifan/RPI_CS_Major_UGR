HOMEWORK 6: CROSSWORD BLACKOUT


NAME:  < Yifan Wang >



COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< https://www.cs.rpi.edu/academics/courses/spring19/csci1200/lectures/12_advanced_recursion.pdf,
 http://www.cplusplus.com/reference/vector/vector/ >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 40 hours >



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(running time order notation & concise paragraph, < 200 words)
The dimensions of the board (w and h) 
The number of words in the dictionary (d)? 
The total number of spaces with a letter (l)?
The total number of blacked out spaces (b)? 
The number of constraints (c)? 
Etc. 

For the vector_erase_index function, the order notation is O(c);

For the check_add function, the order notation is O(w*h*x) where x is the size of start_end_point;

For the find_possible_start_point function, the order notation is O(w*h*d);

For the search function, the order notation is O((x*c*w*d)(x*c!)) where x is the size of start_end_point
Because one time of the function depend on the size of board, the size of constraints, the size of start_end_point 
And the function runs x*c! times since the recursion is in the for loop of c and x;
 

DESCRIPTION OF YOUR ADDTIONAL PUZZLES/DICTIONARIES:
Briefly explain the design of the puzzles or dictionaries and what
makes them "interesting" test cases (or why they aren't interesting).

My first test case test if the board has both width and height of board is less than 3
Which is illegal because the word should have size not less than 3;

The second test case test if I can find the word that occupy the whole width of the board;



SUMMARY OF PERFORMANCE OF YOUR PROGRAM ON THE PROVIDED PUZZLES AND 
ON YOUR ADDITIONAL PUZZLES/DICTIONARIES:
# of solutions & approximate wall clock running time for different
puzzles / dictionaries for a single solution or all solutions. Be
specific about which puzzle/dictionary/parameters you were timing.

 dict_file              board_file              running time                 solutions
 --------------         ----------              ------------                 ----------
 blackout_dict1         blackout1                0m0.002s                       1(wrong)
 blackout_dict1         blackout2                0m0.002s                       1(wrong)
 blackout_dict1         blackout3                0m0.002s                 	1(wrong)
 blackout_dict2         blackout1                0m0.002s                 	1(wrong)
 blackout_dict2         blackout2                0m0.002s                 	1(wrong)
 blackout_dict2         blackout3                0m0.002s                 	1(wrong)
 nyt_mini1_dict         nyt_mini1_noblack        Can't run                      Unknown
 nyt_mini1_dict         nyt_mini2                0m0.002s                     	0
 nyt_mini1_dict         nyt_mini1                Can't run                      Unknown
 reverse_ell_dict       reverse_ell1             0m0.002s                       1
 reverse_ell_dict       reverse_ell2             0m0.002s                       1
 sporcle1_dict1         sporcle1_constraintsA    0m0.002s              		1
 sporcle1_dict2         sporcle1_constraintsA	 0m0.002s			0
 sporcle1_dict3         sporcle1_constraintsA    0m0.002s     			0
 sporcle1_dict4         sporcle1_constraintsA    0m0.002s     			0
 sporcle1_dict1         sporcle1_constraintsB    0m0.002s                   	1(wrong)
 sporcle1_dict2         sporcle1_constraintsB    0m0.002s                   	1(wrong)
 sporcle1_dict3         sporcle1_constraintsB    0m0.002s                   	1(wrong)
 sporcle1_dict4         sporcle1_constraintsB    0m0.002s                   	1(wrong)

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

I think most of the algorithm is alright but I have a memory bug (out of bound of grid row) that I spent 2 days but still cannot fix. So I can just return the recursive function after the first try so I can only get point for some of the one solution cases and some of them can still be wrong. This is the best I can do and it takes me whole week doing it. Thanks for the effort of grading my hw6, have a good day.
