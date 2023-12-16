#include "hamming.h"

// Epistrefei thn apostash hamming dyo koryfwn
int CUBE_hamming_distance(int vertex_a,int vertex_b,int num_of_vertices){
	
	// Arxikopoihsh ths apostashs hamming
	int hamming_distance = 0;
	
	// Bitwise xor and twn dyo koryfwn
	int hamming = vertex_a ^ vertex_b;
	
	// Boh8htikh memtablhth
	int i;
	
	// Gia kathe bit
	for(i=0;i<num_of_vertices;i++){
		
		// ...prostithetai to hamming bit sto a8roisma..
		hamming_distance+= (hamming & 1);
		
		//..kai proxwrame sto epomeno
		hamming = hamming >> 1;
	}
	
	// H apostash hamming epistrefetai
	return hamming_distance;
		
}
