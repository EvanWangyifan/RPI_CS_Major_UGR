;; YOUR CODE HERE

;; Contract: expreval: ( (list l) -> Boolean)
;; Example: (expreval '(prog (myand (mynot true) false)) #t) returns #f
;; Purpose: Parse the boolean expression into boolean output recursively
;; Definition:
(define (expreval l binding)
    (if (equal? l 'false);;meet false
        #f
        (if (equal? l 'true);;meet true
            #t
            (if (symbol? l);;meet id
                binding
                (if (equal? (car l) 'prog);;meet prog
                    (expreval (cdr l) binding)
                    (if (equal? (car l) 'mynot);;meet mynot
                        (not (expreval (cdr l) binding))
                        (if (equal? (car l) 'myignore);;meet myignore
                            #f
                            (if (equal? (car l) 'myand);;meet myand
                                (and (expreval (car (cdr l)) binding) (expreval (cdr (cdr l)) binding))
                                (if (equal? (car l) 'myor);;meet myor
                                    (or (expreval (car (cdr l)) binding) (expreval (cdr (cdr l)) binding))
                                    (if (equal? (car l) 'mylet);;meet mylet
                                        (expreval (cdr (cdr (cdr l))) (expreval (car(cdr (cdr l))) binding))
                                        (expreval (car l) binding)))))))))))

;; Contract: myinterpreter ( (list x) -> (list Boolean))
;; Example: (myinterpreter '((prog (myand (mynot true) false))) return (#f)
;; Purpose: Takes a list of boolean expression and returns a list boolean result.
;; Definition:
(define (myinterpreter x)
  (if (null? x) '()
      (cons (expreval (car x) '#f) (myinterpreter (cdr x))));;map expreval to all elements in the list
)