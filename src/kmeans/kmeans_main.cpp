#include<stdio.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<math.h>

#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>

#include"ptree.h"
#include"hash.h"
#include"list.h"
#include"kmeans.h"

void save(list<article_entry*> &entries, hash<double> &h, const char* name) {
	article_entry* entry = new article_entry;
	entries.add(entry);
	int size = h.size;
	entry->v.size = size;
	int* keys = new int[size];
	double* values = new double[size];
	for(int i=0; i<size; i++) {
		keys[i] = h.keys[i];
		values[i] = h.data[i];
	}
	int nameLength = strlen(name);
	char* entry_name = new char[nameLength + 1];
	for(int i=0; i<nameLength; i++)
		entry_name[i] = name[i];
	entry_name[nameLength] = 0;
	entry->name = entry_name;
	entry->v.keys = keys;
	entry->v.values = values;
}

int tokenCount = 0;
void processFile(const char* path, const char* name, ptree<int>& p, list<article_entry*> &entries) {
	if(*name == '.')
		return;
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		printf("Cannot open file %s\nCode = %d\n", path, fd);
		return;
	}
	char* fullData = 0;
	readFullData(fd, &fullData);
	close(fd);

	char buffer[4096];
	int offset = 0;
	
	hash<double> h;
	int* index_ptr = 0;
	int index = 0;
	while(readToken(fullData, offset, buffer, 4096)) {
		index_ptr = p.get(buffer);
		if(index_ptr == 0) {
			p.add(buffer, tokenCount);
			index = tokenCount;
			tokenCount++;
		} else {
			index = *index_ptr;
		}

		int tf = h.get(index, 0);
		tf++;
		h.insert(index, tf);
	}

	save(entries, h, name);

	delete fullData;
}

void processDirectory(const char* inputDirPath) {
	DIR* dirHandle = opendir(inputDirPath);
	struct dirent* entry = readdir(dirHandle);

	list<article_entry*> entries;
	char path[4096];
	ptree<int> p;
	double docCount = 0;
	while(entry != NULL) {
		sprintf(path, "%s/%s", inputDirPath, entry->d_name);
		processFile(path, entry->d_name, p, entries);
		entry = readdir(dirHandle);
		docCount++;
	}
	printf("Unique tokens: %d\n", tokenCount);
	// Document frequencies
	double* dfs = new double[tokenCount];
	for (int i=0; i<tokenCount; i++)
		dfs[i] = 0;
	for(int i=0; i<entries.size(); i++) {
		vector& v = entries.get(i)->v;
		for(int j=0; j<v.size; j++)
			dfs[ v.keys[j] ]++;
	}
	for(int i=0; i<entries.size(); i++) {
		vector& v = entries.get(i)->v;
		for(int j=0; j<v.size; j++) {
			int tf = v.values[j];
			double idf = log( docCount / dfs[ v.keys[j] ]);
			v.values[j] = ((double) tf) * idf;
		}
	}
	delete dfs;
	
	kmeans(entries);
	
	closedir(dirHandle);
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Not enough arguments\n");
		printf("Usage: %s <input_dir> <output_file>", argv[0]);
		return 1;
	}

	const char* inputPath = argv[1];
	processDirectory(inputPath);
}

