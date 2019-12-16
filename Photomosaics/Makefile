CXX := g++
CXXFLAGS=-std=c++17 -O2 -Wall
LDFLAGS=-lpthread -lcurl

main: Photomosaics.o main.o
	$(CXX) -o main main.o Photomosaics.o $(CXXFLAGS) $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c main.cpp

Photomosaics.o: Photomosaics.cpp
	$(CXX) -c Photomosaics.cpp

:PHONY: clean
clean:
	rm -f *.o main
