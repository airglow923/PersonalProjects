CXX:=g++
# GCC_VERSION=9.2.0
# CXX:=$(CXX)-$(GCC_VERSION)
CXXFLAGS=-std=c++17 -O3 -Wall
LINKER=-lpthread -lcurl -llexbor
CPPMAGICK=`Magick++-config --cppflags --cxxflags`
LDMAGICK=`Magick++-config --ldflags --libs`

main: Photomosaics.o dw_image.o main.o
	$(CXX) $(CXXFLAGS) main.o Photomosaics.o dw_image.o $(LINKER) $(LDMAGICK) -o main

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(CPPMAGICK) -c main.cpp

Photomosaics.o: Photomosaics.cpp
	$(CXX) $(CXXFLAGS) $(CPPMAGICK) -c Photomosaics.cpp

dw_image.o: dw_image.cpp
	$(CXX) $(CXXFLAGS) -c dw_image.cpp

:PHONY: clean
clean:
	rm -f *.o main
