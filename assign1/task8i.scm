(define (ecfi x)
    (define (iter i)
        (if (> i x)
            0
            (/ 1 (+ 1 (/ 1 (+ (* 2 i) (/ 1 (+ 1 (iter (+ i 1))))))))
            )
        )
    (+ 2.0 (iter 1.0))
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readInt))
    (println "(ecfi " arg1 ") is " (fmt "%.25f" (ecfi arg1)))
    )
