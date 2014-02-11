#include<cstdio>
#include<math.h>

#include"kmeans.h"
#include"utils.h"

void expandVector(vector& v) {
	int* new_keys = new int[v.size+1];
	double* new_values = new double[v.size+1];
	for(int i=0; i<v.size; i++) {
		new_keys[i] = v.keys[i];
		new_values[i] = v.values[i];
	}

	if(v.keys != 0)
		delete v.keys;
	if(v.values != 0)
		delete v.values;

	v.size++;
	v.keys = new_keys;
	v.values = new_values;
}

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

void clone(vector& dest, vector& src) {
	if(dest.keys != 0)
		delete dest.keys;
	if(dest.values != 0)
		delete dest.values;
	dest.size = src.size;
	dest.keys = new int[dest.size];
	dest.values = new double[dest.size];
	for(int i=0; i<dest.size; i++) {
		dest.keys[i] = src.keys[i];
		dest.values[i] = src.values[i];
	}
}

void computeCentroid(vector& centroid, const list<vector*> &vectors) {
	if(vectors.size() == 0)
		return;
	vector& first = *vectors.get(0);
	clone(centroid, first);

	for(int i=1; i<vectors.size(); i++) {
		vector& member = *(vectors.get(i));
		for(int j=0; j<member.size; j++) {
			int key = member.keys[j];
			double value = member.values[j];
	
			if(centroid.size ==0) {
				expandVector(centroid);
				centroid.keys[0] = key;
				centroid.values[0] = value;
				continue;
			}
	
		  int index = bin_search(centroid.keys, centroid.size, key);
		  if(index >= 0) {
		    centroid.values[index] += value;
		  } else {
		    expandVector(centroid);
		    index = -index - 1;
		    for (int k = centroid.size-1; k > index; k--) {
		      centroid.keys[k] = centroid.keys[k-1];
		      centroid.values[k] = centroid.values[k-1];
		    }
		    centroid.keys[index] = key;
		    centroid.values[index] = value;
		  }
		}
	}

	for(int i=0; i<centroid.size; i++)
		centroid.values[i] /= vectors.size();
}

void recomputeCentroid(cluster& cl) {
	vector &centroid = cl.centroid;
	list<vector*> vectors;
	for(int i=0; i<cl.members.size();i++)
		vectors.add(&(cl.members.get(i)->v));

	centroid.size = 0;
	computeCentroid(centroid, vectors);
}

void selectSeeds(cluster* clusters, entryList &l, int k) {
	int index = 0;
	for(int i=0; i<l.size(); i++) {
		index = i % k;
		cluster &cl = clusters[index];
		article_entry* e = l.get(i);
		cl.members.add(e);
	}
	for(int i=0; i<k; i++)
		recomputeCentroid(clusters[i]);
}

cluster& findBestCluster(cluster* clusters, int k, vector& v) {
	int best_cl = 0;
	double best_dist = INFINITY;
	for(int i=0; i<k; i++) {
		double dist = distanceSQ(v, clusters[i].centroid);
		if(dist < best_dist) {
			best_dist = dist;
			best_cl = i;
		}
	}
	return clusters[best_cl];
}

double calculateRSS(cluster& c) {
	double result = 0;
	for (int i=0; i<c.members.size(); i++) {
		result += distanceSQ(c.centroid, c.members.get(i)->v);
	}
	return result;
}

double printRSS(cluster* clusters, int n) {
	double result = 0;
	for(int i=0; i<n; i++) {
		double rss = calculateRSS(clusters[i]);
		printf("Cluster %d: RSS = %f Members = %d\n", i, rss, clusters[i].members.size());
		result += rss;
	}
	return result;
}

const int MAX_ITERATIONS = 10;
const double THRESHOLD = 1e-20;
void clusterize(int k, entryList &l) {
	cluster *clusters = new cluster[k];
	selectSeeds(clusters, l, k);
	double RSS = printRSS(clusters, k);
	double prevRSS = 0;
	printf("Total RSS = %f\n", RSS);

	int iterations = 0;
	while(iterations < MAX_ITERATIONS && (RSS - prevRSS) < THRESHOLD) {
		iterations++;
		printf("Iteration %d\n", iterations);
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

		prevRSS = RSS;
		RSS = printRSS(clusters, k);
		printf("Total RSS = %f\n", RSS);
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
