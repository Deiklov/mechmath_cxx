CC = g++ $(CXXFLAGS)
CXXFLAGS=-std=c++2a -Wall -O0

compile:
	$(CC) -o earthdist main.cpp R3Graph/R3Graph.cpp
