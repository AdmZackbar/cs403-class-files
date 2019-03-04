(define (ramanujanr x)
    (define (recur i)
        (cond
            ((> i x) 0.0)
            (else
                (* (+ 1.0 i) (sqrt (+ 6.0 i (recur (+ i 1)))))
                )
            )
        )
    (recur 0)
    )

(define (latexR x)
    (define (recur i)
        (cond
            ((> i x) 0)
            ((= i x)
                (string+ "" (+ 1 i) "\\cdot\\sqrt{" (+ 6 i) "}")
                )
            (else
                (string+ "" (+ 1 i) "\\cdot\\sqrt{" (+ 6 i) "+" (recur (+ i 1)) "}")
                )
            )
        )
    (string+ "$" (recur 0) "$")
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readInt))
    (println "(ramanujanr " arg1 ") is " (fmt "%.25f" (ramanujanr arg1)))
    (println (latexR arg1))
    )
