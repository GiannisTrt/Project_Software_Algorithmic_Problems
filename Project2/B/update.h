#ifndef _UPDATE_
#define _UPDATE_

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "list.h"
#include "bucket.h"
#include "metrics.h"


// Enhmerwsh toy kentroy olwn twn clusters
int UPDATE_center(double** vectors, int num_of_vectors, double (*metric)(double*,double*,int), int* cluster_centers,int K, int d,list** clusters);


#endif




