CXX	:= g++

CXXFLAGS=-std=c++17 -O2 -Wall
LDFLAGS=-lm -lpthread -lstdc++fs

main: GameOfLife.o LangtonsAnt.o main.o
	$(CXX) -o main main.o GameOfLife.o LangtonsAnt.o $(CXXFLAGS) $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp

GameOfLife.o: GameOfLife.cpp
	$(CXX) -c GameOfLife.cpp

LangtonsAnt.o: LangtonsAnt.cpp
	$(CXX) -c LangtonsAnt.cpp

.PHONY: clean
clean:
	rm -f *.o main
