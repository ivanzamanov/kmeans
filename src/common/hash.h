#include "utils.h"

#ifndef __HASH_H__
#define __HASH_H__

template<class T>
class hash {
public:
  hash();
  hash(const hash<T>& other);

  int cap;
  int size;

  void insert(const int& key, const T& data);
  const T& get(const int& key, const T& def);

  void expand();

  T* data;
  int* keys;
};

template<class T>
hash<T>::hash() {
  hash<T>& h = *this;
  h.cap = 2;
  h.size = 0;
  h.keys = new int[cap];
  for (int i=0; i<cap; i++) {
    h.keys[i] = -1;
  }
  h.data = new T[cap];
}

template<class T>
hash<T>::hash(const hash<T>& other) {
  hash<T>& h = *this;
  h.cap = other.cap;
  h.size = other.size;
  h.keys = new int[cap];
  h.data = new T[cap];
  for (int i=0; i<size; i++) {
    h.keys[i] = other.keys[i];
    h.data[i] = other.data[i];
  }
}

template<class T>
void hash<T>::insert(const int& key, const T& el) {
  int index = bin_search(keys, size, key);
  if(index >= 0) {
    this->data[index] = el;
  } else {
    if(size >= cap)
      expand();
    index = -index - 1;
    for (int i = size; i > index; i--) {
      keys[i] = keys[i-1];
      data[i] = data[i-1];
    }
    keys[index] = key;
    data[index] = el;
    size++;
  }
}

template<class T>
const T& hash<T>::get(const int& key, const T& def) {
  int index = bin_search(keys, size, key);
  if(index >= 0)
    return data[index];
  
  return def;
}

template<class T>
void hash<T>::expand() {
  hash<T>& table = *this;
  int new_c = table.cap + 1;
  expand_array(table.keys, table.cap, new_c);
  expand_array(table.data, table.cap, new_c);
  table.cap = new_c;
}

#endif
