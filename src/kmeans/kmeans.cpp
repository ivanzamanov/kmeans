#include<cstdio>
#include<math.h>

#include"kmeans.h"
#include"list.h"

typedef list<article_entry*> entryList;

struct cluster {
	vector centroid;
	entryList members;
};

double scalar(const vector& v1, const vector& v2) {
	double result = 0;
	int i1 = 0, i2 = 0;
	while(i1 < v1.size && i2 < v2.size) {
		if(v1.keys[i1] == v2.keys[i2]) {
			result += v1.values[i1] * v2.values[i2];
			i1++;
			i2++;
		} else if (v1.keys[i1] < v2.keys[i2]) {
			i1++;
		} else {
			i2++;
		}
	}
	return result;
}

double distanceSQ(const vector& v1, const vector& v2) {
	double result = 0;
	int i1 = 0, i2 = 0;
	while(i1 < v1.size && i2 < v2.size) {
		double diff;
		if(v1.keys[i1] == v2.keys[i2]) {
			diff = v1.values[i1] - v2.values[i2];
			i1++;
			i2++;
		} else if (v1.keys[i1] < v2.keys[i2]) {
			diff = v1.values[i1];
			i1++;
		} else {
			diff = v2.values[i2];
			i2++;
		}
		result += diff * diff;
	}
	return result;
}

void recomputeCentroid(cluster& cl) {
	
}

void selectSeeds(cluster* clusters, entryList &l, int k) {
	int index = 0;
	for(int i=0; i<l.size(); i++) {
		index = i % k;
		cluster &cl = clusters[index];
		article_entry* e = l.get(i);
		cl.members.add(e);
	}
	for(int i=0; i<l.size(); i++)
		recomputeCentroid(clusters[i]);
}

cluster& findBestCluster(cluster* clusters, int k, vector& v) {
	int best_cl = 0;
	double best_dist = 1 << 31;
	for(int i=0; i<k; i++) {
		double dist = distanceSQ(v, clusters[i].centroid);
		if(dist < best_dist) {
			best_dist = dist;
			best_cl = i;
		}
	}
	return clusters[best_cl];
}

const int MAX_ITERATIONS = 10;
void clusterize(int k, entryList &l) {
	cluster *clusters = new cluster[k];
	selectSeeds(clusters, l, k);

	int iterations = 0;
	while(iterations < MAX_ITERATIONS) {
		// Clear cluster members.
		for(int i=0; i<k; i++)
			clusters[i].members.clear();

		// Find the best cluster for each entry
		// and add as a member
		for(int i=0; i<l.size(); i++) {
			article_entry* entry = l.get(i);
			cluster& cl = findBestCluster(clusters, k, entry->v);
			cl.members.add(entry);
		}

		// Recompute centroids
		for(int i=0; i<k; i++)
			recomputeCentroid(clusters[i]);

		iterations++;
	}

	delete[] clusters;
}

void normalize(article_entry* entry) {
	double factor = 0;
	for(int i=0; i<entry->v.size; i++) {
		double d = entry->v.values[i];
		factor += d * d;
	}
	factor = sqrt(factor);
	for (int i=0; i<entry->v.size; i++) {
		entry->v.values[i] = entry->v.values[i] / factor;
	}
}

void kmeans(list<article_entry*> &l) {
	// Normalize vectors
	for(int i=0; i<l.size(); i++)
		normalize(l.get(i));
	int k = 10;

	clusterize(k, l);
}
