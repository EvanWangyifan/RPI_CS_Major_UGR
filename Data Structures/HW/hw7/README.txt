HOMEWORK 7: SPATIALLY-EMBEDDED ADJACENCY MAPS


NAME:  < Yifan Wang >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< https://www.cs.rpi.edu/academics/courses/spring19/csci1200/lectures/15_maps_I.pdf, https://www.cs.rpi.edu/academics/courses/spring19/csci1200/lectures/16_maps_II.pdf >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 10 >



ORDER NOTATION ANALYSIS:
Consider the following variables and give the complexity along with a short
justification for each of the functions below:

n: The total number of users
l: The total number of users with a location
d: The total number of users with degree >= 1
m: The total number of connections
p: The number of users that a function will print out

loadConnections():			O(m), this function read through the file and for each connection it check if the existing map has the key. If not create a new pair, if yes add to the value.

loadLocations():			O(l), this function go through the file and for each user that has location it creates a new pair in the map to store the location vector as value and user id as key.

printAllUsersWithinDistance():	O(n-1+p), we need to go through all the user except our own to see if the user's distance is smaller than the maximum which is n-1. Then go through the map to print all result which is p.

printDegreesHistogram():		O(n+p), this function go through the connection map and for users that has at least 1 degree it store it to a map so it has n. Then it go through the result map and print every single of them out.

printDegreesOfAll():		O(n), this function go through the connection map and print it out if the user has at least 1 degree during the loop which is n. So it does not have to add p.

printFriendsWithDegree():	O(p), this function store all user id that satisfy the degree requirement in a vector and sort it then print it out, so order of the printed user.

printFriendsWithinDistance():	O(p), this function store the distance and id that satisfy the distance requirement in a map and print it out. So, the order of printed user.

printUsersWithinIDRange():	O(n-1+p), this function check through the whole list of user except himself to see the id difference, so n-1. Then it go through the result vector to print every user id out, which is order of p.


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)






