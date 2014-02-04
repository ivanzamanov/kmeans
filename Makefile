
FLAGS=-Wall -std=c++11 -g
CC=g++
CMD=$(CC) $(FLAGS)

all: clean
	mkdir bin
	cd src && \
	$(CMD) extractor.cpp matcher.cpp matcher.h -o ../bin/extractor
	cd ..

clean:
	rm -rf bin

