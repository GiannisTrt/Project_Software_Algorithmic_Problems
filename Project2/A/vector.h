#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef _VECTOR_
#define _VECTOR_
	
// Diavasma dianysmatwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn dianysmatwn kai h diastash
int VECTOR_read_vectors_from_file(char* file,double*** vectors,char*** vector_names,int* num_of_vectors,int* d);

// Synarthsh emfanishs dianysmatwn (boh8htikh gia logoys elegxoy)
void VECTOR_print_vectors(double** vectors,int num_of_vectors,int d);

// Synarthsh emfanishs dianysmatos (boh8htikh gia logoys elegxoy)
void VECTOR_print_vector(double* vector,int d);

// Synarthsh emfanishs dianysmatwn (boh8htikh gia logoys elegxoy)
void VECTOR_print_vectors_and_names(double** vectors,char** vector_names,int num_of_vectors,int d);

// Apeley8erwsh mnhmhs toy pinaka twn dianysmatwn
void VECTOR_free_vectors(double** vectors,int num_of_vectors,int d);

#endif