(define (nand-gate a1 a2 output)
    (define (nand-action-procedure)
        (let ((new-value (logical-not (logical-and (get-signal a1) (get-signal a2)))))
            (after-delay nand-gate-delay
                (lambda ()
                    (print "\nNAND activated!")
                    (set-signal! output new-value)
                    )
                )
            )
        )
    (add-action! a1 nand-action-procedure)
    (add-action! a2 nand-action-procedure)
    'ok
    )

(define nand-gate-delay 6)
