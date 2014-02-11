

all: build

build:
	$(CMD) utils.cpp matcher.cpp stem.cpp -c && \
	ar rvs ../../bin/libkm_common.a *.o && \
	rm *.o
