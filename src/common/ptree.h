#ifndef __PTREE_H__
#define __PTREE_H__

class ptree {
public:
	ptree();
	~ptree();
	void add(const char* str, int i);
	int get(const char* str);

private:
	int getIndex(char c);
	void expand();
	int buildState();
	int** table;
	int* values;
	int cap;
	int size;
};

#endif
