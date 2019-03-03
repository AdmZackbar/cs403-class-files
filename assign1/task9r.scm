(define (ramanujanr x)
    (cond
        ((= x 0) (* 1 (sqrt 6)))
        (else
            
            )
        )
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readInt))
    ;(println "(ramanujanr " arg1 ") is " (fmt "%.25f" (ramanujanr arg1)))
    )
