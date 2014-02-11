#ifndef __PTREE_H__
#define __PTREE_H__

int SIZE = 'z' - '0' + 1;

template<class T>
class ptree {
public:
	ptree();
	~ptree();
	void add(const char* str, T i);
	T* get(const char* str);

private:
	int getIndex(char c);
	void expand();
	int buildState();
	int** table;
	T* values;
	bool* flags;
	int cap;
	int size;
};

template<class T>
int ptree<T>::getIndex(char c) {
	int result = c - '0';
	return result;
}

template<class T>
ptree<T>::ptree() {
	cap = 1;
	size = 1;
	table = new int*[cap];
	table[0] = new int[SIZE];
	for(int i=0; i<SIZE; i++)
		table[0][i] = -1;
	values = new T[cap];
	values[0] = -1;
	flags = new bool[cap];
	flags[0] = 0;
}

template<class T>
ptree<T>::~ptree() {
	for(int i=0; i<size; i++)
		delete table[i];
	delete table;
	delete values;
	delete flags;
}

template<class T>
void ptree<T>::expand() {
	int new_cap = cap << 1;
	int** new_table = new int*[new_cap];
	T* new_values = new T[new_cap];
	bool* new_flags = new bool[new_cap];
	for(int i=0; i<cap; i++) {
		new_table[i] = table[i];
		new_values[i] = values[i];
		new_flags[i] = flags[i];
	}
	for(int i=cap; i<new_cap; i++) {
		new_table[i] = 0;
		new_flags[i] = 0;
	}

	delete table;
	delete values;
	delete flags;

	table = new_table;
	values = new_values;
	flags = new_flags;
	cap = new_cap;
}

template<class T>
T* ptree<T>::get(const char* str) {
	const char* s = str;
	int state = 0;
	while(*s != '\0' && state >= 0) {
		int c = getIndex(*s);
		state = table[state][c];
		s++;
	}

	if(state == -1 || !flags[state])
		return 0;

	return values + state;
}

template<class T>
void ptree<T>::add(const char* str, T i) {
	const char* s = str;
	int state = 0;
	while(*s != '\0') {
		int c = getIndex(*s);
		int n_state = table[state][c];
		if(n_state == -1) {
			break;
		}
		state = n_state;
		s++;
	}

	while(*s != '\0') {
		int c = getIndex(*s);
		int n_state = buildState();
		table[state][c] = n_state;
		state = n_state;
		s++;
	}
	values[state] = i;
	flags[state] = 1;
}

template<class T>
int ptree<T>::buildState() {
	if(size == cap)
		expand();
	if(size == 1423)
		size = size;

	int result = size;
	size++;
	table[result] = new int[SIZE];
	for(int i=0; i<SIZE; i++)
		table[result][i] = -1;
	return result;
}

#endif
