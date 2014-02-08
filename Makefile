
FLAGS=-Wall -std=c++11 -g
CC=g++
CMD=$(CC) $(FLAGS)

export

all: clean extractor tokenizer

clean:
	rm -rf bin
	mkdir bin

extractor:
	$(MAKE) -C src/extractor all

tokenizer:
	$(MAKE) -C src/tokenizer all
