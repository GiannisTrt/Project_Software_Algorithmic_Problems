#include "hash.h"


// Synarthsh poy dhmioyrgei kai epistrefei tis synarthseis katakermatismoy g_i se morfh pinaka g
double*** HASH_create_hash_functions(int L, int k, int d, double w){ 
      
	// Ari8modeiktes
	int g_i,h_i,d_i,i,j; 
	
	// Tyxaioi akeraioi
	int first_random;
	int second_random;
	
	// O pinakas g poy periexei tis g_i kai o opoios 8a epistrafei
	double*** g;
	
	// Desmeysh mnhmhs gia ton pinaka
	g=malloc(L*sizeof(double**));

	for(i=0;i<L;i++){
		g[i]=malloc(k*sizeof(double*));
		for(j=0;j<k;j++)
			g[i][j]=malloc(d*sizeof(double));
	}
	
	// Ka8orismos toy trexonta xronoy ws sporoy thw rand gia kalyterh tyxaiopoihsh
	srand(time(NULL));
	
	// Gia ka8e synarthsh g_i...
	for(g_i=0;g_i<L;g_i++){

		// ...gia ka8e synarthsh h_i...
		for(h_i=0;h_i<k;h_i++){
		
			//...dhmioyrgoyntai diadoxika oi syntelestes.	
			for(d_i=0;d_i<d;d_i++){
				
				// Ka8e syntelesths einai enas tyxaios ari8mos sto diasthma [0,w).
				// Gia na ton ekleksoyme arxika ftiaxnoyme enan tyxaio ari8mo sto diasthma [0,1).
				// Gia na ton ftiaksoyme ari8mo sto diasthma [0,1) eklegoyme dyo 8etikoys akeraioys etsi wste o prwtos na einai
				// mikroteros toy deyteroy (aytomata ayto apokleiei thn periptwsh na einai o deyteros mhden).
				// To phliko aytwn 8a einai enas pragmatikos sto diasthma [0,1).
				do{
					
					first_random=rand();
					second_random=rand();					
					
				}while(first_random>=second_random);
				
				// Ekxwrhsh tyxaioy ari8moy toy diasthmatos [0,w) ws epomeno syntelesth ths h_i
				g[g_i][h_i][d_i]=first_random/(double)second_random*w;
			}
			
		}
		
	}
	
	// Epistrofh pinaka synarthsewn katakermatismoy
	return g;
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

// Dhmioyrgia dynamewn toy m
int* HASH_create_m_powers(int d,unsigned int m,int M){
		
	// Boh8htikh metablhth
	int i;
	
	// Pinakas dynamewn toy m
	int* m_powers;
	
	// H posothta m mod M
	int m_mod_M;
	
	m_mod_M = (int)(m % (unsigned int)M);
	
	// Desmeysh xwroy gia ton pinaka pou 8a epistrafei
	m_powers=malloc(sizeof(int)*d);
		
	// Kataskeyh pinaka pou 8a epistrafei
	m_powers[0]=1; // m_powers[0] = m^0 mod M
	m_powers[1]=m_mod_M; // m_powers[1] = m^1 mod M
	
	for(i=2;i<d;i++)	
		m_powers[i]=(m_powers[i-1]*m_mod_M)%M; // m_powers[1] = m^i mod M

	return m_powers;
	
}

// H synarthsh epistrefei se morfh akeraioy ta k deksiotera pshfia toy katakermatismoy 
// toy dianysmatos vector me th synarthsh katakermatismoy h_i
int HASH_h_i(double* h_i,int* vector,int* m_powers,double w, int d, int M){
	
	// Epistrefomeno a8roisma
	int S=0;
	
	// Metablhth paragonta poy pollaplasiazetai me tis dynameis toy m
	int d_i;
	
	// Boh8htikos ari8modeikths
	int i;
	
	for(i=d-1;i>-1;i--){
		
		// Ypologismos d_i
		d_i=floor((vector[i]-h_i[i])/w);
				
		// Modular exponentiation
		if(d_i>=0)
			d_i=d_i%M ;
		else
			d_i=(M-(-d_i%M))%M;
		
		// Enhmerwsh a8roismatos kai modular exponentiation
		S=(S+m_powers[d-1-i]*d_i)%M ;		
		
	}
	
	return S;
	
	
}


// H synarthsh epistrefei ton ari8mo kadoy pou mpainei ena dianysma me bash thn 
// amplified synarthsh katakermatismoy g_i
int HASH_g_i(double** g_i,int* vector,int* m_powers,double w, int d, int M, int k,int buckets){
	
	// Ari8modeikths 
	int i;
	
	// Apotelesma hash ths h_i synarthshs
	int h_i_hash;
	
	// O ari8mos toy kadou o opoios 8a epistrafei
	unsigned int bucket_number;
	
	// H posothta 32/k (tha xreiastei sthn olis8hsh)
	int div_32_k;

	div_32_k=32/k;
	
	// Arxikopoihsh toy ari8mou kadoy	
	bucket_number=0;
	
	// Gia ka8e mia apo tis synarthseis h_i...
	for(i=0;i<k;i++){
		
		// ...ypologizoyme thn h_i...
		h_i_hash=HASH_h_i(g_i[i],vector,m_powers,w,d,M);

		// ... thn kankoyme shift left kai enhmerwnoyme ton ari8mo  bucket
		bucket_number = (bucket_number<<(div_32_k)) | h_i_hash;
	}
	
	bucket_number=bucket_number%buckets;

	return bucket_number;
	
}