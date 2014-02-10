#include"ptree.h"

int SIZE = 'z' - '0' + 1;

int ptree::getIndex(char c) {
	int result = c - '0';
	return result;
}

ptree::ptree() {
	cap = 1;
	size = 1;
	table = new int*[cap];
	table[0] = new int[SIZE];
	for(int i=0; i<SIZE; i++)
		table[0][i] = -1;
	values = new int[cap];
	values[0] = -1;
}

ptree::~ptree() {
	for(int i=0; i<size; i++)
		delete table[i];
	delete table;
	delete values;
}

void ptree::expand() {
	int new_cap = cap << 1;
	int** new_table = new int*[new_cap];
	int* new_values = new int[new_cap];
	for(int i=0; i<cap; i++) {
		new_table[i] = table[i];
		new_values[i] = values[i];
	}
	for(int i=cap; i<new_cap; i++) {
		new_table[i] = 0;
		new_values[i] = -1;
	}

	delete table;
	delete values;

	table = new_table;
	values = new_values;
	cap = new_cap;
}

int ptree::get(const char* str) {
	const char* s = str;
	int state = 0;
	while(*s != '\0' && state >= 0) {
		int c = getIndex(*s);
		state = table[state][c];
		s++;
	}

	if(state == -1)
		return -1;

	return values[state];
}

void ptree::add(const char* str, int i) {
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
}

int ptree::buildState() {
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
