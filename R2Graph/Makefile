CC = g++ $(CFLAGS)
CFLAGS = -O0 -g

inCircle: inCircle.cpp R2Graph.h R2Graph.cpp
	$(CC) -o inCircle inCircle.cpp R2Graph.cpp

clean:
	rm -f inCircle *.o
