(define (range start end step)
    (define (iter i)
        (if (>= i end)
            nil
            (cons i (iter (+ i step)))
            )
        )
    (iter start)
    )

(define (main)
    (inspect (range 0 10 1))
    )
