(define (stream-map proc s)
    (cons-stream (proc (stream-car s))
        (stream-map proc (stream-cdr s))
        )
    )

(define (add-streams s1 s2)
    (cons-stream (+ (stream-car s1) (stream-car s2))
        (add-streams (stream-cdr s1) (stream-cdr s2))
        )
    )

(define (interleave s1 s2)
    (if (stream-null? s1) s2
        (cons-stream (stream-car s1)
            (interleave s2 (stream-cdr s1))
            )
        )
    )

(define (weighted-merge weight s1 s2)
    (cond ((stream-null? s1) s2)
        ((stream-null? s2) s1)
        (else
        (let ((s1car (stream-car s1))
            (s2car (stream-car s2)))
            (cond ((< (weight s1car) (weight s2car))
                (cons-stream s1car
                    (weighted-merge weight (stream-cdr s1) s2)))
                (else
                    (cons-stream s2car
                        (weighted-merge weight s1 (stream-cdr s2)))))))))

(define (weighted-pairs weight s t)
    (cons-stream
    (list (stream-car s) (stream-car t))
    (weighted-merge
        weight
        (stream-map (lambda (x) (list (stream-car s) x))
                    (stream-cdr t))
        (weighted-pairs weight (stream-cdr s) (stream-cdr t)))))

(define (ramanujan)
    (define (cube x)
        (* x x x)
        )
    (define (calc-result pair)
        (+ (cube (car pair)) (cube (cadr pair)))
        )
    (define (create-pairs s t)
        (cons-stream (list (stream-car s) (stream-car t))
            (interleave (stream-map (lambda (x) (list (stream-car s) x)) (stream-cdr t))
                (create-pairs (stream-cdr s) (stream-cdr t))
                )
            )
        )
    (define ones
        (cons-stream 1 ones)
        )
    (define integers
        (cons-stream 1
            (add-streams ones integers)
            )
        )
    (define pairs (weighted-pairs calc-result integers integers))
    (define numbers (stream-map calc-result pairs))
    (define (create-stream s)
        (let ((first (stream-car s))
            (second (stream-car (stream-cdr s))))
            (if (= (calc-result first) (calc-result second))
                (cons-stream first
                    (create-stream (stream-cdr s))
                    )
                (create-stream (stream-cdr s))
                )
            )
        )
    (create-stream numbers)
    )

(define (sdisplay num stream)
    (define (iter i strm)
        (if (= i num) nil
            (begin
                (print (stream-car strm))
                (print ',)
                (iter (+ i 1) (stream-cdr strm))
                )
            )
        )
    (print "(")
    (iter 0 stream)
    (print "...)")
    )

(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define env this)
    (define (iter expr)
        (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
        )
    (iter (readExpr))
    )
