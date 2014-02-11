#ifndef __TOK_UTILS_H__
#define __TOK_UTILS_H__

bool readToken(const char* data, int& offset, char* buffer, int buffLength);
void lowercaseToken(char* token);
void writeToken(const char* buffer, int outputFD);
int readFullData(int fd, char** data);

template<class T>
inline void expand_array(T*& array, const int n, const int new_n) {
  T* new_array = new T[new_n];
  for (int i=0; i<n; i++) {
    new_array[i] = array[i];
  }
  delete array;
  array = new_array;
}

template<class T>
inline void expand_array(T*& array, const int length, const int new_n, T def) {
  T* new_array = new T[new_n];
  for (int i=0; i<length; i++) {
    new_array[i] = array[i];
  }
  for (int i=length; i<new_n; i++) {
    new_array[i] = def;
  }
  delete array;
  array = new_array;
}

template<class T>
inline void fill_array(T* array, const int length, const T& value) {
  for (int i = 0; i < length; i++) {
    array[i] = value;
  }
}

template<class T>
inline void copy_array(T* from, T* to, const int length) {
  for (int i = 0; i < length; i++) {
    to[i] = from[i];
  }
}

template<class T>
inline int bin_search(T* array, int n, const T& x) {
	if(n < 1)
		return -1;
  int left = 0;
  int right = n;
  int current;
  while (left < right) {
    current = (left + right) / 2;
    if(array[current] == x) {
      return current;
    } else if (array[current] > x) {
      right = current;
    } else if (array[current] < x) {
      left = current + 1;
		}
	}
	if(right == 0)
		return -1;
	return -right;
}

#endif
