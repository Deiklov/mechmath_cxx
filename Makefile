CC = g++ $(CXXFLAGS)
CXXFLAGS=-std=c++17 -Wall -O0

compile:
	$(CC) -o earthdist main.cpp R3Graph/R3Graph.cpp
