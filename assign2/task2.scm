(define (append-list list1 list2)
    (define (iter index)
        (if (nil? (cdr index))
            (cons (car index) list2)
            (cons (car index) (iter (cdr index)))
            )
        )
    (iter list1)
    )

(define (curry function @)
    (if (nil? @)
        (define params nil)
        (define params (car @))
        )
    (inspect params)
    (if (= (length params) (length (get 'parameters function)))
        (apply function params)
        (lambda (@)
            (if (nil? params)
                (curry function @)
                (curry function (append-list params @))
                )
            )
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
