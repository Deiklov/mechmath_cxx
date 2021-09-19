CC = g++ $(CXXFLAGS)
CXXFLAGS=-std=c++2a -Wall

compile:
	$(CC) -o orthocenter main.cpp R2Graph/R2Graph.cpp

clean:
	rm -f inCircle *.o
