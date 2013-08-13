main : 	compilingError.h useGMP.h objects.o environments.o parse.o 	\
		execute.o builtIn.o userDefined.o extract.o main.o
	g++ main.o objects.o environments.o parse.o execute.o builtIn.o \
		userDefined.o extract.o -lm -std=c++11 -lgmp -lgmpxx -o main

main.o : main.cpp extract.h parse.h execute.h compilingError.h
	g++ main.cpp -c -std=c++11 -lgmp -lgmpxx
 
objects.o :	objects.cpp objects.h
	g++ objects.cpp -c -std=c++11 -lgmp -lgmpxx

environments.o :	environments.cpp environments.h
	g++ environments.cpp -c -std=c++11 -lgmp -lgmpxx

parse.o : parse.cpp parse.h
	g++ parse.cpp -c -std=c++11 -lgmp -lgmpxx

execute.o : execute.cpp execute.h
	g++ execute.cpp -c -std=c++11 -lgmp -lgmpxx

builtIn.o : builtIn.cpp builtIn.h
	g++ builtIn.cpp -c -std=c++11 -lgmp -lgmpxx

userDefined.o : userDefined.cpp userDefined.h
	g++ userDefined.cpp -c -std=c++11 -lgmp -lgmpxx

extract.o : extract.cpp extract.h
	g++ extract.cpp -c -std=c++11 -lgmp -lgmpxx

clear: 
	rm *.o main
