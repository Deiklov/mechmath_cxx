CC = g++ $(CFLAGS)
CFLAGS = -O0 -g

russianWords: russianWords.cpp utf8/utf8.h utf8/utf8.cpp
	$(CC) -o russianWords russianWords.cpp utf8/utf8.cpp

clean:
	rm -f russianWords russianWords.exe
