(define (ramanujani x)
    (define (iter i)
        (cond
            ((> i x) 0.0)
            (else
                (* (+ 1.0 i) (sqrt (+ 6.0 i (iter (+ i 1)))))
                )
            )
        )
    (iter 0)
    )

(define (latexR x)
    (define (iter i)
        (cond
            ((> i x) 0)
            ((= i x)
                (string+ "" (+ 1 i) "\\cdot\\sqrt{" (+ 6 i) "}")
                )
            (else
                (string+ "" (+ 1 i) "\\cdot\\sqrt{" (+ 6 i) "+" (iter (+ i 1)) "}")
                )
            )
        )
    (string+ "$" (iter 0) "$")
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readInt))
    (println "(ramanujani " arg1 ") is " (fmt "%.25f" (ramanujani arg1)))
    (println (latexR arg1))
    )
