#include "bucket.h"

// Emfanish bucket
void BUCKET_print(bucket* bucket_ptr){
	
	// Ari8modeikths
	int i;
	
	// To mhkos tou bucket
	int length;
	
	// Deikths sto prwto stoixeio toy bucket
	int* keys;
	
	
	length=bucket_ptr->length;
	keys=bucket_ptr->keys;
	printf("Number of keys: %d Keys: ",length);
	
	for(i=0;i<length;i++)
		printf("%d ",keys[i]);
	printf("\n");
	
}
