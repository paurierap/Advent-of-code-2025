# C++17 needed for this one
	
CXX = g++
CXXFLAGS = -std=c++17 -Wall
OPTS = -O2
DEBUG = -g

all: main

main: main.cpp geometry.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) main.cpp geometry.cpp -o main

debug:
	$(CXX) $(CXXFLAGS) $(DEBUG) main.cpp geometry.cpp -o main

clean:
	rm -f main
