#ifndef _INIT_
#define _INIT_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "metrics.h"

// Arxikopoihsh ths systadopoihshs me kmeans++
void INIT_k_means(double** vectors, int num_of_vectors, double (*metric)(double*,double*,int), int* cluster_centers, int K, int d);

#endif