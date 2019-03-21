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
    (iter list nil)
    )

(define (main)
    (define env this)
    (define (iter expr)
         (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
         )
    (iter (readExpr))
    )
