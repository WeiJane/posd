all: hw5

hw5: mainTerm.o number.o variable.o atom.o struct.o list.o
ifeq (${OS}, Windows_NT)
	g++ -o hw5 mainTerm.o number.o variable.o atom.o struct.o list.o -lgtest
else
	g++ -o hw5 mainTerm.o number.o variable.o atom.o struct.o list.o -lgtest -lpthread
endif

#test: global.o test.o
#ifeq (${OS}, Windows_NT)
#	g++ -o hw4 global.o test.o -lgtest
#else
#	g++ -o hw4 global.o test.o -lgtest -lpthread
#endif
#
#test.o: test.cpp global.h
#	g++ -std=gnu++0x -c test.cpp

mainTerm.o: mainTerm.cpp utVariable.h utStruct.h utList.h utScanner.h utParser.h scanner.h parser.h
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

#parser.o: parser.h parser.cpp
#	g++ -std=gnu++0x -c parser.cpp

#global.o: global.h global.cpp
#	g++ -std=gnu++0x -c global.cpp

clean:
	rm -f *.o *.exe hw5

stat:
	wc *.h *.cpp
