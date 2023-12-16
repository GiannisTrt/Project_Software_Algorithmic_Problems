#include "metrics.h"

// Eykleideia metrikh
double METRIC_L2(int* first_vector,int* second_vector, int d){
	
	int i;
	
	unsigned long long distance=0;
	
	for(i=0;i<d;i++){
		
		distance+=(first_vector[i]-second_vector[i])*(first_vector[i]-second_vector[i]);
		
	}
	
	return sqrt(distance);
	
}
