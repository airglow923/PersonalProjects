CXX	:= g++

CXXFLAGS=-std=c++17 -O2 -Wall

main: TicTacToe.o Extension.o main.o
	$(CXX) -o main main.o TicTacToe.o Extension.o $(CXXFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp

TicTacToe.o: TicTacToe.cpp
	$(CXX) -c TicTacToe.cpp

Extension.o: Extension.cpp
	$(CXX) -c Extension.cpp

:PHONY: clean
clean:
	rm -f *.o main
