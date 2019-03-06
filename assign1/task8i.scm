(define (ecfi x)
    (define (iter term value)
        (cond
            ((<= term 0) value)
            (else (iter (- term 1) (/ 1.0 (+ 1.0 (/ 1.0 (+ (* 2.0 term) (/ 1.0 (+ 1.0 value))))))))
            )
        )
    (+ 2.0 (iter x 0.0))
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readExpr))
    (println "(ecfi " arg1 ") is " (fmt "%.25f" (ecfi arg1)))
    )
