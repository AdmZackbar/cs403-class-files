(define (mandelbrot threshold)
    (lambda (x y)
        (define (iter r s numIter)
            (cond
                ((> (+ (* r r) (* s s)) 4) numIter)
                ((>= numIter threshold) 0)
                (else
                    (iter (+ (- (* r r) (* s s)) x) (+ (* 2 r s) y) (+ numIter 1))
                    )
                )
            )
            (iter 0.0 0.0 0)
        )
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define threshold (readExpr))
    (define x (readExpr))
    (define y (readExpr))
    (println "((mandelbrot " threshold ") " x " " y ") is " ((mandelbrot threshold) x y))
    )
