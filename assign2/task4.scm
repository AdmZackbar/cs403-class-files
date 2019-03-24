(define (translate statements)
    (define (add-lambda assignment)
        (if (nil? assignment)
            (cddr (car statements))
            (list (append (list 'lambda (list (car (car assignment)))) (add-lambda (cdr assignment))) (cadr (car assignment)))
            )
        )
    (if (nil? statements) nil
        (if (eq? (caar statements) 'let*)
            (cons (add-lambda (cadr (car statements))) (translate (cdr statements)))
            (cons (car statements) (translate (cdr statements)))
            )
        )
    )

(define (let*->lambdas arg)
    (append (list (car arg) (cadr arg)) (translate (cddr arg)))
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define env this)
    (define (iter expr)
         (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
         )
    (iter (readExpr))
    )
