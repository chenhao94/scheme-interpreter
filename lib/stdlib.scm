(define nil '())
 
( define ( caar pair ) ( car ( car pair ) ) )
( define ( cadr pair ) ( car ( cdr pair ) ) )
( define ( cdar pair ) ( cdr ( car pair ) ) )
( define ( cddr pair ) ( cdr ( cdr pair ) ) )
( define ( caaar pair ) ( car ( car ( car pair ) ) ) )
( define ( caadr pair ) ( car ( car ( cdr pair ) ) ) )
( define ( cadar pair ) ( car ( cdr ( car pair ) ) ) )
( define ( caddr pair ) ( car ( cdr ( cdr pair ) ) ) )
( define ( cdaar pair ) ( cdr ( car ( car pair ) ) ) )
( define ( cdadr pair ) ( cdr ( car ( cdr pair ) ) ) )
( define ( cddar pair ) ( cdr ( cdr ( car pair ) ) ) )
( define ( cdddr pair ) ( cdr ( cdr ( cdr pair ) ) ) )
( define ( caaaar pair ) ( car ( car ( car ( car pair ) ) ) ) )
( define ( caaadr pair ) ( car ( car ( car ( cdr pair ) ) ) ) )
( define ( caadar pair ) ( car ( car ( cdr ( car pair ) ) ) ) )
( define ( caaddr pair ) ( car ( car ( cdr ( cdr pair ) ) ) ) )
( define ( cadaar pair ) ( car ( cdr ( car ( car pair ) ) ) ) )
( define ( cadadr pair ) ( car ( cdr ( car ( cdr pair ) ) ) ) )
( define ( caddar pair ) ( car ( cdr ( cdr ( car pair ) ) ) ) )
( define ( cadddr pair ) ( car ( cdr ( cdr ( cdr pair ) ) ) ) )
( define ( cdaaar pair ) ( cdr ( car ( car ( car pair ) ) ) ) )
( define ( cdaadr pair ) ( cdr ( car ( car ( cdr pair ) ) ) ) )
( define ( cdadar pair ) ( cdr ( car ( cdr ( car pair ) ) ) ) )
( define ( cdaddr pair ) ( cdr ( car ( cdr ( cdr pair ) ) ) ) )
( define ( cddaar pair ) ( cdr ( cdr ( car ( car pair ) ) ) ) )
( define ( cddadr pair ) ( cdr ( cdr ( car ( cdr pair ) ) ) ) )
( define ( cdddar pair ) ( cdr ( cdr ( cdr ( car pair ) ) ) ) )
( define ( cddddr pair ) ( cdr ( cdr ( cdr ( cdr pair ) ) ) ) )

( define ( id obj ) obj )
( define ( foldl func accum lst )
( if ( null? lst )
accum
( foldl func ( func accum ( car lst ) ) ( cdr lst ) ) ) )
( define fold foldl )
( define (mem-helper pred op ) ( lambda ( acc next ) ( if (and (not acc ) (pred
( op next ) ) ) next acc ) ) )
 
( define (not x ) ( if x #f #t ) )
( define ( null? obj ) ( if ( eqv? obj '( ) ) #t #f ) )

( define (odd? num) (= (modulo num 2) 1) )
( define ( even? num) (= (modulo num 2) 0) )
( define ( curry func arg1 ) ( lambda ( arg ) ( func arg1 arg ) ) )
( define zero? ( curry = 0) )
( define positive? ( curry < 0) )
( define negative? ( curry > 0) )
( define ( length lst ) ( fold ( lambda ( x y ) (+ x 1) ) 0 lst ) )
( define (abs x) (if (< x 0) (- x) x))

( define (memq obj lst ) ( fold (mem-helper ( curry eq? obj ) id ) #f 
lst ) )
( define (memv obj lst ) ( fold (mem-helper ( curry eqv? obj ) id ) #f 
lst ) )
( define (member obj lst ) ( fold (mem-helper ( curry equal? obj ) id ) #f 
lst ) )


