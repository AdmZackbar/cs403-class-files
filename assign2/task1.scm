(define (range start end step)
    (define (iter i)
        (if (>= i end)
            nil
            (cons i (iter (+ i step)))
            )
        )
    (iter start)
    )

(define (for-loop list procedure)
    (define (iter arg_list return)
        (if (null? arg_list)
            return
            (iter (cdr arg_list) (procedure (car arg_list)))
            )
        )
    (iter (car list nil))
    )

(define (main)
    (inspect (range 0 10 1))
    (for-loop (range 0 10 5) (lambda (x) (inspect x)))
    )
