CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -g
LDFLAGS = -lpthread -lcrypto
DEPS = hashing.hpp account.hpp database.hpp
VPATH = ./src

.PHONY: all
all: main

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $<

main: main.o hashing.o account.o database.o
	$(CXX) -o main main.o hashing.o account.o database.o $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o main
