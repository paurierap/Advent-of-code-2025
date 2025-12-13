# C++17 needed for this one

CXX = g++
CXXFLAGS = -std=c++17 -Wall -lglpk
OPTS = -O2
DEBUG = -g

all: main

main: main.cpp functions.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) main.cpp functions.cpp -o main

debug: main.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) main.cpp -o main

clean:
	rm -f main
