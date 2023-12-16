#ifndef _SEARCH_ARGS_
#define _SEARCH_ARGS_

#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "constants.h"

// Synarthsh diavasmatos orismatwn 
int ARGS_get_search_args(int argc, char* argv[],char** input_file,char** query_file,char** output_file,int* arlhorithm,int* k,int* L,int* M,int* probes,int* metric,double* delta,int default_lsh_k,int default_hypercube_k,int default_L,int default_M,int default_probes,int default_metric);

// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void SEARCH_ARGS_print_search_args(char* input_file,char* query_file,char* output_file,int arlhorithm,int k,int L,int M,int probes,int metric,double delta);

















// Synarthsh diavasmatos parametrwn apo to arxeio configuration 
int SEARCH_ARGS_get_search_parameters(char* configuration_file,int default_K,int default_L,int default_LSH_k,int default_HYPERCUBE_M,int default_HYPERCUBE_k,int default_HYPERCUBE_probes,int* K,int *L,int *LSH_k,int *HYPERCUBE_M,int *HYPERCUBE_k,int *HYPERCUBE_probes,int* lsh_divider,unsigned int* lsh_m,double *lsh_w,unsigned int* cube_m, int* cube_divider,double* cube_w,int* max_iterations);

// Synarthsh emfanishs parametrwn (boh8htikh gia logoys elegxoy) 
void SEARCH_ARGS_print_search_parameters(int K,int L,int LSH_k,int HYPERCUBE_M,int HYPERCUBE_k,int HYPERCUBE_probes,int max_iterations);

#endif