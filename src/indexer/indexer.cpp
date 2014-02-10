#include<stdio.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>

#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>

#include"utils.h"
#include"ptree.h"
#include"hash.h"

void processDirectory(const char* inputDirPath, int outputFD);
void processFile(int inputFD, int outputFD, ptree& p);

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Not enough arguments\n");
		printf("Usage: %s <input_dir> <output_file>", argv[0]);
		return 1;
	}

	int outputFD = -1;

	if(argc < 3) {
		outputFD = 1;
	} else {
		char* outputPath = argv[2];
		outputFD = open(outputPath, O_WRONLY | O_CREAT);
		if(outputFD < 0) {
			printf("Cannot open file %s\n", outputPath);
			return 1;
		}
	}

	const char* inputPath = argv[1];
	processDirectory(inputPath, outputFD);

	if(outputFD == 1) {
		printf("\n");
	}
}

int tokenCount = 0;
void processDirectory(const char* inputDirPath, int outputFD) {
	DIR* dirHandle = opendir(inputDirPath);
	struct dirent* entry = readdir(dirHandle);
	char path[4096];
	ptree p;
	while(entry != NULL) {
		sprintf(path, "%s/%s", inputDirPath, entry->d_name);
		int fd = open(path, O_RDONLY);
		if(fd < 0) {
			printf("Cannot open file %s\nCode = %d\n", path, fd);
			return;
		}
		processFile(fd, outputFD, p);
		close(fd);
		entry = readdir(dirHandle);
	}
	printf("Unique tokens: %d\n", tokenCount);
	
	closedir(dirHandle);
}

void expand(int** arr, int* cap);
void processFile(int inputFD, int outputFD, ptree& p) {
	char* fullData = 0;
	readFullData(inputFD, &fullData);

	char buffer[4096];
	int offset = 0;
	
	hash<int> h;
	bool hasToken = readToken(fullData, offset, buffer, 4096);
	if(hasToken) {
		int index = p.get(buffer);
		if(index < 0) {
			p.add(buffer, tokenCount);
			index = tokenCount;
			tokenCount++;
		}
		int tf = h.get(index, 0);
		tf++;
		h.insert(index, tf);
		
		while(readToken(fullData, offset, buffer, 4096)) {
			index = p.get(buffer);
			if(index < 0) {
				p.add(buffer, tokenCount);
				index = tokenCount;
				tokenCount++;
			}
			tf = h.get(index, 0);
			tf++;
			h.insert(index, tf);
		}
	}
	delete fullData;
}

