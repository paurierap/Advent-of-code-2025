# C++17 needed for this one

CXX = g++
CXXFLAGS = -std=c++17 -Wall
DEBUG = -g

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

debug: main.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) main.cpp -o main

clean:
	rm -f main