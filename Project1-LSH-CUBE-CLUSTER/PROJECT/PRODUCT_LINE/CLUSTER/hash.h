#ifndef _HASH_
#define _HASH_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bucket.h"
#include "vector.h"
#include "metrics.h"

// Synarthsh poy dhmioyrgei kai epistrefei tis synarthseis katakermatismoy g_i se morfh pinaka g
double*** HASH_create_hash_functions(int L, int k, int d, double w);

// Apeley8erwsh mnhmhs toy pinaka twn synarthsewn katakermatismoy
void HASH_free_hash_functions(double*** g,int L, int k);

// Synarthsh emfanishs synarthsewn katakermatismoy (boh8htikh gia logoys elegxoy)
void HASH_print_hash_functions(double*** g,int L, int k, int d);

// Dhmioyrgia dynamewn toy m
int* HASH_create_m_powers(int d,unsigned int m,int M);

// H synarthsh epistrefei se morfh akeraioy ta k deksiotera pshfia toy katakermatismoy 
// toy dianysmatos vector me th synarthsh katakermatismoy h_i
int HASH_h_i(double* h_i,int* vector,int* m_powers,double w, int d, int M);

// H synarthsh epistrefei ton ari8mo kadoy pou mpainei ena dianysma me bash thn 
// amplified synarthsh katakermatismoy g_i
int HASH_g_i(double** g_i,int* vector,int* m_powers,double w, int d, int M, int k,int buckets);

#endif
