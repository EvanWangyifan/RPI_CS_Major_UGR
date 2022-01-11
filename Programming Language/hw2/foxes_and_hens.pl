% start with [3,3,1], end with [0,0,0]

% check no hen is in danger
safe([LF,LH,LB]) :-
    (LB is 0; LB is 1),
    (LF =< LH; LH = 0),
    !,
    LF1 is 3-LF, LH1 is 3-LH, 
    (LF1 =< LH1; LH1 = 0).



% all possibl moves(no matter safe or not) from left to right shore
move([A,B,1],[A1,B1,0]) :- 
   A1 is A-2, B1 is B, A1 >= 0, B1 >=0, A1 =< 3, B1 =< 3.
move([A,B,1],[A3,B3,0]) :- 
   A3 is A-1, B3 is B, A3 >= 0, B3 >=0, A3 =< 3, B3 =< 3.
move([A,B,1],[A5,B5,0]) :- 
   A5 is A-1, B5 is B-1, A5 >= 0, B5 >=0, A5 =< 3, B5 =< 3.
move([A,B,1],[A2,B2,0]) :- 
   A2 is A, B2 is B-2, A2 >= 0, B2 >=0, A2 =< 3, B2 =< 3.
move([A,B,1],[A4,B4,0]) :- 
   A4 is A, B4 is B-1, A4 >= 0, B4 >=0, A4 =< 3, B4 =< 3.


move([A,B,0],[A1,B1,1]) :- 
   A1 is A+2, B1 is B, A1 >= 0, B1 >=0, A1 =< 3, B1 =< 3.
move([A,B,0],[A3,B3,1]) :- 
   A3 is A+1, B3 is B, A3 >= 0, B3 >=0, A3 =< 3, B3 =< 3.
move([A,B,0],[A5,B5,1]) :- 
   A5 is A+1, B5 is B+1, A5 >= 0, B5 >=0, A5 =< 3, B5 =< 3.
move([A,B,0],[A2,B2,1]) :- 
   A2 is A, B2 is B+2, A2 >= 0, B2 >=0, A2 =< 3, B2 =< 3.
move([A,B,0],[A4,B4,1]) :- 
   A4 is A, B4 is B+1, A4 >= 0, B4 >=0, A4 =< 3, B4 =< 3.

    


% Current [A,B,C] is current position,Partial_solution is visited steps,Total_solution is the final steps that is valid

search([0,0,0],Partial_solution,Total_solution) :- % base case
    Total_solution = Partial_solution.

search(Current,Partial_solution,Total_solution) :-
    move(Current,NextStep),
    safe(NextStep), % check safe before add to total_solution
    not(member(NextStep,Partial_solution)), % Make sure is not visited
    append(Partial_solution,[NextStep],Temp),
    search(NextStep,Temp,Total_solution). % recursive step


% Runner function
solve(X) :-
    search([3,3,1],[[3,3,1]],X).


