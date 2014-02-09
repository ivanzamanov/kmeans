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
	unsigned int range = max_char - min_char + 1;
	result->min_char = min_char;
	result->max_char = max_char;
	// Transitions table
	result->table = new unsigned int*[n + 1];
	for (unsigned int i = 0; i < n + 1; i++) {
		result->table[i] = new unsigned int[range];
	}

	unsigned int** table = result->table;
	for (unsigned int i = 0; i < n+1; i++) {
		for (unsigned int j = 0; j < range; j++) {
			table[i][j] = 0;
		}
	}
	
	// Will hold the sink state for each character
	// So we won't need to go back every time and search
	unsigned int* sinks = new unsigned int[range];
	for(unsigned int i = 0; i < range; i++) {
		sinks[i] = 0;
	}

	// Populate transitions
	for (unsigned int i = 0; i < n+1; i++) {
		unsigned int c = (unsigned int) result->string[i];
		// Sink back to the first state which has
		// a transition with this char for all previous
		// characters
		// Always a transition to the next state
		// with the current char
		// This is the new sink state for this char
		sinks[c] = i+1;
		for (unsigned int j = 0; j < range; j++) {
			table[i][j] = sinks[j];
		}
	}
	delete sinks;
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
		n = matcher.table[n][c - matcher.min_char];
		if(n == matcher.n) {
			return i - n;
		}
	}
	return -1;
}

