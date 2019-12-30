CXX:=g++
GCC_VERSION=9.2.0
CXX:=$(CXX)-$(GCC_VERSION)
CXXFLAGS=-std=c++17 -O3 -Wall
CPPFLAGS=-I/usr/local/include/ImageMagick-7 -L/usr/local/lib
LINKER=-lpthread -lcurl -lMagick++
LDMAGICK=-DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=1

main: Photomosaics.o main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o main main.o Photomosaics.o $(LINKER)

main.o: main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c main.cpp $(LDMAGICK)

Photomosaics.o: Photomosaics.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Photomosaics.cpp $(LINKER) $(LDMAGICK)

:PHONY: clean
clean:
	rm -f *.o main
