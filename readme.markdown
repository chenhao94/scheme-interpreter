#A simple interpreter of Scheme ([R5RS](http://www.schemers.org/Documents/Standards/R5RS/))

##introduction

This is the course project of Principle and Practice of Computer Algorithms 2013 (PPCA2013). The interpreter is designed to impterprete a subset of R5RS, including the basic function of this programming language.

For simplicity, I tacity assume that the identifier defined by the users is the combination of *letters*, *numbers* and *underscores*
which start with letter, others behavoir might be unpredictable.

And I suppose that users will never redefine the preserved keywords (e.g., *if* and *define*), e.g.,
    
    (define define 1)

I believe the users will not use the definition above. Redefining the built-in syntax is banned( the interpreter will ignore it). You can redefine the built-in procedures. It might works, but sometimes may leads to unexpected outcomes.

##Compilation and Run

You should install the GNU MP 5.1.2 first. If your operating system have Advanced Packaging Tool (apt), then,

	sudo apt-get install libgmp3c2 libgmp10-doc libgmp3-dev libgmpxx4ldbl

Then go to the root directory of the interpreter

	make

Then it will be compiled successfully. And you can type the following command to run it

	./main

And you will see

	Welcome to CH's scheme Interpreter!!
	> 

Enjoy it!


##Syntax
The followings are available in the interpreter now.

* if, cond, case
* define
* set!
* lamdba
* and, or
* quote, quasiquote(expect ',@')
* delay
* begin

##Procedure
The followings are available in the interpreter now.

* +, -, \*, /
* quotient, modulo, remainder
* <, >, <=, >=, =
* min, max
* gcd, lcm
* eq?, eqv?, equal?
* cons, car, cdr
* symbol?, string?, number?, bool?, list?, pair?
* string=?, string<?, string>?, string<=?, string>=?
* force
* append
* apply
* display

##Postscript
According to the requirement, the interpreter is programmed individually, but there are some inspiring talks with Zhuoyue Zhao, my classmate.

##Reference
[1] Revised 5 Report on the Algorithmic Language Scheme, Richard Kelsey, William Clinger and Jonathan Rees

[2] GNU MP, The GNU Multiple Precision Arithmetic Library (Edition 5.1.2), Torbjörn Granlund and the GMP development team
