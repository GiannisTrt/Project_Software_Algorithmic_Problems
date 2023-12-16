#ifndef _ASSIGN_
#define _ASSIGN_

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "list.h"
#include "bucket.h"
#include "hash.h"
#include "metrics.h"

// Synarthsh assign lloyd
int ASSIGN_lloyd(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int), int* cluster_center_of_vector, double* distance_from_cluster_center, int* cluster_centers, int K, int d,list** clusters);

// Synarthsh assign reverse R me lsh
int ASSIGN_reverse_R_lsh(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int),void*** lsh_hashtables, int* cluster_center_of_vector, double* distance_from_cluster_center, int* cluster_centers, int** buckets_of_current_centers,double*** g,int* m_powers, int L, int K,int d,double lsh_w, int M,int LSH_k,int buckets,list** clusters);

// Synarthsh assign reverse R me hypercube
int ASSIGN_reverse_R_hypercube(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int),void** hypercube, int** cube_hashtables, int* cluster_center_of_vector, double* distance_from_cluster_center, int* cluster_centers, int* vertices_of_current_centers,int HYPERCUBE_probes,int** probes_array,double*** g,int* m_powers,int K,int d,double cube_w,int M, int HYPERCUBE_k,int cube_num_of_hi_functions,int buckets,list** clusters);

#endif