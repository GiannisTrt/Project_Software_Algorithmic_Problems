#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include"list.h"
#include"bucket.h"
#include"arguments.h"
#include"vector.h"
#include"hash.h"
#include"metrics.h"

#define LSH_k 4
#define LSH_L 5
#define LSH_N 1
#define LSH_R 10000
#define LSH_CONFIG "lsh.conf"

int main(int argc, char* argv[]){
	
	// Parametropoihsh metrikhs
	double (*metric)(int*,int*,int) = METRIC_L2;
	
	// Shmaia periorismoy h mh ston ari8mo twn elegxomenwn apo thn lsh dianysmatwn 
	int reduce_search;
	
	// Pollaplasiasths toy L poy dinei to megisto ari8mo dianysmatwn sthn periptwsh periorismoy 
	// twn elegxomenwn apo thn lsh dianysmatwn 
	int reducing_multiplier;

	// Diadromh arxeioy eisodoy dianysmatwn  
	char* input_file;
	 
	// Diadromh arxeioy eperwthsewn  
	char* query_file;
	
	// Diadromh arxeioy apotelesmatwn 
	char* output_file;	
	
	// Plh8os h_i synarthsewn poy sygkrotoyn mia sykarthsh katakermatismoy g
	int k;
	
	// Plh8os twn g synarthsewn katakermatismoy
	int L;
		
	// Plh8os plhsiesterwn geitonwn
	int N;
	
	// Aktina plhsiesterwn geitonwn
	double R;
	
	// Pinakas dianysmatwn eisodoy
	int** vectors;
	
	// Plh8os dianysmatwn eisodoy
	int num_of_vectors;
		
	// Pinakas query-dianysmatwn 
	int** queries;
	
	// Plh8os query-dianysmatwn
	int num_of_queries;
	
	// Diastash dianysmatwn 
	int d;
	
	// Diastash query-dianysmatwn 
	int d_q;
		
	// Boh8htikoi ari8modeiktes
	register int i,j,q;
	
	// To m ths parametropoihshs twn h_i synarthsewn
	unsigned int m;
	
	// Plh8os kadwn
	int buckets;
	
	register double*** g;
	// Trisdiastatos pinakas ton opoio voleyei na vlepoume ws disdiastato pinaka deiktwn typoy double.
	// Apo aythn thn optikh gwnia ka8e grammh toy pinaka antistoixei se mia synarthsh katakermatismoy g_i
	// enw ka8e keli antistoixei se mia synarthsh h_i.
	// Synepws exoyme L grammes toy pinaka afot exoyme L synarthseis katakermatismoy g_i kai k sthles afoy ka8e 
	// synarthsh katakermatismoy g_i apoteleitai apo k to plh8os synarthseis katakermatismoy h_i.
	// Ka8e synarthsh hi ekfrazetai san dianysma me d to plh8os syntetagmenes.
	// Oi syntetagmenes aytes epilegontai tyxaia sto diasthma (0, W] opoy to W 8ewreitai gnwsto.  
	
	register void*** hashtables;
	// Trisdiastatos pinakas ton opoio voleyei na vlepoume ws disdiastato pinaka deiktwn typoy void.
	// Apo aythn thn optikh gwnia ka8e grammh toy pinaka antistoixei se enan pinaka katakermatismoy
	// enw ka8e keli antistoixei se enan kado.
	// Ka8e kados se prwth fash ylopoieitai ws oyra akeraiwn. Sth synexeia gia logoys taxythtas oi oyres 8a metatrepontai 
	// se structs poy 8a periexoyn enan dynamiko pinaka (pou 8a prokyptei apo thn oyra) kai to antsitoixo mhkos 
	// (dhladh to plh8os twn dianysmatwn poy 8a periexei o kados)
	// Shmeiwnoyme oti ka8e kados 8a periexei ayksontes ari8moys dianysmatwn, oxi ta idia ta dianysmata gia
	// profanh oikonomia xwroy. Etsi an gia paradeigma to dianysma me ayksonta ari8mo 3, otan katakermatizetai me thn g0, 
	// dinei as poyme apotelesma 2, tote sthn oyra toy kelioy hashtables[0][2] eisagetai o aykswn ari8mos 3. 
	// H arithmhsh twn dianysmatwn ksekina apo 1.
	
	// Pinakas dynamewn toy m
	register int* m_powers;
	
	// Symvolizoyme me M thn posothta 2^(32/k). Xrhsimh ston katakermatismo ka8e dianysmatos 
	// me ka8e synarthsh h_i
	int M;
	
	// H parametros w th 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
	double w;
	
	// O ari8mos bucket enos dianysmatos, opw prokyptei apo ton katakermatismo toy 
	// me mia g_i
	int bucket_number;
	
	// Deikths se bucket
	bucket* bucket_ptr;
	
	// Dynamikos pinakas me toys ari8moys buckets poy psaxnei o lsh 
	int* search_bucket_numbers;
	
	// Dynamikos pinakas me ta buckets poy psaxnei o lsh 
	bucket** search_buckets; 
	
	// Megethos pinaka sygxwneyshs kadwn
	int merge_bucket_length;
	
	// Boh8htikh shmaia 
	int max_flag;
	
	// Megistos ari8mos dianysmatos (boh8htikh metablhth ston pinaka sygxwneyshs)
	int max=0;
	
	// O antistoixos ari8mos bucket
	int max_bucket=0;

	// Ginontai typikes arxikopoihseis (kyriws gia na mh bgazei warnings o metaglwttisths)

	// Dynamikos pinakas me toys ari8modeiktes sygxwneyshs twn buckets 
	int* search_bucket_indeces;
	
	// Plh8os dianysmatwn poy telika periexei o pinakas sygxwneyshs
	int merge_bucket_num_of_vectors;
	
	// O dynamikos pinakas sygxwneyshs twn kadwn
	int* merge_bucket;
	
	// Pinakes apostasewn
	double* lsh_distances;
	double* brutal_search_distances;
	
	// Dynamikos pinakas gia thn apo8hkeysh twn ari8mwn dianysmatwn
	int* vector_numbers;
		
	// Boh8htikes metablhtes taksinomhshs
	int temp_int;
	double temp_double;
	
	// O diaireths poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
	int divider;
	
	// Deikths sto arxeio eksodoy
	FILE* fp;
	
	// Oi zhtoymenoi xronoi
	double lsh_time, brutal_search_time;
	
	// Metablhtes ypologismoy xronoy
	struct timespec start, stop;

	// Elaxistes apostaseis
	double n_distance;
	int n_position;
	
	// Deikths sto arxeio configure toy LSH
	FILE* fp_config;
	
	// Epistrefomeno ths fscanf (kyriws gia na mh bgazei warnings o metaglwttisths)
	int fscanf_result;
		
	// Diavasma orismatwn
	if(ARGS_get_lsh_args(argc,argv,&input_file,&query_file,&k,&L,&output_file,&N,&R,LSH_k,LSH_L,LSH_N,LSH_R)<0)
		exit(-1);
	
	// Anoigma toy arxeiou configure
	fp_config=fopen(LSH_CONFIG,"r");
	
	// Se periptwsh apotyxias to programma kanei eksodo 
	if(fp_config==NULL)
		exit(-1);
		
	// Diavasma toy m ths parametropoihshs twn h_i synarthsewn
	fscanf_result=fscanf(fp_config,"%u",&m);
		
	// Diavasma toy diaireth poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
	fscanf_result=fscanf(fp_config,"%d",&divider);
	
	// Ean einai mhden to programma termatizei
	if(divider==0)
		exit(-1);
	
	// Diavasma ths parametroy w ths 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
	fscanf_result=fscanf(fp_config,"%lf",&w);
	
	// Diavasma shmaias periorismoy h mh ston ari8mo twn elegxomenwn apo thn lsh dianysmatwn 	
	fscanf_result=fscanf(fp_config,"%d",&reduce_search);
		
	// Diavasma toy pollaplasiasth toy L poy dinei to megisto ari8mo dianysmatwn sthn periptwsh
	// periorismoy twn elegxomenwn apo thn lsh dianysmatwn 
	fscanf_result=fscanf(fp_config,"%d",&reducing_multiplier);
	
	// Kleisimo arxeiou configure
	fclose(fp_config);
	
	// Se periptwsh poy apotyxei to diavsma h shmaia periorismoy ti8etai mhden 
	// aneksarthta apo thn timh ths
	if(fscanf_result==0)
		reduce_search=0;
	
	// Diavasma dianysmatwn apo arxeio
	VECTOR_read_vectors_from_file(input_file,&vectors,&num_of_vectors,&d);
			  
	// Dhmioyrgia synarthsewn katakermatismoy
	g=HASH_create_hash_functions(L,k,d,w);
	
	// Ypologizetai to M = 2^(32/k)
	// Xrhsimo ston katakermatismo ka8e dianysmatos  me ka8e synarthsh h_i	
	M = 1;		
	for(i=0;i<32/k;i++)
		M*=2;
	
	// Dhmioyrgia twn dynamewn toy m
	m_powers=HASH_create_m_powers(d,m,M);
			
	buckets=num_of_vectors/divider;

	// Desmeyoyme mnhmh gia ton pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
	hashtables=malloc(L*sizeof(void**));
	for(i=0;i<L;i++)
		hashtables[i]=malloc(buckets*sizeof(void*));
	
	// Gia ka8e kado dhmioyrgoyme mia lista
	for(i=0;i<L;i++)
		for(j=0;j<buckets;j++)
		hashtables[i][j]=LIST_create();
		
	// Ka8e dianysma katakermatizetai me ka8e g_i kai mpainei stoys antistoixous kadous
	for(i=0;i<num_of_vectors;i++){
		for(j=0;j<L;j++){
			
			// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
			bucket_number=HASH_g_i(g[j],vectors[i],m_powers,w,d,M,k,buckets);
			
			// Eisagwgh toy dianysmatos sto bucket poy proekypse 
			LIST_insert(hashtables[j][bucket_number],i+1);
		}	
	}
	
	// Twra metatrepoyme ta buckets apo listes se dynamikoys pinakes gia na beltiwsoyme 
	// thn taxythta.
	for(i=0;i<L;i++){
		for(j=0;j<buckets;j++){
				
			// Kataskeyh bucket apo th sxetikh lista
			bucket_ptr=LIST_convert_to_bucket(hashtables[i][j]);
			
			// Adeiasma listas
			LIST_empty(hashtables[i][j]);
			
			// Apeley8erwsh xwroy listas
			free(hashtables[i][j]);
			
			// Ekxwrhsh bucket sthn antistoixh 8esh
			hashtables[i][j]=bucket_ptr;
					
		}
	
	}
	
	// Diavasma query-dianysmatwn apo arxeio
	VECTOR_read_vectors_from_file(query_file,&queries,&num_of_queries,&d_q);
	
	// Elegxos ean sympiptoyn oi diastaseis (prepei na sympiptoyn)
	if(d_q!=d){
		
		printf("Diamensions of query vectors are different from input vectors!\n");
		
		VECTOR_free_vectors(vectors,num_of_vectors,d);
		VECTOR_free_vectors(queries,num_of_queries,d_q);
	
		HASH_free_hash_functions(g,L,k);
		
		exit(-1);
		
	}
	
	// Desmeysh pinaka me toys ari8moys buckets ka8e pinaka katakermatismoy 
	// poy psaxnei o lsh gia ka8e query 
	search_bucket_numbers=malloc(L*sizeof(int));
	
	// Desmeysh pinaka me toys deiktes sta bucketska8e pinaka katakermatismoy 
	// poy psaxnei o lsh gia ka8e query 
	search_buckets=malloc(L*sizeof(bucket*));
	
	// Desmeysh pinaka me toys ari8modeiktes sygxwneyshs twn buckets 
	search_bucket_indeces=malloc(L*sizeof(int));
	
	// Desmeysh pinaka me toys ayksontes ari8moys dianysmatwn
	vector_numbers=malloc(num_of_vectors*sizeof(int));

	// Desmeysh pinaka me tis apostaseis enos query dianysmatos apo ta ypoloipa 
	brutal_search_distances=malloc(num_of_vectors*sizeof(double));

	// Anoigma arxeioy eksodoy
	fp = fopen(output_file, "w");
	
	for(q=0;q<num_of_queries;q++){
			
		// Arxikopoihsh megethoys pinaka sygxwneyshs kadwn
		merge_bucket_length=0;
			
		// Briskoyme toys kadoys poy 8a ginei anazhthsh
		for(j=0;j<L;j++){
			
			// Ypologismos ari8moy bucket gia ton j pinaka katakermatismoy
			search_bucket_numbers[j]=HASH_g_i(g[j],queries[q],m_powers,w,d,M,k,buckets);
			
			// Ypologismos diey8ynshs toy en logw bucket
			search_buckets[j]=hashtables[j][search_bucket_numbers[j]];
						
			// Enhmerwsh megethoys pinaka sygxwneyshs kadwn
			merge_bucket_length+=search_buckets[j]->length;
			
		}	
		
		// Ean doyleyoyme me periorismo anazhthsewn, anaprosarmozoyme to bucket sygxwneyshs
		if(reduce_search){
			
			if(merge_bucket_length>10*L)
				merge_bucket_length=10*L;
			
		}
		
		// Desmeysh mnhmhs dynamikoy pinaka sygxwneyshs twn kadwn
		merge_bucket=malloc(merge_bucket_length*sizeof(int));
		
		merge_bucket_num_of_vectors=0;
		
		// Arxikopoihsh ari8modeiktwn sygxwneyshs
		for(j=0;j<L;j++)
			search_bucket_indeces[j]=0;
		
		// Sygxwnweysh
		for(i=0;i<merge_bucket_length;i++){
		
			// Arxikopoihsh shmaias sygxwneyshs
			max_flag=0;
							
			for(j=0;j<L;j++){
				
				// Periptwsh arxikopoihshs max trexonta gyroy 
				if(max_flag==0){
					
					if(search_bucket_indeces[j]<search_buckets[j]->length){
						
						max=search_buckets[j]->keys[search_bucket_indeces[j]];
						max_bucket=j;
						max_flag=1;
					}
									
				}
				else{
				// Periptwsh enhmerwshs max trexonta gyroy 	
					if(search_bucket_indeces[j]<search_buckets[j]->length){
						
						if(search_buckets[j]->keys[search_bucket_indeces[j]]>max){
							max=search_buckets[j]->keys[search_bucket_indeces[j]];
							max_bucket=j;
						}
					}					
				}				
			}
			// Pros8hkh toy megistoy enapomeinanta kadoy ston pinaka sygxwneyshs 
			// an den yparxei hdh
			if(merge_bucket_num_of_vectors==0)				
				merge_bucket[merge_bucket_num_of_vectors++]=max;
			else if(merge_bucket[merge_bucket_num_of_vectors-1]!=max)
				merge_bucket[merge_bucket_num_of_vectors++]=max;			
			
			// Enhmerwsh sxetikoy ari8modeikth
			search_bucket_indeces[max_bucket]++;
		
		}	
		
		// Enarksh xronometroy xronoy lsh
		clock_gettime( CLOCK_REALTIME, &start);
		
		// Desmeysh pinaka me tis apostaseis enos query dianysmatos ta dianysmata poy orizei o lsh  
		lsh_distances=malloc(merge_bucket_num_of_vectors*sizeof(double));
		
		// Ypologismos apostasewn toy trexontos query me ka8ena apo ta dianysmata poy orizei o lsh
		for(i=0;i<merge_bucket_num_of_vectors;i++)			
			lsh_distances[i]=metric(vectors[merge_bucket[i]-1],queries[q],d);
		
		
		// Eyresh tou dianysmatos me th megalyterh apostash apo to dianysma-query anamesa sta N prwta
		n_position=0;
		n_distance=lsh_distances[0];
		for(j=1;j<N;j++){
			
			if(lsh_distances[j]>n_distance){
				
				n_distance=lsh_distances[j];
				n_position=j;
				
			}
			
		}
		
		
		// Antistrofh sarwsh twn ypoloipwn kai stadiakh eyresh twn N dianysmatwn
		// me tis mikroteres apostaseis apo to dianysma-query (me to peras ths sarwshs 
		// katalambanoyn tis prwtes N-1 8eseis toy pinaka)
		for(i=merge_bucket_num_of_vectors-1;i>=N;i--){
							
			if(lsh_distances[i]<n_distance){
				
				temp_double=lsh_distances[i];
				lsh_distances[i]=lsh_distances[n_position];
				lsh_distances[n_position]=temp_double;
				
				temp_int=merge_bucket[i];
				merge_bucket[i]=merge_bucket[n_position];
				merge_bucket[n_position]=temp_int;
				
				n_position=0;
				n_distance=lsh_distances[0];
				for(j=1;j<N;j++){
					
					if(lsh_distances[j]>n_distance){
						
						n_distance=lsh_distances[j];
						n_position=j;
						
					}
					
				}
		
			}				
									
		}
		
		
		// Taksinomhsh twn N dianysmatwn me th mikroterh apostash apo to dianysma-query		
		for(i=1;i<=N;i++){
			
			for(j=N-1;j>=i;j--){
				
				if(lsh_distances[j]<lsh_distances[j-1]){
					
					temp_double=lsh_distances[j];
					lsh_distances[j]=lsh_distances[j-1];
					lsh_distances[j-1]=temp_double;
					
					temp_int=merge_bucket[j];
					merge_bucket[j]=merge_bucket[j-1];
					merge_bucket[j-1]=temp_int;			
					
				}				
			}			
		}		
		
		// Lhksh xronometroy xronoy lsh
		clock_gettime(CLOCK_REALTIME,&stop);

		// Ypologismow xronoy lsh
		lsh_time = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec);
		
		// Enarksh xronometroy xronoy eksantlhtikhs anazhthshs
		clock_gettime(CLOCK_REALTIME,&start);
	
		// Arxikopoihsh toy pinaka me toys ari8moys twn dianysmatwn
		for(i=0;i<num_of_vectors;i++)
			vector_numbers[i]=i+1;
				
		// Ypologismos apostasewn toy trexontos query me ka8ena apo ta ypoloipa dianysmata
		for(i=0;i<num_of_vectors;i++)			
			brutal_search_distances[i]=metric(vectors[i],queries[q],d);
			
		// Eyresh tou dianysmatos me th megalyterh apostash apo to dianysma-query anamesa sta N prwta
		n_position=0;
		n_distance=brutal_search_distances[0];
		for(j=1;j<N;j++){
			
			if(brutal_search_distances[j]>n_distance){
				
				n_distance=brutal_search_distances[j];
				n_position=j;
				
			}
			
		}
		
		// Antistrofh sarwsh twn ypoloipwn kai stadiakh eyresh twn N dianysmatwn
		// me tis mikroteres apostaseis apo to dianysma-query (me to peras ths sarwshs 
		// katalambanoyn tis prwtes N-1 8eseis toy pinaka)
		for(i=num_of_vectors-1;i>=N;i--){
							
			if(brutal_search_distances[i]<n_distance){
				
				temp_double=brutal_search_distances[i];
				brutal_search_distances[i]=brutal_search_distances[n_position];
				brutal_search_distances[n_position]=temp_double;
				
				temp_int=vector_numbers[i];
				vector_numbers[i]=vector_numbers[n_position];
				vector_numbers[n_position]=temp_int;
				
				n_position=0;
				n_distance=brutal_search_distances[0];
				for(j=1;j<N;j++){
					
					if(brutal_search_distances[j]>n_distance){
						
						n_distance=brutal_search_distances[j];
						n_position=j;
						
					}
					
				}
		
			}	
						
		}
		
		// Taksinomhsh twn N dianysmatwn me th mikroterh apostash apo to dianysma-query
		for(i=1;i<=N;i++){
			
			for(j=N-1;j>=i;j--){
				
				if(brutal_search_distances[j]<brutal_search_distances[j-1]){
					
					temp_double=brutal_search_distances[j];
					brutal_search_distances[j]=brutal_search_distances[j-1];
					brutal_search_distances[j-1]=temp_double;
					
					temp_int=vector_numbers[j];
					vector_numbers[j]=vector_numbers[j-1];
					vector_numbers[j-1]=temp_int;			
					
				}				
			}			
		}	
	
		// Lhksh xronometroy xronoy eksantlhtikhs anazhthshs	
		clock_gettime(CLOCK_REALTIME,&stop);
		
		// Ypologismow xronoy eksantlhtikhs anazhthshs	
		brutal_search_time = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec);
				
		fprintf(fp,"Query id: %d\n", q+1);
		
		// Eggrafh apotelesmatwn
		for(i=0;i<N;i++){
			
			fprintf(fp,"Nearest neighbor-%d: %d\n",i+1,merge_bucket[i]);
			fprintf(fp,"distanceLSH: %.6lf\n",lsh_distances[i]);
			fprintf(fp,"distanceTrue: %.6lf\n",brutal_search_distances[i]);
		}
		fprintf(fp,"tLSH: %.3lf microseconds\n",lsh_time/1000);
		fprintf(fp,"tTrue: %.3lf microseconds\n",brutal_search_time/1000);	
		
		// Emfanish twn R-nearest neighbors (exoyme hdh apo8hkeymenes tis apostaseis)
		fprintf(fp,"%.3lf-nearest neighbors:\n",R);		
		for(i=0;i<merge_bucket_num_of_vectors;i++){
			
			if(lsh_distances[i]<R){
				
				fprintf(fp,"%d\n",merge_bucket[i]);
				
			}			
			
		}
		fprintf(fp,"\n");
		
		// Apeley8erwsh mnhmhs pinaka sygxwneyshs bucket		
		free(merge_bucket);
		
		// Apeley8erwsh mnhmhs pinaka me tis apostaseis enos query dianysmatos ta dianysmata poy orizei o lsh  
		free(lsh_distances);
		
	}	
		
	// Kleisimo arxeioy eksodoy
	fclose(fp);
	
	// Apeley8erwsh mnhmhs pinaka ari8mwn dianysmatwn
	free(vector_numbers);
	
	// Apeley8erwsh mnhmhs pinaka me tis apostaseis enos query dianysmatos apo ta ypoloipa 
	free(brutal_search_distances);
	
	// Apeley8erwsh mnhmhs
	free(search_bucket_numbers);
	free(search_buckets);
	free(search_bucket_indeces);
	
	// Apeley8erwsh pinakwn dianysmatwn eisodoy kai dianysmatwn queries
	VECTOR_free_vectors(vectors,num_of_vectors,d);
	VECTOR_free_vectors(queries,num_of_queries,d);
	
	// Apeley8erwsh synarthsewn katakermatismoys
	HASH_free_hash_functions(g,L,k);
	
	// Apeley8erwsh pinaka m-dianysmatos
	free(m_powers);
	
	// Apeley8erwsh mnhmhs toy pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
	for(i=0;i<L;i++){
		for(j=0;j<buckets;j++){
			
			free(((bucket*)(hashtables[i][j]))->keys);
			free(hashtables[i][j]);
			
		}
		free(hashtables[i]);		
		
	}
	free(hashtables);	
	
	exit(0);
	
}

