#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef _CURVE_
#define _CURVE_
	
// Diavasma kampylwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn kampylwn kai h diastash
int CURVE_read_discrete_frechet_curves_from_file(char* file,double*** curves,char*** curve_names,int* num_of_curves,int* d);

// Diavasma kampylwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn kampylwn kai h diastash
int CURVE_read_continuous_frechet_curves_from_file(char* file,double*** curves,char*** curve_names,int* num_of_curves,int* d);


// Synarthsh emfanishs kampylwn (boh8htikh gia logoys elegxoy)
void CURVE_print_discrete_frechet_curves(double** curves,int num_of_curves,int d);

// Synarthsh emfanishs dianysmatos (boh8htikh gia logoys elegxoy)
void CURVE_print_vector(double* vector,int d);

// Synarthsh emfanishs kampylwn (boh8htikh gia logoys elegxoy)
void CURVE_print_curves_and_names(double** curves,char** curve_names,int num_of_curves,int d);

// Apeley8erwsh mnhmhs toy pinaka twn kampylwn
void CURVE_free_discrete_frechet_curves(double** curves,int num_of_curves,int d);

// Apeley8erwsh mnhmhs toy pinaka twn kampylwn
void CURVE_free_continuous_frechet_curves(double** curves,int num_of_curves,int d);

#endif