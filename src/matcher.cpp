#include "matcher.h"

bool build_matcher(const char* string, matcher* &result) {
  // The result
  result = new matcher();
  unsigned int n = 0;
  // strlen
  while(*(string + n) != 0) {
    n++;
  }
  // Save the string and it's lenght
  // Not sure why...
  result->n = n;
  result->string = new char[n + 1];
  result->string[n] = 0;

  // While copying the string, find and save
  // the min and max chars to reduce
  // memory footprint
  char min_char = 0;
  char max_char = 0;
  for(unsigned int k = 0; k < n; k++) {
    char c = string[k];
    result->string[k] = c;
    if(min_char > c) {
      min_char = c;
    }
    if(max_char < c) {
      max_char = c;
    }
  }
  unsigned int range = max_char - min_char;
  result->min_char = min_char;
  result->max_char = max_char;
  // Transitions table
  result->table = new int*[n + 1];
  for (unsigned int i = 0; i < n + 1; i++) {
    result->table[i] = new int[range+1];
  }

  int** table = result->table;
  for (unsigned int i = 0; i < n+1; i++) {
    for (unsigned int j = 0; j < range + 1; j++) {
      table[i][j] = 0;
    }
  }
  
  // Populate transitions
  for (unsigned int i = 0; i < n+1; i++) {
    // Always a transition to the next state
    // with the current char
    int c = (int) result->string[i];
    table[i][c] = i+1;
    // Sink back to the first state which has
    // a transition with this char for all previous
    // characters
    for (int k = i-1; k >= 0; k--) {
      int sink_char = (int) result->string[k];
      int target = table[i][sink_char];
      if(target == -1) {
        table[i][sink_char] = k;
      }
    }
  }
  return 1;
}

int search(char const* string, const matcher& matcher) {
  unsigned int i = 0;
  unsigned int n = 0;
  while(*string != 0) {
    i++;
    int c = (int)*string;
    string++;
    if(c < matcher.min_char || c > matcher.max_char) {
      n = 0;
      continue;
    }
    n = matcher.table[n][c];
    if(n == matcher.n) {
      return i - n;
    }
  }
  return -1;
}

