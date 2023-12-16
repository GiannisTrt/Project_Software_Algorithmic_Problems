#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdlib>

#ifndef _BUCKET_
#define _BUCKET_

	// Orismos komvou bucket
	typedef struct bucket{
		
		int length;
		int* keys;
		
	}bucket;
	
	// Emfanish bucket
	void BUCKET_print(bucket* bucket_ptr);

#endif