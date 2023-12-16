#ifndef _CLUSTER_ARGS_
#define _CLUSTER_ARGS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

// Synarthsh diavasmatos orismatwn 
int CLUSTER_ARGS_get_cluster_args(int argc, char* argv[],char** input_file,char** configuration_file,char** output_file,int* assignment,int* update,int* complete,int* silhouette);


// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void CLUSTER_ARGS_print_cluster_args(char* input_file,char* configuration_file,char* output_file,int assignment,int update,int complete,int silhouette);

// Synarthsh diavasmatos parametrwn apo to arxeio configuration 
int CLUSTER_ARGS_get_cluster_parameters(char* configuration_file,int default_K,int default_L,int default_LSH_k,int default_HYPERCUBE_M,int default_HYPERCUBE_k,int default_HYPERCUBE_probes,int* K,int *L,int *LSH_k,int *HYPERCUBE_M,int *HYPERCUBE_k,int *HYPERCUBE_probes,int* lsh_divider,unsigned int* lsh_m,double *lsh_w,unsigned int* cube_m, int* cube_divider,double* cube_w,int* max_iterations);

// Synarthsh emfanishs parametrwn (boh8htikh gia logoys elegxoy) 
void CLUSTER_ARGS_print_cluster_parameters(int K,int L,int LSH_k,int HYPERCUBE_M,int HYPERCUBE_k,int HYPERCUBE_probes,int max_iterations);

#endif