#include<stdio.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>

#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

#include"utils.h"
#include"stem.h"

void processFile(int inputFD, int outputFD, struct stat* fileStat);

int main(int argc, char** argv) {
	int inputFD = -1;
	int outputFD = -1;

	const char* inputPath = "STDIN";
	if(argc < 2) {
		inputFD = 0;
	} else {
		inputPath = argv[1];
		inputFD = open(inputPath, O_RDONLY);
		if(inputFD < 0) {
			printf("Cannot open file %s\n", inputPath);
			return 1;
		}
	}

	if(argc < 3) {
		outputFD = 1;
	} else {
		char* outputPath = argv[2];
		outputFD = open(outputPath, O_WRONLY | O_CREAT, 0664);
		if(outputFD < 0) {
			printf("Cannot open file %s\n", outputPath);
			return 1;
		}
	}

	struct stat fileStat;
	if(fstat(inputFD, &fileStat) != 0) {
		printf("Cannot stat file %s\n", inputPath);
		return 1;
	}

	processFile(inputFD, outputFD, &fileStat);

	if(outputFD == 1) {
		printf("\n");
	}
}

void processFile(int inputFD, int outputFD, struct stat* const fileStat) {
	char* fullData = 0;
	readFullData(inputFD, &fullData);

	char* buffer = new char[4096];
	int offset = 0;
	readToken(fullData, offset, buffer, 4096);
	lowercaseToken(buffer);
	writeToken(buffer, outputFD);
	while(readToken(fullData, offset, buffer, 4096)) {
		writeToken(" ", outputFD);
		lowercaseToken(buffer);
		writeToken(buffer, outputFD);
	}
	delete fullData;
}

