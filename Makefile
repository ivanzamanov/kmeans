
FLAGS=-Wall -std=c++11 -g
CC=g++
CMD=$(CC) $(FLAGS)

all: clean
	mkdir bin
	cd src && \
	$(CMD) prepare.cpp -o ../bin/prepare
	cd ..

clean:
	rm -rf bin

