(define (root-n n)
    (lambda (x)
        (define (close? y oldY)
            (< (abs (- y oldY)) 0.000000000000000001)
            )
        (define (improve y)
            (/ (+ (* (- n 1) y) (/ x (expt y (- n 1)))) n)
            )
        (define (iter y oldY)
            (if (close? y oldY)
                y
                (iter (improve y) y)
                )
            )
        (if (= x 0)
            0.0
            (iter 1.0 0.0)
            )
        )
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define root (readReal))
    (define x (readReal))
    (println "((root-n " root ") " x ") is " (fmt "%.15f" ((root-n root) x)))
    )
