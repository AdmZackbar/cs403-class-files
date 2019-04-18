(include "../queue.scm")
(include "../gates.scm")
(include "../nand-act.scm")

(define the-agenda (make-agenda))

(define (inverter input output)
    (nand-gate input input output)
    )

(define (and-gate a1 a2 output)
    (let ((nand-output (make-wire)))
        (nand-gate a1 a2 nand-output)
        (inverter nand-output output)
        )
    )

(define (or-gate a1 a2 output)
    (let ((inverted-a1 (make-wire))
          (inverted-a2 (make-wire)))
        (inverter a1 inverted-a1)
        (inverter a2 inverted-a2)
        (nand-gate inverted-a1 inverted-a2 output)
        )
    )

(define (nor-gate a1 a2 output)
    (let ((or-output (make-wire)))
        (nor-gate a1 a2 or-output)
        (inverter or-output output)
        )
    )

(define (xor-gate a1 a2 output)
    (let ((output-1 (make-wire))
          (output-2 (make-wire))
          (output-3 (make-wire)))
        (nand-gate a1 a2 output-1)
        (nand-gate a1 output-1 output-2)
        (nand-gate a2 output-1 output-3)
        (nand-gate output-2 output-3 output)
        )
    )

(define (xnor-gate a1 a2 output)
    (let ((inverted-a1 (make-wire))
          (inverted-a2 (make-wire))
          (output-1 (make-wire))
          (output-2 (make-wire)))
        (inverter a1 inverted-a1)
        (inverter a2 inverted-a2)
        (nand-gate inverted-a1 inverted-a2 output-1)
        (nand-gate a1 a2 output-2)
        (nand-gate output-1 output-2 output)
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
