CXX:=g++
# GCC_VERSION=9.2.0
# CXX:=$(CXX)-$(GCC_VERSION)
CXXFLAGS=-std=c++17 -O3 -Wall
LINKER=-lpthread -lcurl -lstdc++fs
CPPMAGICK=`Magick++-config --cppflags --cxxflags`
LDMAGICK=`Magick++-config --ldflags --libs`

main: Photomosaics.o main.o
	$(CXX) $(CXXFLAGS) $(CPPMAGICK) main.o Photomosaics.o -o main $(LINKER) $(LDMAGICK)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(CPPMAGICK) -c main.cpp $(LDMAGICK)

Photomosaics.o: Photomosaics.cpp
	$(CXX) $(CXXFLAGS) $(CPPMAGICK) -c Photomosaics.cpp $(LINKER) $(LDMAGICK)

:PHONY: clean
clean:
	rm -f *.o main
