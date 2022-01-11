% Grammar 
% =======
% Nonterminals expr, term, term_tail and factor_tail are encoded as
% non(e,_), non(t,_), non(tt,_) and non(ft,_), respectively. 
% Special nonterminal start is encoded as non(s,_).
% Terminals num, -, and * are encoded as 
% term(num,_), term(minus,_) and term(times,_). 
% Special terminal term(eps,_) denotes the epsilon symbol.
% 
% Productions are represented as prod(N,[H|T]) where N is the unique
% index of the production, H is the left-hand-side, and T is the 
% right-hand-side. 

prod(0,[non(s,_),non(e,_)]).
prod(1,[non(e,_),non(t,_),non(tt,_)]). 
prod(2,[non(tt,_),term(minus,_),non(t,_),non(tt,_)]).
prod(3,[non(tt,_),term(eps,_)]).
prod(4,[non(t,_),term(num,_),non(ft,_)]).
prod(5,[non(ft,_),term(times,_),term(num,_),non(ft,_)]).
prod(6,[non(ft,_),term(eps,_)]).


% LL(1) parsing table
% ===================
% E.g., predict(non(s,_),term(num,_),0) stands for "on start and num, 
% predict production 0. start -> expr".


% YOUR CODE HERE. 
% Complete the LL(1) parsing table for the above grammar.
predict(non(s,_),term(num,_),0).
predict(non(e,_),term(num,_),1).
predict(non(tt,_), term(minus,_), 2).
predict(non(tt,_), term(end,_), 3).
predict(non(t,_), term(num,_), 4).
predict(non(ft,_), term(minus,_), 4).
predict(non(ft,_), term(times,_), 5).
predict(non(ft,_), term(end,_), 6).

% Sample inputs
% =============
input0([3,-,5]).
input1([3,-,5,*,7,-,18]).


% Transform
% ========
% Transform translates a token stream into the generic representation, 
% including the special end-of-input-marker. E.g., [3,-,5] translates 
% into [term(num,3),term(minus,_),term(num,5),term(end,_)].

% YOUR CODE HERE.
% Write transform(L,R): it takes input list L and transforms it into a
% list where terminals are represented with term(...). The transformed 
% list will be computed in unbound variable R.
% E.g., transform([3,-,5],R).
% R = [term(num,3),term(minus,_),term(num,5),term(end,_)]


transform([H|T],X) :-
	helper([term(num,H)],T,X).

% Base Case
helper(Result,[],X) :-
	append(Result,[term(end,_)],X).

% Recursive Steps
helper(Result,[H|T],X) :-
	((=(H,'-'),append(Result,[term(minus,_)],Temp));
	(=(H,'*'),append(Result,[term(times,_)],Temp));
	(number(H),append(Result,[term(num,H)],Temp))),
	helper(Temp,T,X).


% parseLL
% =======
% YOUR CODE HERE.
% Write parseLL(R,ProdSeq): it takes a transformed list R and produces 
% the production sequence the predictive parser applies.
% E.g., transform([3,-,5],R),parseLL(R,ProdSeq).
% ProdSeq = [0, 1, 4, 6, 2, 4, 6, 3].

% Runner function
parseLL(R,ProdSeq) :-
	helper([non(e,_)],[0],R,ProdSeq).

% Check for the same term
same(term(A,_),term(B,_)) :-
	A is B.

% helper(A,B,C,D), A is the stack, B is temp result, C is all input, D is output list
% Base Case
helper([term(eps,_)], Temp, [term(end,_)], ProdSeq) :-
	ProdSeq = Temp.

% Recursive Steps
% Case: first stack element is the same as Temp head
helper([Stack_H|Stack_T],Temp,[Input_H|Input_T],ProdSeq) :-
	same(Stack_H,Input_H),
	helper(Stack_T,Temp,Input_T,ProdSeq).


% Case: epsolon is the first stack element
helper([Stack_H|Stack_T],Temp,Input,ProdSeq) :-
	same(Stack_H,term(eps,_)),
	helper(Stack_T,Temp,Input,ProdSeq).


% Case: first stack element is nonterminal
helper([Stack_H|Stack_T],Temp,[Input_H|Input_T],ProdSeq) :-
	Stack_H \= Input_H,
	predict(Stack_H,Input_H,N),
	prod(N,[_|Rest]),
	helper([Rest|Stack_T],[Temp|N],[Input_H|Input_T],ProdSeq).




% parseAndSolve
% =============
% YOUR CODE HERE.
% Write parseAndSolve, which augments parseLL with computation. 
% E.g., transform([3,-,5],R),parseAndSolve(R,ProdSeq,V).
% ProdSeq = [0, 1, 4, 6, 2, 4, 6, 3],
% V = -2

parseAndSolve(R,ProdSeq,V) :-
    parseLL(R, ProdSeq).
