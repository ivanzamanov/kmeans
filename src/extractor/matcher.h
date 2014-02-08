#ifndef __MATCHER_H__
#define __MATCHER_H__

struct matcher {
	char* string;
	unsigned int n;
	char min_char;
	char max_char;
	unsigned int** table;
	~matcher() {
		delete string;
		for (unsigned int i = 0; i < n+1; i++) {
			delete table[i];
		}
		delete table;
	}
};

bool build_matcher(const char* string, matcher* &result);
int search(char const* string, const matcher& matcher);

#endif
