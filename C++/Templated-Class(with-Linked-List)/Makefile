all: templateInt templateChar templateString

templateString: mainString.o
	g++ mainString.o -o templateString

mainString.o: mainString.cpp
	g++ -c mainString.cpp

templateChar: mainChar.o
	g++ mainChar.o -o templateChar

mainChar.o: main.cpp
	g++ -c mainChar.cpp

templateInt: main.o
	g++ main.o -o templateInt

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -rf *.o templateInt templateChar templateString