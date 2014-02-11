#ifndef __KMEANS__H__
#define __KMEANS__H__

#include"list.h"

struct vector {
	int size;
	int* keys;
	int* values;
};

struct article_entry {
	char* name;
	vector v;
};

void kmeans(list<article_entry*> &l);

#endif
