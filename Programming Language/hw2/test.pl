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

% all member of terminals
terminals(term(minus,_), term(num,_), term(times,_)).


% LL(1) parsing table
% ===================
% E.g., predict(non(s,_),term(num,_),0) stands for "on start and num,
% predict production 0. start -> expr".


% YOUR CODE HERE.
% Complete the LL(1) parsing table for the above grammar.
predict(non(s,_),term(num,_),0).
predict(non(e,_),term(num,_),1).
predict(non(t,_), term(num,_), 4).
predict(non(tt,_), term(minus,_), 2).
predict(non(tt,_), term(end,_), 3).
predict(non(ft,_), term(times,_), 5).
predict(non(ft,_), term(minus,_), 4).
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

% Base Case
trans([],[],_).
trans([],Temp, R) :-
    append(Temp, [term(end,_)], R),
    trans([],[],R).

% Recursive Case
trans([H|T], Temp, R) :-
    ( number(H), append(Temp,[term(num,H)],New),trans(T, New, R) );
    ( =(H,'-'), append(Temp,[term(minus,_)],New),trans(T, New, R) );
    ( =(H,'*'), append(Temp,[term(times,_)],New),trans(T, New, R) ).

% Initializer
transform([H|T], R) :-
    trans(T, [term(num,H)], R).

% parseLL
% =======
% YOUR CODE HERE.
% Write parseLL(R,ProdSeq): it takes a transformed list R and produces
% the production sequence the predictive parser applies.
% E.g., transform([3,-,5],R),parseLL(R,ProdSeq).
% ProdSeq = [0, 1, 4, 6, 2, 4, 6, 3].

% compare(A,B) will return true if both terms are num / minus / times.
compare(term(A1,_),term(B1,_)) :-
     =(A1,B1).

% parse(A, B, C, D)  A is a list of all termimals, B is the stack to be worked on
% C is the temporary Product Sequence, D is the Prodseq to store the reuslt finally

% Base Case: reach the end
parse([],[],[],_).
parse([term(end,_)],[term(eps,_)], S, ProdSeq) :-
    ProdSeq = S,
    parse([],[],[],ProdSeq).

% Recursive Case
% Case 1: Head of Stack is a nonterminal
parse([Terminal_Head|Terminal_Tail], [Stack_Head|Stack_Tail], S, ProdSeq) :-
    Terminal_Head \=Stack_Head,
    predict(Stack_Head, Terminal_Head, Number),
    prod(Number,[_|[H|T]]),
    append([H|T], Stack_Tail, Newstack),
    append(S,[Number], Newseq),
    parse([Terminal_Head|Terminal_Tail], Newstack, Newseq, ProdSeq).

% Case 2: Head of Stack is the same as the terminal head
parse([Terminal_Head|Terminal_Tail], [Stack_Head|Stack_Tail], S, ProdSeq) :-
    compare(Terminal_Head,Stack_Head),
    parse(Terminal_Tail, Stack_Tail, S, ProdSeq).

% Case 3: Head of Stack is epsilon
parse([Terminal_Head|Terminal_Tail], [Stack_Head|Stack_Tail], S, ProdSeq) :-
    compare(Stack_Head,term(eps,_)),
    parse([Terminal_Head|Terminal_Tail], Stack_Tail, S, ProdSeq).

% Initializer
parseLL(R, ProdSeq) :-
    parse(R, [non(e,_)], [0], ProdSeq).


% parseAndSolve
% =============
% YOUR CODE HERE.
% Write parseAndSolve, which augments parseLL with computation.
% E.g., transform([3,-,5],R),parseAndSolve(R,ProdSeq,V).
% ProdSeq = [0, 1, 4, 6, 2, 4, 6, 3],
% V = -2.


parseAndSolve(R,ProdSeq,V) :-
    parseLL(R, ProdSeq).











