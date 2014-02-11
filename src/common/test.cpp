#include"ptree.h"

#include<cstdio>
#include<stdio.h>

int main() {
	ptree<int> p;
	const int n = 2;
	const char* strs[n] = {"amer", "a"};

//	for(int i=0; i<n; i++)
		p.add(strs[0], 5);

	printf("%d\n", *p.get("a"));

	for(int i=0; i<n; i++)
		printf("%d\n", *p.get(strs[i]));
}
