;; Explaination of lis_slow: based on pseudo code provided in the HW pdf

;; Explaination of lis_fast: This program will implement lisfast using dynamic programming.
;; First it augments a given list in the following way: assume we are given '(1 3 2 4), then it is augmented to '((1 1 0) (3 1 0) (2 1 0) (4 1 0))
;; For each augmented element (a,b,c), a refers to the original value in the list, b refers to the maximum lenngth of list starting from this
;; number, c refers to the next number it points to. b and c are initialized in the above way
;; Next it runs algorithms to fill in those initialized value b and c. We will start from the last item and then moving backward to update.
;; For the same example, it should yields '((1 3 3) (3 2 4) (2 2 4) (4 1 0))
;; For conflicts and draws, the rules explained in the HW pdf will be followed
;; Finallly from the result list of lists, we find a item with the maximum b, i.e max length at this number, and then moving forward using the value
;; recorded in c. We will then be able to get the non-descending list of max length.


;; Contract: lis_slow: (l) -> (1 2 3 4)
;; Purpose: to find the longest non-decreasing sublist
;; Example: (list_slow '(1 2 3 4 2 3)) should produce (1 2 3 4)
;; Definition:
(define (lis_slow l)
    (longest 0 (myfilter (sub l)) '()))

;; Contract: check: (n l) -> #t/#f
;; Purpose: to check if the list is non-decreasing
;; Example: (check 0 '(1 2 3 4)) should produce #t
;; Definition:
(define (check n l)
    (if (null? l) #t
    (if (<= n (car l)) (check (car l) (cdr l)) #f  )))

;; Contract: myfilter: (l) -> list
;; Purpose: In list of lists, remain the non-decreasing list and change others to '()
;; Example: (myfilter (list '(1 2) '(3 4) '(4 3))) should produce ( (1 2) (3 4) () )
;; Definition:
(define (myfilter l)
    (if (null? l) '()
    (cons (if (check 0 (car l)) (car l) '()) (myfilter (cdr l)))))

;; Contract: foldr: (op lis id) -> (op lis id)
;; Purpose: Apply op to all element in lis in left-associative. Code from Lec 13.
;; Example: (foldl + ‘(1 2 3) 0) should produce ((0 + 1) + 2) + 3
;; Definition:
(define (foldr op lis id)
    (if (null? lis) id
    (op (car lis) (foldr op (cdr lis) id))))


;; Contract: foldl: (op lis id) -> (op lis id)
;; Purpose: Apply op to all element in lis in right-associative. Code from Lec 13.
;; Example: (foldr + ‘(1 2 3) 0) should produce 6
;; Definition:
(define (foldl op lis id)
  (if (null? lis) id
  (foldl op (cdr lis) (op id (car lis) )))
)

;; Contract: sub: (lis) -> ((list) (list) (list)...)
;; Purpose: Find all sublist of the input list. Code from problem set 2.
;; Example: (sub '(1 2 3)) should produce ( ()  (1) (2) (3) (1 2) (2 3) (1 3) (1 2 3))
;; Definition:
(define (sub lis)
    (foldr (lambda (x y)
    (let ((f (lambda (z) (cons x z))))
    (append (map f y) y)))
    lis '(())))

;; Contract: list-length: (n l) -> int
;; Purpose: to get the number of contents in the list
;; Example: (list-length 0 '(1 2 3)) should produce 3
;; Definition:
(define (list-length n l)
    (if (null? l) n (list-length (+ n 1) (cdr l))))

;; Contract: longest: (n l lng) -> list
;; Purpose: In list of lists, output the longest one
;; Example: (longest 0 '( (1 2 3) (3 4) () ) '()) should produce (1 2 3)
;; Definition:
(define (longest n l lng)
    (if (null? l) lng
     (if (< n (list-length 0 (car l))) (longest (list-length 0 (car l)) (cdr l) (car l) ) (longest n (cdr l) lng ) )))

;;---------------------------------------

;; Contract: mymap: ( (f * l -> a) * (list l) -> (list a))
;; Example: (mymap abs ‘(-1 2 -3 -4)) yields (1 2 3 4)
;; Purpose: Takes 2 arguments: a function f and a list lis and builds a new list by applying the f to each element of lis
;; Definition:
(define (mymap f l) (if (null? l) '( )
      (cons (f (car l)) (mymap f (cdr l)) )))

;; Contract: reverse1: (list l) -> (list l)
;; Example: (reverse1 '(1 2 3 4)) yields '(4 3 2 1)
;; Purpose: Take a list as input and reverse it
;; Definition
(define (reverse1 l)
  (if (null? l)
     '()
     (append (reverse1 (cdr l)) (list (car l)))
  )
)

;; Contract: augment: (id) -> (list l)
;; Example: (augment '(1)) yields '(1 1 0)
;; Purpose: augment a num into a intialized list with (num longest_length previous_number), where the latter two are initialized to 1 and 0 respectively
;; Definition:
(define (augment num)
  (cons num '(1 0))
)


;; Contract: compare: (src target) -> (list l)
;; Example: (compare '(7 1 0) '(1 1 0)) yields '(1 2 7)
;; Purpose: Compare two elements in the augmented list to follow the update rules
;; Definition:
(define (compare src target)
  (cond ((null? src) target)
        ((>=  (car src) (car target) )
        (if (>= (cadr src) (cadr target))
          (list (car target)  (+ 1 (cadr src)) (car src) )
          target
        ))
        (target)
  )
)

;; Contract: find: (unchecked checked) -> (list (list l))
;; Purpose: checked is a list of augmented list, unchecked is a current augmented list to look at, this is to fill in the values
;; in the dynamic programming method
;; Definition:
(define (find unchecked checked)
  (let ((copy checked))
  (foldl (lambda (y x) (compare x y)) copy unchecked)
    )
)

;; Purpose: helper funcion of lis_fast
;; Definiton
(define (helper checked unchecked)
  (if (null? unchecked) checked
   (helper (append (list(find (car unchecked) checked)) checked) (cdr unchecked))
  )
)

;; Contract: find_max: (lis current) -> (list l)
;; Example: (find_max '((7 4 8) (1 6 4) (4 5 5) (8 3 11) (5 4 6) (11 2 11) (6 3 7) (4 4 5) (5 3 7) (7 2 11) (6 2 11) (11 1 0))) = '(1 6 4),
;; since its max length is 6
;; Purpose: find the item with the maximum length
;; Definition
(define (find_max lis current)
  (foldl (lambda (y x) (if (> (cadr x) (cadr y)) x y) )  lis current)
)

;; Contract: findprint (num lis result) -> (list l)
;; Purpose: print the final sequence
;; Definition
(define (print num lis result)
  (if (null? lis) result
        (if (equal? num (caar lis))
            ;;then
            (print (caddar lis) (cdr lis) (cons num result)) 
            ;;else
            (print num (cdr lis) result )
        )
  )
)

;; Details see the beginning at this file
;; Purpose: calculate the maximum length of non-decreasing list of a given random list in polynomial time
;; Definition:
(define (lis_fast lis)
  (if (null? lis) lis
  (let (( newlis (helper (list (car (mymap augment (reverse1 lis))))  (cdr (mymap augment (reverse1 lis)))) ))
    (let (( maxitem (find_max newlis (car newlis)) ))
      (reverse1 (print (car maxitem) newlis '()) )
    )
  )
  )
)

