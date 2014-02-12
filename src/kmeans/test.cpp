#include<cstdio>

#include"kmeans.h"


int main() {
	vector v1;
	vector v2, v3;

	v1.keys = new int[2];
	v1.values = new double[2];
	v1.keys[0] = 2; v1.keys[1] = 3;
	v1.values[0] = 0; v1.values[1] = 0;
	v1.size = 2;

	v2.keys = new int[2];
	v2.values = new double[2];
	v2.keys[0] = 1; v2.keys[1] = 5;
	v2.values[0] = 1; v2.values[1] = 1;
	v2.size = 2;

	v3.keys = new int[2];
	v3.values = new double[2];
	v3.keys[0] = 1; v3.keys[1] = 3;
	v3.values[0] = 1; v3.values[1] = 1;
	v3.size = 2;

	printf("Scalar %f\n", scalar(v1, v2));
	printf("DistanceSQ %f\n", distanceSQ(v1, v2));

	list<vector*> vs;
	vs.add(&v1);
	vs.add(&v2);
	vs.add(&v3);

	vector c;
	computeCentroid(c, vs);

	printf("Centroid: ");
	for(int i=0; i<c.size; i++)
		printf("%f ", c.values[i]);
	printf("\n");
}

