#ifndef __ARR_LIST_H
#define __ARR_LIST_H

template<class T>
class list {
public:
	list() {
		s = 0;
		cap = 2;
		arr = new T[cap];
	};

	list(const list<T>& other) {
		s = other.s;
		cap = other.cap;
		arr = new T[cap];
		for(int i=0; i<s; i++)
			arr[i] = other.arr[i];
	};

	~list() {
		delete arr;
	};

	void add(const T& el) {
		if(s == cap) {
			int new_c = cap << 1;
			T* new_arr = new T[new_c];
			for (int i=0; i<cap; i++)
				new_arr[i] = arr[i];
			delete arr;
			arr = new_arr;
			cap = new_c;
		}
		arr[s] = el;
		s++;
	};

	T get(int i) const {
		return arr[i];
	};

	void clear() {
		s = 0;
		cap = 2;
		delete arr;
		arr = new T[cap];
	};

	int size() const {
		return s;
	};

private:
	int s;
	int cap;
	T* arr;
};

#endif
