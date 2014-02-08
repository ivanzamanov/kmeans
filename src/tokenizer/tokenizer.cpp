#include<stdio.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>

#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

#include"utils.h"

void processFile(int inputFD, int outputFD, struct stat* fileStat);

int main(int argc, char** argv) {
	int outputFD = -1;
	if(argc < 2) {
		printf("Not enough arguments\n");
		printf("Usage: %s <input_file> [<output_file>]\n", argv[0]);
		return 1;
	} else if (argc == 2) {
		outputFD = 1;
	} else {
		char* outputPath = argv[2];
		outputFD = open(outputPath, O_WRONLY);
		if(outputFD < 0) {
			printf("Cannot open file %s\n", outputPath);
			return 1;
		}
	}

	char* inputPath = argv[1];
	int inputFD = open(inputPath, O_RDONLY);
	if(inputFD < 0) {
		printf("Cannot open file %s\n", inputPath);
		return 1;
	}

	struct stat fileStat;
	if(fstat(inputFD, &fileStat) != 0) {
		printf("Cannot stat file %s\n", inputPath);
		return 1;
	}

	processFile(inputFD, outputFD, &fileStat);
}

void processFile(int inputFD, int outputFD, struct stat* const fileStat) {
	char* fullData = new char[fileStat->st_size];
	int bytes = read(inputFD, fullData, fileStat->st_size);
	fullData[fileStat->st_size] = 0;

	if (bytes != fileStat->st_size) {
		printf("Could not read entire file\n");
	} else {
		char* buffer = new char[4096];
		int offset = 0;
		while(readToken(fullData, offset, buffer, 4096)) {
			lowercaseToken(buffer);
			writeToken(buffer, outputFD);
		}
	}
	delete fullData;
}

