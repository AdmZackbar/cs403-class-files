(define (scoping var obj)
    (define (checkLocal varList)
        (if (nil? varList) #f
            (if (eq? var (car varList)) #t (checkLocal (cdr varList)))
            )
        )
    (define (checkNonLocal env)
        (if (nil? env) #f
            (if (checkLocal (cadr env)) #t
                (checkNonLocal (env '__context))
                )
            )
        )
    (if (checkLocal (cadr obj)) 'bound
        (if (checkNonLocal (obj '__context)) 'free 'undefined)
        )
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define env this)
    (define (iter expr)
        (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
        )
    (iter (readExpr))
    )
