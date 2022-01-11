HOMEWORK 8: ROPES


NAME:  < Yifan Wang >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< https://www.cs.rpi.edu/academics/courses/spring19/csci1200/lectures/ds_set_soln.h >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 30 >

BALANCED/WORST CASE RUNNIG TIME ANALYSIS:
n = characters in the string of the current Rope (*this)
m = characters in the string of the Rope passed in (for functions that take
    a second rope)
v = longest "value" of a leaf
p = longest path from root to a leaf
Using these variables, state and explain the big-O running time of each 
of the following operations two ways: 1) with a balanced Rope, 2) with an 
extremely unbalanced Rope. Explain what you considered an extremely 
unbalanced rope for the string "Hello my name is Simon".

Copy Constructor:		
1)	O(2^p), We need to go to both left and right for each level's each node. So it should be 2^p.
2)	O(p), The extremely unbalanced tree have all node on one side, no node has other side's child. So just simply go through the only path which will be p.

Construct from Node*:		
1)	O(2^p), Since we need to go through all nodes to calculate the size of the rope, it should be 2^p.
2)	O(p), Since it's an unbalanced rope, we go through the length of path which is p.

Index:
1)	O(p), it goes down to the leaf at the bottom of the rope. So should be the depth of rope which is p.
2)	O(p), it's same with the balanced rope just p might be larger.

Report:
1)	O(p*n), the function call index for every characters in the word so order of index which is p multiply the length of word n.
2)	O(p*n), this should be the same because it need to call index for every character. 

iterator operator++:
1)	O(1), it just move to the parent and find the next right node and then find the first left if there is. So the steps should just be a constant.
2)	O(1), there's no difference between balanced and unbalanced tree in this function.

Split:
1)	O(p+(1+2^p)*p), it first take p steps to find the node to start split(processed in cut function), then it concat that is order of (1+2^p) to another rope depending on how deep and position of the node to start split is which is p, the more deep the node is, the more time we need to call concat. So the order notation should be O(p+(1+2^p)*p).
2)	O(p+(1+2^p)*p), there's no difference between balanced and unbalanced tree in this function.

Concat:
1)	O(1+2^p), The function call copy once which is 2^p and then connect two rope to a new node which is order of 1. So 1+2^p.
2)	O(1+2^p), there's no difference between balanced and unbalanced tree in this function.


TESTING & DEBUGGING STRATEGY: 
Briefly describe the tests in your StudentTests() function.
How did you test the "corner cases" of your implementation?

I focus mostly on testing the split function since that's where the most uncertainty comes from.
I first tried some cases that need to break the string node first and then split. Since I wrote a 
Cut function to do this. It goes well. Then I tested some extreme cases such as cut from the index 1,
This requires concat to process multiple times in the split function.

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

