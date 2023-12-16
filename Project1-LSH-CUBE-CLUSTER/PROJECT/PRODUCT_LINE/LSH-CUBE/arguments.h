#ifndef _ARGUMENTS_
#define _ARGUMENTS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Synarthsh diavasmatos orismatwn 
int ARGS_get_lsh_args(int argc, char* argv[],char** input_file,char** query_file,int* k,int* L,char** output_file,int* N,double* R,int default_k,int default_L,int default_N,double default_R);

// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void ARGS_print_lsh_args(char* input_file,char* query_file,int k,int L,char* output_file,int N,double R);

// Synarthsh diavasmatos orismatwn 
int ARGS_get_cube_args(int argc, char* argv[],char** input_file,char** query_file,int* k,int* M,char** output_file,int* N,double* R,int* probes,int default_k,int default_M,int default_N,double default_R,int default_probes);

// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void ARGS_print_cube_args(char* input_file,char* query_file,int k,int M,char* output_file,int N,double R,int probes);

#endif