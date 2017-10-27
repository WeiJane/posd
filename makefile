all: hw4

hw4: mainTerm.o number.o variable.o atom.o struct.o list.o
ifeq (${OS}, Windows_NT)
	g++ -o hw4 mainTerm.o number.o variable.o atom.o struct.o list.o -lgtest
else
	g++ -o hw4 mainTerm.o number.o variable.o atom.o struct.o list.o -lgtest -lpthread
endif

mainTerm.o: mainTerm.cpp utVariable.h utStruct.h utList.h
	g++ -std=gnu++0x -c mainTerm.cpp

number.o: number.h number.cpp
	g++ -std=gnu++0x -c number.cpp

variable.o: variable.h variable.cpp
	g++ -std=gnu++0x -c variable.cpp

atom.o: atom.h atom.cpp
	g++ -std=gnu++0x -c atom.cpp

struct.o: struct.h struct.cpp
	g++ -std=gnu++0x -c struct.cpp

list.o: list.h list.cpp
	g++ -std=gnu++0x -c list.cpp

clean:
	rm -f *.o *.exe hw4

stat:
	wc *.h *.cpp
