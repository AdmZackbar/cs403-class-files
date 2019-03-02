(define (makeS function i)
    (define (iter n list)
        (cond
            ((> n i) list)
            (else
                (iter (+ n 1) (cons (+ (function n) (car list)) list))
                )
            )
        )
    (iter 0 (list 0))
    )

(define (S function i)
    (car (makeS function i))
    )

(define (w function i)
    (cond
        ((= i 0) (function i))
        (else
            (define sArray (makeS function (+ i 1)))
            (/ (- (* (car sArray) (car (cdr (cdr sArray)))) (* (car (cdr sArray)) (car (cdr sArray)))) (+ (- (car sArray) (* 2 (car (cdr sArray)))) (car (cdr (cdr sArray)))))
            )
        )
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define argFunction (eval (readExpr) this))
    (define arg1 (readInt))
    (println "(S " argFunction " " arg1 ") is " (fmt "%.15f" (S argFunction arg1)))
    (println "(w " argFunction " " arg1 ") is " (fmt "%.15f" (w argFunction arg1)))
    )
