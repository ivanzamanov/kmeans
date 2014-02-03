#include<stdio.h>
#include<regex>
#include<string>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<iostream>

void processFile(int fd, struct stat* const fileStat);
int main(int argc, char** argv) {
//  std::regex reg("(.*)(cc)(.*)");
//  printf("%d\n", (int) regex_match("ccccc", reg));
//  exit(0);
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
  
  off_t size = fileStat.st_size;

  processFile(fd, &fileStat);
}

void processFile(int fd, struct stat* const fileStat) {
  char* fullData = new char[fileStat->st_size];
  int bytes = read(fd, fullData, fileStat->st_size);
  fullData[fileStat->st_size] = 0;
  if (bytes != fileStat->st_size) {
    printf("Could not read entire file\n");
  } else {
    std::string* str = new std::string(fullData, bytes);
    std::regex matcher("[a-zA-Z_][a-zA-Z_0-9]*\\.[a-zA-Z0-9]+");
    std::cmatch results;
    
    bool matched = regex_search(fullData, results, matcher);
    printf("%s\n", matched ? "matched" : "not matched");
    for(unsigned int i=0; i<results.size(); i++) {
      printf("%s\n", results.str().data());
    }
  }
  delete fullData;
}
