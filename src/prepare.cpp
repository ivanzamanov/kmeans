#include"matcher.h"

#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>

void processFile(int fd, struct stat* const fileStat);
int main(int argc, char** argv) {
  if(argc < 2) {
    printf("No input file specified\n");
  }

  char* inputPath = argv[1];
  int fd = open(inputPath, O_RDONLY);
  if(fd < 0) {
    printf("Cannot open file %s\n", inputPath);
    return 1;
  }

  struct stat fileStat;
  if(fstat(fd, &fileStat) != 0) {
    printf("Cannot stat file %s\n", inputPath);
    return 1;
  }
  
  processFile(fd, &fileStat);
}

void processFile(int fd, struct stat* const fileStat) {
  char* fullData = new char[fileStat->st_size];
  int bytes = read(fd, fullData, fileStat->st_size);
  fullData[fileStat->st_size] = 0;
  if (bytes != fileStat->st_size) {
    printf("Could not read entire file\n");
  } else {
    matcher* start_m;
    matcher* end_m;
    build_matcher("<BODY>", start_m);
    build_matcher("</BODY>", end_m);
    printf("%d\n", search(fullData, start_m) < search(fullData, end_m));
  }
  delete fullData;
}
