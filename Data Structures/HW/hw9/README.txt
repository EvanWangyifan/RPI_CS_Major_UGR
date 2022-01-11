HOMEWORK 9: IMDB SEARCH


NAME:  < Yifan Wang >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< Yihang Wang/ Qiran Sun / https://blog.csdn.net/mys1450663972/article/details/82588806 >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 30 >


HASH FUNCTION DESCRIPTION

My hash function take the partial information's string and use a bitwise hash function written by Justin Sobel which is the same one shown in the lecture. I slightly changed the initialize number. It's from http://www.partow.net/programming/hashfunctions/.


HASH TABLE IMPLEMENTATION

For every movie I find all combinations(63 total) of partial information possible. My hash table takes a string of all partial information to calculate an index within the table size. 
So, I first write a movie class storing all attributes for a movie including the string contains all partial information. Then I wrote a hash table class storing the pointer to the complete movie object as value and the index calculated by the partial string the entry. Including the all_combo function finding possible partial information's string; the  insert function insert a movie object 63 times into a hash table which will use linear probing if meet collision; the find function that rehash the partial string from the query and check if the it has the same partial string and go through the hash table by linear probing and store all of these movies into the vector; last but not the least, I wrote a resize_table function that create a new hash table with the new size and copy all entry in the original table, rehash them and add them to the new hash table.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

