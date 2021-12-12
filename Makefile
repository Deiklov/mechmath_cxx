CC = g++ $(CXXFLAGS)
CXXFLAGS=-std=c++11 -Wall -O0

russianWords: main.cpp utf8/utf8.h utf8/utf8.cpp
	$(CC) -o bigrams main.cpp utf8/utf8.cpp

clean:
	rm -f russianWords russianWords.exe
