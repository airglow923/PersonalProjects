CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -g
LDFLAGS = -lpthread -lcrypto -lsqlite3
DEPS = hashing.hpp account.hpp database.hpp database.tpp
VPATH = ./src

GCCVERSION := $(shell expr `gcc -dumpversion | cut -f1 -d.` \< 9)

ifeq "$(GCCVERSION)" "1"
	LDFLAGS += -lstdc++fs
endif

.PHONY: all
all: main

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $<

main: main.o hashing.o account.o database.o
	$(CXX) -o main main.o hashing.o account.o database.o $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o main
