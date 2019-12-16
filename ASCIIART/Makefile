CC	:= gcc
CXX	:= g++ 

CPPFLAGS=-O2 -Wall
CXXFLAGS=-std=c++17 -O2 -Wall
LDFLAGS=-lm -lpthread -lz -ldl
LDOPENCV=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml\
	 -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_calib3d\
	 -lopencv_objdetect -lopencv_flann -lopencv_imgcodecs

webcam: webcam.o
	$(CXX) -o webcam webcam.o $(CXXFLAGS) $(LDFLAGS) $(LDOPENCV)

webcam.o: webcam.cpp
	$(CXX) -c webcam.cpp

.PHONY: clean
clean:
	rm -f *.o webcam
