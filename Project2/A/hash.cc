#include "hash.h"
#include <limits.h>


// Synarthsh poy dhmioyrgei kai epistrefei tis synarthseis katakermatismoy g_i se morfh pinaka g
double*** HASH_create_hash_functions(int L, int k, int d, double w){ 
      
	// Ari8modeiktes
	int g_i,h_i,d_i,i,j; 
	
	// O pinakas g poy periexei tis g_i kai o opoios 8a epistrafei
	double*** g;
	
	// Desmeysh mnhmhs gia ton pinaka
	g=(double***)malloc(L*sizeof(double**));

	for(i=0;i<L;i++){
		g[i]=(double**)malloc(k*sizeof(double*));
		for(j=0;j<k;j++)
			g[i][j]=(double*)malloc((d+1)*sizeof(double));
	}
	
	// Ka8orismos toy trexonta xronoy ws sporoy thw rand gia kalyterh tyxaiopoihsh
	srand(time(NULL));
	
	// Gia ka8e synarthsh g_i...
	for(g_i=0;g_i<L;g_i++){

		// ...gia ka8e synarthsh h_i...
		for(h_i=0;h_i<k;h_i++){
		
			//...dhmioyrgoyntai diadoxika oi syntelestes.	
			for(d_i=0;d_i<d;d_i++){
				
				// Dhmioyrgeitai dianysma me tyxaies syntetagmenes poy akoloy8oyn thn kanonikh katanomh sto
				// diasthma (0,1) 
				
				// Ekxwrhsh tyxaioy ari8moy toy diasthmatos [0,w) ws epomeno syntelesth ths h_i
				g[g_i][h_i][d_i]=0.5+(rand()%2 ? -1.0 : 1.0)*0.1666*pow(-log((1 - __DBL_EPSILON__)*(__DBL_EPSILON__ + ((double) rand()/RAND_MAX))), 0.5);
			}
			
			// Se mia epipleon 8esh apo8hkeyoyme to t gia ton lsh, to 0 gia ton cube
			if(L!=1)
				g[g_i][h_i][d_i]=w*(0.5+(rand()%2 ? -1.0 : 1.0)*0.1666*pow(-log((1 - __DBL_EPSILON__)*(__DBL_EPSILON__ + ((double) rand()/RAND_MAX))), 0.5));
			else
				g[g_i][h_i][d_i]=0;
			
		}
		
	}
	
	// Epistrofh pinaka synarthsewn katakermatismoy
	return g;
} 

// Synarthsh poy dhmioyrgei kai epistrefei tih synarthseis katakermatismoy h se morfh pinaka (Gia hypercube)
double** HASH_create_hash_function(int k, int d, double w){ 
      
	// Ari8modeiktes
	int g_i,h_i,d_i,i,j; 
	
	// O pinakas g poy periexei tis g_i kai o opoios 8a epistrafei
	double** h;
	
	// Desmeysh mnhmhs gia ton pinaka
	h=(double**)malloc(k*sizeof(double*));


	for(j=0;j<k;j++)
		h[j]=(double*)malloc((d+1)*sizeof(double));

	// Ka8orismos toy trexonta xronoy ws sporoy thw rand gia kalyterh tyxaiopoihsh
	srand(time(NULL));
	
	// Gia ka8e synarthsh h_i...
	for(h_i=0;h_i<k;h_i++){
	
		//...dhmioyrgoyntai diadoxika oi syntelestes.	
		for(d_i=0;d_i<d;d_i++){
			
			// Dhmioyrgeitai dianysma me tyxaies syntetagmenes poy akoloy8oyn thn kanonikh katanomh sto
			// diasthma (0,1) 
			
			// Ekxwrhsh tyxaioy ari8moy toy diasthmatos [0,w) ws epomeno syntelesth ths h_i
			h[h_i][d_i]=0.5+(rand()%2 ? -1.0 : 1.0)*0.1666*pow(-log((1 - __DBL_EPSILON__)*(__DBL_EPSILON__ + ((double) rand()/RAND_MAX))), 0.5);
		}
		
		h[h_i][d_i]=0;
		
	}
	
	
	// Epistrofh pinaka synarthsewn katakermatismoy
	return h;
} 




// Apeley8erwsh mnhmhs toy pinaka twn synarthsewn katakermatismoy
void HASH_free_hash_functions(double*** g,int L, int k){
	
	// Ari8modeiktes
	int g_i,h_i;

	for(g_i=0;g_i<L;g_i++){
		for(h_i=0;h_i<k;h_i++)
			free(g[g_i][h_i]);
		free(g[g_i]);
	}
	free(g);
}

// Apeley8erwsh mnhmhs toy pinaka ths synarthshs katakermatismoy (Gia hypercube)
void HASH_free_hash_function(double** h,int k){
	
	// Ari8modeikths
	int h_i;


	for(h_i=0;h_i<k;h_i++)
		free(h[h_i]);
	free(h);
	
}

// Synarthsh emfanishs synarthsewn katakermatismoy (boh8htikh gia logoys elegxoy)
void HASH_print_hash_functions(double*** g,int L, int k, int d){
	
	// Ari8modeiktes
	int g_i,h_i,d_i;

	for(g_i=0;g_i<L;g_i++){
		
		printf("\ng_%d\n",g_i);
		
		for(h_i=0;h_i<k;h_i++){
			
			printf("h_%d: ",h_i);
			for(d_i=0;d_i<d;d_i++)
				printf("%.3lf ",g[g_i][h_i][d_i]);
			
			printf("\n");
			
		}
	}
}

// Dhmioyrgia timwn r
int* HASH_create_r_values(int d){
		
	// Boh8htikh metablhth
	int i;
	
	// Pinakas timwn r
	int* r_values;
	
	r_values=(int*)malloc(d*sizeof(int));
	
	for(i=0;i<d;i++)	
		r_values[i]=rand();

	return r_values;

}

// Synartyhsh h_i twn algori8mwn
int HASH_h_i(double* h_i,double* vector,double w, int d){
	
	// Boh8htiko a8roisma
	double S=0;

	// Boh8htikos ari8modeikths
	int i;
	
	// Ypologizetai to eswteriko ginomeno
	for(i=0;i<d;i++){
		
		S+=h_i[i]*vector[i];
		
	}
	// Prosti8etai to t
	S+=h_i[i];
	
	
	return floor(S/w);
	
	
}


// H synarthsh epistrefei ton ari8mo kadoy pou mpainei ena dianysma me bash thn 
// amplified synarthsh katakermatismoy g_i
int HASH_g_i(double** g_i,double* vector,int* r_values,double w, int d, int M, int k,int buckets){
	
	// Ari8modeikths 
	register int i;
	
	// O ari8mos toy kadou o opoios 8a epistrafei
	register unsigned int bucket_number;
		
	// Arxikopoihsh toy ari8mou kadoy	
	bucket_number=0;
	
	// Gia ka8e mia apo tis synarthseis h_i...
	for(i=0;i<k;i++){
		
		// ... ypologizoyme thn timh ths kai enhmerwnoyme to a8roisma ginomenwn
		bucket_number+= (int)(HASH_h_i(g_i[i],vector,w,d)*r_values[i])%M;
	}
	
	bucket_number=bucket_number%buckets;

	return bucket_number;
	
}