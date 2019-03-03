(define (ecfr x)
    (cond
        ((<= x 0) 1.0)
        (else
            (/ (ecfr (- x 1)) (+ 1 (/ 1 (+ (* 2 x) 1))))
            )
        )
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readInt))
    (println "(ecfr " arg1 ") is " (fmt "%.25f" (ecfr arg1)))
    )
