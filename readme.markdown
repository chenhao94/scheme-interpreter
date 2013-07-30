#A simple interpreter of Scheme ([R5RS](http://www.schemers.org/Documents/Standards/R5RS/))

##introduction

This is the course project of Principle and Practice of Computer Algorithms 2013 (PPCA2013). The interpreter is designed to impterprete a subset of R5RS, including the basic function of this programming language.

For simplicity, I tacity assume that the identifier defined by the users is the combination of *letters*, *numbers* and *underscores*
which start with letter, which means, the followings are illegal.

    (define a->b 1)
    (define 1a 2)
	(define _a 3)

And I suppose that users will never redefine the preserved keywords (e.g., *if* and *define*), e.g.,
    
    (define define 1)

I believe the users will not use the definition above.

##Function
The followings are available in the interpreter.

##Reference
[1] Revised 5 Report on the Algorithmic Language Scheme, Richard Kelsey, William Clinger and Jonathan Rees

[2] GNU MP, The GNU Multiple Precision Arithmetic Library (Edition 5.1.2), Torbjörn Granlund and the GMP development team
