module Interpreter where
import Control.Exception
import Debug.Trace
import Data.List


--Problem 1--
--
--interpret(x) = x
--interpret(\x.E1) = interpret(\x.interpret(E1))
--interpret(E1 E2) = let f = interpret(E1)
--						 a = interpret(E2)
--					in case f of
--						\x.E3 -> interpret(E3[a/x])
--							_ -> f a
--


---- Data types ----

type Name = String

data Expr = 
  	Var Name
  | Lambda Name Expr
  | App Expr Expr
  deriving 
    (Eq,Show)


---- Functions ----

--Contract: freeVars::Expr -> [Name]
--Purpose: t takes an expression expr and returns the list of variables that are free in expr without repetition
--Example: freeVars (App (Var "x") (Var "x")) yields ["x"]
--Definition:
freeVars::Expr -> [Name]
---- YOUR CODE HERE 
freeVars (Var n) = [n]
freeVars (Lambda n e) = freeVars e \\ [n]
freeVars (App e1 e2) = nub (freeVars e1 ++ freeVars e2)


--Contract: freshVars::[Expr]->[Name]
--Purpose: generate fresh variables for a list of expressions by making use of the infinite list of positive integers [1..]
--Example: freshVars [Lambda "1_" (App (Var "x") (App (Var "1_") (Var "2_")))] yields the infinite list [1_,3_,4_,5_,..]
--Definition:
freshVars::[Expr]->[Name]
---- YOUR CODE HERE
freshVars expr_li = (map (++ "_") (map show [1..])) \\ (foldl (++) [] (map freeVars expr_li))


--Contract: subst::(Name,Expr) -> Expr -> Expr
--Purpose: takes a variable x and an expression e, and returns a function that takes an expression E and returns E[e/x]
--Example: subst [Lambda y, Lambda z.x y (x z)] yields 
--		   	Lambda "1_" (Lambda "3_" (App (App (Lambda "x" (Var "x")) (Var "1_")) (App (Lambda "x" (Var "x")) (Var "3_"))))
--Definition:
subst::(Name,Expr) -> Expr -> Expr
---- YOUR CODE HERE
subst (x,m) e = case e of

	(Var n) -> 	if x==n then m
				else (Var n)

	(Lambda n e1) -> if x==n then (Lambda n e1)
					else let tmp = (head ((freshVars [m,e1,(Var x)]))) 
							in (Lambda tmp (subst (x,m) (subst (n, (Var tmp)) e1)))

	(App e1 e2) ->	App (subst (x,m) e1) (subst (x,m) e2)



--Contract: appNF_OneStep::Expr -> Maybe Expr
--Purpose: takes an expression e. If there is a redex available in e, it picks the correct applicative order redex and reduces e. 
--			(Note that in applicative order we are pursuing the leftmost, innermost strategy as follows. Pick the leftmost redex R; 
--			if there are nested (inner) redexes within R, pick the leftmost one, and so on, until we reach a redex without nested ones.)
--Example: appNF_OneStep (lambda x. lambda y. x) (lambda z. (lambda x. lambda y. x) z ((lambda x. z x) (lambda x. z x))) yields 
--			Just (App (Lambda "x" (Lambda "y" (Var "x"))) (Lambda "z" (App (Lambda "1_" (Var "z")) (App (Lambda "x" (App (Var "z") (Var "x"))) (Lambda "x" (App (Var "z") (Var "x")))))))
--Definition:
appNF_OneStep::Expr -> Maybe Expr
---- YOUR CODE HERE
appNF_OneStep e = case e of

	(Var n) -> Nothing

	(Lambda n e1) -> let tmp=appNF_OneStep e1
						in case tmp of
							Just e2 -> Just (Lambda n e2)
							Nothing -> Nothing

	(App e1 e2) -> let tmp1=appNF_OneStep e1
					in case tmp1 of
						Just e5 -> Just (App e5 e2)
						Nothing -> let tmp2=appNF_OneStep e2
									in case tmp2 of
										Just e4 -> Just (App e1 e4)
										Nothing -> case e1 of
											(Lambda n1 e3) -> Just (subst (n1,e2) e3)
											_ -> Nothing
												
							
--Contract: fappNF_n::Int -> Expr -> Expr
--Purpose: Given an integer n and an expression e, appNF_n does n reductions (or as many as possible) and returns the resulting expression.
--Example: appNF_n 0 (Var "a") yields (Var "a")
--Definition:
appNF_n::Int -> Expr -> Expr
---- YOUR CODE HERE
appNF_n n e = case n of
	0 -> e
	_ -> let tmp = (appNF_OneStep e)
			in case tmp of
				Just e1 -> (appNF_n (n-1) e1)
				Nothing -> e



