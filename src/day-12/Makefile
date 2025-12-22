# C++20 needed for this one

CXX = g++
CXXFLAGS = -std=c++20 -Wall
OPTS = -O2
DEBUG = -g

all: main

main: main.cpp utils.hpp
	$(CXX) $(CXXFLAGS) $(OPTS) main.cpp -o main

debug: main.cpp utils.hpp
	$(CXX) $(CXXFLAGS) $(DEBUG) main.cpp -o main

clean:
	rm -f main *.txt