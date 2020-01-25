CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -g
LIBS = -L/usr/lib/X11
LDFLAGS = -lpthread -lz -lX11
DEPS = game_base.hpp game.hpp snake.hpp apple.hpp
VPATH = ./src

.PHONY: all
all: main

%.o: %.c $(DEPS)
	$(CXX) $(LIBS) -c -o $@ $<

main: main.o game.o snake.o apple.o
	$(CXX) -o main main.o game.o snake.o apple.o $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o main
