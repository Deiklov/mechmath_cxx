CC = g++ $(CXXFLAGS)
CXXFLAGS=-std=c++11 -Wall -O0

compile:
	$(CC) -o sorts main.cpp
