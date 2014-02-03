#include "matcher.h"

bool build_matcher(const char* string, matcher* &result) {
  result = new matcher();
  unsigned int n = 0;
  while(*(string + n) != 0) {
    i++;
  }
  result->n = n;
  result->string = new char[n];
  result->sinks = new int[n+1];
  int* sinks = result->sinks;
  sinks[0] = 0;
  result->string = new char[n];
  for(unsigned int k = 0; k < n; k++) {
    result->string[k] = string[k];
  }

  
}
