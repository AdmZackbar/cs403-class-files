(define (node value left right)
    (define (display) (print value))
    this
    )

(define (newBST value)
    (node value nil nil)
    )

(define (displayBST root)
    (define (iter root indent)
        (if (valid? root)
            (begin
                (iter (root'right) (string+ indent "    "))
                (print indent)
                ((root'display))
                (println)
                (iter (root'left) (string+ indent "    "))
                )
            )
        )
    (iter root "")
    )

(define (insertBST tree newVal)
    (define (insertLeft x)
        (if (nil? (tree 'left))
            (node (tree 'value) (node newVal nil nil) (tree 'right))
            (node (tree 'value) (insertBST (tree 'left) newVal) (tree 'right))
            )
        )
    (define (insertRight x)
        (if (nil? (tree 'right))
            (node (tree 'value) (tree 'left) (node newVal nil nil))
            (node (tree 'value) (tree 'left) (insertBST (tree 'right) newVal))
            )
        )
    (if (< newVal (tree 'value)) (insertLeft tree)
        (insertRight tree)
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
