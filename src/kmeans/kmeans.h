#include"list.h"

#ifndef __KMEANS__H__
#define __KMEANS__H__

struct vector {
	int size = 0;
	int* keys = 0;
	double* values = 0;
};

struct article_entry {
	char* name;
// Contain the tf's.
	vector v;
};

struct iter {
	int size;
	int index = 0;
};

typedef list<article_entry*> entryList;

struct cluster {
	vector centroid;
	entryList members;
};

void kmeans(list<article_entry*> &l);
void computeCentroid(vector& centroid, const list<vector*>& vectors);
double scalar(const vector& v1, const vector& v2);
void expandVector(vector& v);
void clone(vector& dest, vector& src);
double distanceSQ(const vector& v1, const vector& v2);

#endif
