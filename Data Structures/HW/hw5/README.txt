HOMEWORK 5: UNROLLED LINKED LISTS


NAME:  < Yifan Wang >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< https://www.cs.rpi.edu/academics/courses/spring19/csci1200/lectures/11_list_implementation.pdf, http://www.cplusplus.com/reference/array/array/?kw=array, http://www.cplusplus.com/reference/iterator/iterator/?kw=iterator,  >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 25 >



TESTING & DEBUGGING STRATEGY:
Please be concise!  

For testing I added to the MoreTests in main.cpp, I test the list with char. I tested the push_back, push_front, pop_back, pop_front first and print the list out, assert the size, and assert specific element to see if the list performs as expected. After making sure it does, I tried some basic erase and insert in different position. Last but not the least, I insert to the first position of the node that is already full and see if it successfully creates the new node. Then I tried erasing function where there is only one element in the node to see if it successfully delete the empty node also erase both from the first element of the array and the last element of the array.

For debugging, I mainly rely on the assertion given by the main.cpp and compare my output with the expected output file given. When all of them works, I tried the code on Dr Memory and deal with the memory bugs.


ORDER NOTATION & EVALUATION:
What is the order notation of each of the member functions in your
class? Discuss any differences between the UnrolledLL, dslist, and STL
list classes. Evaluate the memory usage of your initial implementation
in the worst case. Give a specific example sequence of operations
where the data structure is inefficient. What is the average number of
elements per node in this case? 

ORDER NOTATION
**************************************************************************************
Node class:									     *
	Node(const T new_elements[NUM_ELEMENTS_PER_NODE], int num):		 O(N)*
	void print(std::ostream& a):						 O(N)*
										     *
Iterator class:									     *
	No function involving order notation.				       	     *
										     *
UnrolledLL class:								     *
	unsigned int size() const:						 O(N)*
	bool empty() const { return head_ == NULL; }				 O(1)*
	void clear() { destroy_list(); }					 O(N)*
	T& front()								 O(1)*
	T& back()								 O(1)*
	iterator begin()							 O(1)*
	iterator end()								 O(1)*
	void UnrolledLL<T>::print(std::ostream& a)				 O(N)*
	void UnrolledLL<T>::push_front(const T& v)				 O(N)*
	void UnrolledLL<T>::pop_front()						 O(N)*
	void UnrolledLL<T>::push_back(const T& v)				 O(1)*
	void UnrolledLL<T>::pop_back()						 O(1)*
	bool operator== (UnrolledLL<T>& left, UnrolledLL<T>& right)		 O(N)*
	UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator itr)		 O(1)*
	UnrolledLL<T>::iterator UnrolledLL<T>::insert(iterator itr, const T& v)	 O(N)*
	void UnrolledLL<T>::copy_list(const UnrolledLL<T>& old)			 O(N)*
	void UnrolledLL<T>::destroy_list()					 O(N)*
**************************************************************************************
Evaluation:									     *
The difference between UnrolledLL, dslist, and STL list are STL can only go one.     *
direction. UnrolledLL and dslist can go both forward and backward. Also, UnrolledLL  *
contains more than one element (an array) in a single node but other two just contain*
one element. However, the worst case for UnrolledLL is that every node just contains *
1 element in its array that the rest space in the array are all empty. This will be  *
the situation that a lot of memory being wasted and very inefficient.		     *
										     *
**************************************************************************************

EXTRA CREDIT:
Improved memory usage implementation.  Discussion as outlined in .pdf.



MISC. COMMENTS TO GRADER:  
Optional, please be concise!


