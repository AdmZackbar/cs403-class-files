(define (ramanujani x)
    (define (iter term value)
        (cond
            ((< term 0) value)
            (else (iter (- term 1) (* (+ 1 term) (sqrt (+ 6 term value)))))
            )
        )
    (iter x 0)
    )

(define (latexI x)
    (define (iter term value)
        (cond
            ((< term 0) value)
            ((= term x) (iter (- term 1) (string+ "" (+ 1 term) "\\cdot\\sqrt{" (+ 6 term) "}")))
            (else (iter (- term 1) (string+ "" (+ 1 term) "\\cdot\\sqrt{" (+ 6 term) "+" value "}")))
        )
    (iter x nil)
    )

(string+ "" (+ 1 i) "\\cdot\\sqrt{" (+ 6 i) "}")
(string+ "" (+ 1 i) "\\cdot\\sqrt{" (+ 6 i) "+" (recur (+ i 1)) "}")


(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define arg1 (readInt))
    (println "(ramanujani " arg1 ") is " (fmt "%.25f" (ramanujani arg1)))
    (println (latexI arg1))
    )
