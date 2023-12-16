#include <stdio.h>
#include <stdlib.h>

#ifndef _VECTOR_
#define _VECTOR_
	
// Diavasma dianysmatwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn dianysmatwn kai h diastash
int VECTOR_read_vectors_from_file(char* file,int*** vectors,int* num_of_vectors,int* d);

// Synarthsh emfanishs dianysmatwn (boh8htikh gia logoys elegxoy)
void VECTOR_print_vectors(int** vectors,int num_of_vectors,int d);

// Synarthsh emfanishs dianysmatos (boh8htikh gia logoys elegxoy)
void VECTOR_print_vector(int* vector,int d);

// Apeley8erwsh mnhmhs toy pinaka twn dianysmatwn
void VECTOR_free_vectors(int** vectors,int num_of_vectors,int d);

#endif