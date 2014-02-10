
FLAGS=-Wall -std=c++11 -g
LIB_DIRS=-I../common -L../../bin
LIBS=-lkm_common
CC=g++
CMD=$(CC) $(FLAGS) $(LIB_DIRS)

export

all: clean extractor tokenizer stemmer indexer

clean:
	rm -rf bin
	mkdir bin

common:
	$(MAKE) -C src/common

extractor: common
	$(MAKE) -C src/extractor

tokenizer: common
	$(MAKE) -C src/tokenizer

stemmer: common
	$(MAKE) -C src/stemmer

indexer: common
	$(MAKE) -C src/indexer
