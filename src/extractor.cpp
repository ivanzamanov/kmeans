#include"matcher.h"

#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>

struct article_match {
  int start;
  int end;
  article_match(int start, int end) {
    this->start = start;
    this->end = end;
  }
};

void processFile(int inputFileFD, struct stat* const fileStat);
article_match* get_match(const char* data, const matcher& startMarker, const matcher& endMarker);
void save_article(article_match* match, const char* data, int article_num);

int main(int argc, char** argv) {
  if(argc < 2) {
    printf("Not enough arguments\n");
    return 1;
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

  char* outputDir = argv[2];
  
  processFile(inputFD, &fileStat);
}

void processFile(int inputFD, struct stat* const fileStat) {
  // Read the whole file in memory
  char* fullData = new char[fileStat->st_size];
  int bytes = read(inputFD, fullData, fileStat->st_size);
  fullData[fileStat->st_size] = 0;

  if (bytes != fileStat->st_size) {
    printf("Could not read entire file\n");
  } else {
    matcher* startMarker;
    matcher* endMarker;
    build_matcher("<BODY>", startMarker);
    build_matcher("</BODY>", endMarker);
    article_match* lastArticleMatch = 0;
    int lastArticleEndPos = 0;
    int articleCount = 0;

    // Read articles while found
    do {
      char* offsetData = fullData + lastArticleEndPos;
      // Try to find an article
      lastArticleMatch = get_match(offsetData, *startMarker, *endMarker);
      if(lastArticleMatch != 0) {
        // Found an article, save it
        articleCount++;
        save_article(lastArticleMatch, offsetData, articleCount);
        // Move offset after the article's end.
        lastArticleEndPos = lastArticleEndPos + lastArticleMatch->end;
        delete lastArticleMatch;
      }
    } while(lastArticleMatch != 0);
  }
  delete fullData;
}

article_match* get_match(const char* data, const matcher& startMarker, const matcher& endMarker) {
  // Search for the start marker
  int start = search(data, startMarker);
  int end = -1;
  if(start >= 0) {
      // Search for the end marker after the start
      end = search(data + start, endMarker);
      end = end + start;
      if(end > start) {
        // Found, return a reference
        article_match* result = new article_match(start + startMarker.n, end);
        return result;
      }
  }
  return 0;
}

void save_article(article_match* match, const char* data, int article_num) {
  int startPos = match->start;
  int endPos = match->end;
  int length = endPos - startPos;
  char* articleData = new char[length + 1];
  articleData[length] = 0;
  for (int i = 0; i < length; i++) {
    articleData[i] = data[startPos + i];
  }
  
  delete articleData;
  printf("%s\n\n\n", articleData);
}