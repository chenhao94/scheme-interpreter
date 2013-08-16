main : 	error.h useGMP.o objects.o environments.o parse.o \
	execute.o builtIn.o userDefined.o extract.o display.o \
	main.o parseTreePrinter
	g++ main.o useGMP.o objects.o environments.o parse.o \
		execute.o builtIn.o userDefined.o extract.o      \
		display.o -lm -std=c++11 -lgmp -lgmpxx -o main -g

main.o : main.cpp extract.h parse.h execute.h error.h
	g++ main.cpp -c -std=c++11 -lgmp -lgmpxx -g
 
useGMP.o :	useGMP.cpp useGMP.h
	g++ useGMP.cpp -c -std=c++11 -lgmp -lgmpxx -g

objects.o :	objects.cpp objects.h
	g++ objects.cpp -c -std=c++11 -lgmp -lgmpxx -g

environments.o :	environments.cpp environments.h
	g++ environments.cpp -c -std=c++11 -lgmp -lgmpxx -g

parse.o : parse.cpp parse.h
	g++ parse.cpp -c -std=c++11 -lgmp -lgmpxx -g

execute.o : execute.cpp execute.h
	g++ execute.cpp -c -std=c++11 -lgmp -lgmpxx -g

builtIn.o : builtIn.cpp builtIn.h
	g++ builtIn.cpp -c -std=c++11 -lgmp -lgmpxx -g

userDefined.o : userDefined.cpp userDefined.h
	g++ userDefined.cpp -c -std=c++11 -lgmp -lgmpxx -g

extract.o : extract.cpp extract.h
	g++ extract.cpp -c -std=c++11 -lgmp -lgmpxx -g

display.o : display.cpp display.h
	g++ display.cpp -c -std=c++11 -lgmp -lgmpxx -g

parseTreePrinter : parseTreePrinter.o parse.o
	g++ parseTreePrinter.o parse.o -lm -lgmp -lgmpxx -std=c++11 -o parseTreePrinter -g

parseTreePrinter.o : parseTreePrinter.cpp parse.h 
	g++ parseTreePrinter.cpp -c -lgmp -lgmpxx -std=c++11 -g
	
clear: 
	rm *.o *~ main parseTreePrinter

