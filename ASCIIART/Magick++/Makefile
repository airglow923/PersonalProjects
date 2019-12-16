CC	:= gcc
CXX	:= g++ 

CPPFLAGS=-O2 -Wall
CXXFLAGS=-std=c++17 -O2 -Wall
LDFLAGS=-lm -lpthread -lMagick++ -lz -ldl
LDOPENCV=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml\
	 -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_calib3d\
	 -lopencv_objdetect -lopencv_flann -lopencv_imgcodecs

asciiart: asciiart.o
	$(CXX) -o asciiart asciiart.o $(CXXFLAGS) $(LDFLAGS) $(LDOPENCV)

asciiart.o: asciiart.cpp
	$(CXX) -c asciiart.cpp

.PHONY: clean
clean:
	rm -f *.o asciiart *.out
