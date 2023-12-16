#ifndef _HASH_
#define _HASH_

#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <ctime>
#include <cmath>

#include "bucket.h"
#include "vector.h"
#include "metrics.h"

// Synarthsh poy dhmioyrgei kai epistrefei tis synarthseis katakermatismoy g_i se morfh pinaka g
double*** HASH_create_hash_functions(int L, int k, int d, double w);

// Apeley8erwsh mnhmhs toy pinaka twn synarthsewn katakermatismoy
void HASH_free_hash_functions(double*** g,int L, int k);

// Apeley8erwsh mnhmhs toy pinaka ths synarthshs katakermatismoy (Gia hypercube)
void HASH_free_hash_function(double** h,int k);

// Synarthsh poy dhmioyrgei kai epistrefei tih synarthseis katakermatismoy h se morfh pinaka (Gia hypercube)
double** HASH_create_hash_function(int k, int d, double w);

// Synarthsh emfanishs synarthsewn katakermatismoy (boh8htikh gia logoys elegxoy)
void HASH_print_hash_functions(double*** g,int L, int k, int d);

// Dhmioyrgia dynamewn toy m
int* HASH_create_r_values(int d);

// Synartyhsh h_i twn algori8mwn
int HASH_h_i(double* h_i,double* vector,double w, int d);

// H synarthsh epistrefei ton ari8mo kadoy pou mpainei ena dianysma me bash thn 
// amplified synarthsh katakermatismoy g_i
int HASH_g_i(double** g_i,double* vector,int* r_values,double w, int d, int M, int k,int buckets);

#endif
