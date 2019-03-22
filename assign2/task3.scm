(define ADD 1)
(define REMOVE 0)
(define SHIFT 2)


(define (Queue @)
    (cond
        ((nil? @)
            (define stack1 nil)
            (define stack2 nil)
            (define size 0)
            )
        ((= ADD (car @))
            (define stack1 (cons (cadr @) ((caddr @) 'stack1)))
            (define stack2 ((caddr @) 'stack2))
            (define size (+ ((caddr @) 'size) 1))
            )
        ((= SHIFT (car @))
            (define (iter item stack)
                (if (nil? item)
                    stack
                    (iter (cdr item) (cons (car item) stack))
                    )
                )
            (define stack1 nil)
            (define stack2 (iter ((cadr @) 'stack1) nil))
            (define size ((cadr @) 'size))
            )
        ((= REMOVE (car @))
            (if (nil? ((cadr @) 'stack2))
                (begin
                    (define shiftedQueue (Queue SHIFT (cadr @)))
                    (define stack1 (shiftedQueue 'stack1))
                    (define stack2 (cdr (shiftedQueue 'stack2)))
                    )
                (begin
                    (define stack1 ((cadr @) 'stack1))
                    (define stack2 (cdr ((cadr @) 'stack2)))
                    )
                )
            (define size (- ((cadr @) 'size) 1))
            )
        )
    this
    )

(define (enqueue queue item)
    (Queue ADD item queue)
    )

(define (dequeue queue)
    (if (!= (queue 'size) 0) (Queue REMOVE queue))
    )

(define (qpeek queue)
    (if (= (queue 'size) 0)
        nil
        (begin
            (if (nil? (queue 'stack2))
                (car ((Queue SHIFT queue) 'stack2))
                (car (queue 'stack2))
                )
            )
        )
    )

(define (qsize queue)
    (queue 'size)
    )


(define (Stack @)
    (cond
        ((nil? @)
            (define store nil)
            (define size 0)
            )
        ((= ADD (car @))
            (define store (cons (cadr @) ((caddr @) 'store)))
            (define size (+ ((caddr @) 'size) 1))
            )
        ((= REMOVE (car @))
            (define store (cdr ((cadr @) 'store)))
            (define size (- ((cadr @) 'size) 1))
            )
        )
    this
    )

(define (push stack item)
    (Stack ADD item stack)
    )

(define (pop stack)
    (if (!= (stack 'size) 0) (Stack REMOVE stack))
    )

(define (speek stack)
    (if (!= (stack 'size) 0) (car (stack 'store)))
    )

(define (ssize stack)
    (stack 'size)
    )


(define (main)
    (setPort (open (getElement ScamArgs 1) 'read))
    (define env this)
    (define (iter expr)
         (if (not (eof?)) (begin (eval expr env) (iter (readExpr))))
         )
    (iter (readExpr))
    )
