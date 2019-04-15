(include "queue.scm")
(include "gates.scm")
(include "nand.scm")

(define the-agenda (make-agenda))

(define (or-gate a1 a2 output)
    
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define env this)
    (define (iter expr)
        (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
        )
    (iter (readExpr))
    )
