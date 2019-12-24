CXX := g++
CXXFLAGS=-std=c++17 -O2 -Wall
CPPFLAGS=-I /usr/local/include/ImageMagick-7 -L /usr/local/lib
LINKER=-lpthread -lcurl -lstdc++fs

main: Photomosaics.o main.o
	$(CXX) -o main $(CPPFLAGS) main.o Photomosaics.o $(CXXFLAGS) $(LINKER)

main.o: main.cpp
	$(CXX) -c $(CPPFLAGS) main.cpp

Photomosaics.o: Photomosaics.cpp
	$(CXX) -c $(CPPFLAGS) Photomosaics.cpp

:PHONY: clean
clean:
	rm -f *.o main
