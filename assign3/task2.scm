(define (replace block exprList)
    (define (iter c)
        (define old (car exprList))
        (define new (cadr exprList))
        (cond
            ((nil? c) nil)
            ((object? c) nil)
            ((eq? (car c) 'quote)
                (if (eq? old 'quote) (set-car! c new))
                nil
                )
            ((list? (car c))
                (iter (car c))
                (iter (cdr c))
                )
            ((eq? (car c) old)
                (set-car! c new)
                (iter (cdr c))
                )
            (else (iter (cdr c)))
            )
        )
    (if (nil? exprList) nil
        (if (eq? (car exprList) 'quote)
            (begin  ; Delay this expr change
                (replace block (cddr exprList)) ; Handle code for the next sets of replacements
                (iter (get 'code block))        ; Handle the code block for the first set of replacements
                (iter (get 'parameters block))  ; Handle the parameters
                )
            (begin  ; Handle this expr change first
                (iter (get 'code block))        ; Handle the code block for the first set of replacements
                (iter (get 'parameters block))  ; Handle the parameters
                (replace block (cddr exprList)) ; Handle code for the next sets of replacements
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
