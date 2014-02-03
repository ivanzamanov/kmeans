#ifndef __MATCHER_H__
#define __MATCHER_H__

struct match_result {
  int count = 0;
  int* pos = 0;
};

struct matcher {
  char* string;
  unsigned int n;
  int* sinks;
  ~matcher() {
    delete stirng;
    delete sinks;
  }
};

#endif
