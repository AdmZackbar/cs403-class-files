(define (accumulate op init matrix)
    (if (nil? matrix)
        init
        (op (car matrix) (accumulate op init (cdr matrix)))
        )
    )

(define (accumulate-n op init matrix)
    (if (nil? (car matrix))
        '()
        (cons (accumulate op init (map car matrix)) (accumulate-n op init (map cdr matrix)))
        )
    )

(define (dot-product v w)
    (accumulate + 0 (map * v w))
    )

(define (transpose matrix)
    (accumulate-n cons nil matrix)
    )

(define (matrix-*-vector matrix vector)
    (map (lambda (row)
            (dot-product row v)
            )
        matrix
        )
    )

(define (matrix-*-matrix mat1 mat2)
    (transpose (map (lambda (x)
            (map (lambda (y)
                    (dot-product x y)
                    )
                mat2
                )
            )
        (transpose mat1)
        ))
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define env this)
    (define (iter expr)
         (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
         )
    (iter (readExpr))
    )