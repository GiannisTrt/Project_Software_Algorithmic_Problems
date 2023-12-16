#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <limits.h>

#include "constants.h"
#include "vector.h"
#include "metrics.h"
#include "list.h"
#include "hash.h"
#include "bucket.h"
#include "cluster_args.h"
#include "init.h"
#include "assign.h"
#include "update.h"
#include "hamming.h"

int main(int argc, char* argv[]){
	
	// To arxeio eisodoy
	char* input_file;
	
	// To arxeio configuration
	char* configuration_file;
	
	// To arxeio eksodoy
	char* output_file;
	
	// H shmaia complete
	int complete;
	
	// H me8odos (ypo th morfh kwdikoy)
	int method;
	
	// O ari8mos twn kentrwn
	int K;
	
	// To plh8os twn g_i synarthsewn (kadwn) stn LSH
	int L;
	
	// To k toy LSH (plh8os h_i synarthsewn)
	int LSH_k;
	
	// To M toy yperkyboy (megisto plh8os dianysmatwn poy elegxontai)
	int HYPERCUBE_M;
	
	// To k toy yperkyboy (diastash kyboy)
	int HYPERCUBE_k;
	
	// Ta probes (geitonikes koryfes poy eksetazontai) toy yperkyboy
	int HYPERCUBE_probes;
	
	// Pinakas dianysmatwn eisodoy
	int** vectors;
	
	// Boh8htikh metablhth
	int* vct;
	
	// Deikths arxeioy eksodoy
	FILE* fp;
	
	// O parakatw pinakas einai 1 pros 1 me tis grammes toy pinaka vectors
	// Ka8e stoixeio antistoixei se ena dianysma kai periexei to kentro tou cluster sto opoio anhkei to dianysma 
	int* cluster_center_of_vector;
	
	// Oi apostaseis twn dianysmatwn apo ta antisoixa kentra twn klasewn toys (pinakas)
	double* distance_from_cluster_center;
	
	// Plh8os dianysmatwn eisodoy
	int num_of_vectors;
	
	// Diastash dianysmatwn eisodoy
	int d;
	
	// Boh8htikoi ari8modeiktes
	register int i;
	register int j;
	register int v;
	
	// Ta clusters ylopoioyntai san pinakes apo listes
	list** clusters;
	
	// To m toy lsh
	unsigned int lsh_m;
	
	// Pinakas me ta kentra twn klasewn
	int* cluster_centers;
	
	// To w toy lsh
	double lsh_w;
	
	// Boh8htikes metablhtes
	int current_center_index;
	int current_vector_index;
	register double*** g;
	
	// Trisdiastatos pinakas ton opoio voleyei na vlepoume ws disdiastato pinaka deiktwn typoy double.
	// Apo aythn thn optikh gwnia ka8e grammh toy pinaka antistoixei se mia synarthsh katakermatismoy g_i
	// enw ka8e keli antistoixei se mia synarthsh h_i.
	// Synepws, ston LSH exoyme L grammes toy pinaka afot exoyme L synarthseis katakermatismoy g_i kai k sthles afoy ka8e 
	// synarthsh katakermatismoy g_i apoteleitai apo k to plh8os synarthseis katakermatismoy h_i.
	// Ston HYPERCUBE exoyme k grammes toy pinaka afoy exoyme k synarthseis katakermatismoy g_i kai 
	// cube_num_of_hi_functions sthles afoy ka8e synarthsh katakermatismoy g_i apoteleitai apo cube_num_of_hi_functions
	// to plh8os synarthseis katakermatismoy h_i.
	// Ka8e synarthsh hi ekfrazetai san dianysma me d to plh8os syntetagmenes.
	// Oi syntetagmenes aytes epilegontai tyxaia sto diasthma (0, W] opoy to W 8ewreitai gnwsto.  
	
	// Pinakas dynamewn toy m
	register int* m_powers;
	
	register void*** lsh_hashtables;
	// Oi pinakes katakermatismoy toy LSH.
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
	
	// Symvolizoyme me M thn posothta 2^(32/k). Xrhsimh ston katakermatismo ka8e dianysmatos 
	// me ka8e synarthsh h_i
	int M;
	
	// O ari8mos bucket enos dianysmatos, opw prokyptei apo ton katakermatismo toy 
	// me mia g_i
	int bucket_number;
	
	// Deikths se bucket
	bucket* bucket_ptr;

	// Boh8htikh metablhth (mhkos bucket)
	int bucket_length;

	// Plh8os kadwn
	int buckets;
	
	// O divider toy lsh opws ton eidame sto prwto meros
	int lsh_divider;
	
	// Didiastatos pinakas me ta buckets twn kentrwn
	int** buckets_of_current_centers;
	
	// To m toy hypercube
	unsigned int cube_m;
	
	// O divider toy hypercube opws ton eidame sto prwto meros
	int cube_divider;
	
	// To w toy hypercube
	double cube_w;
	
	// To proka8orismeno plh8os h_i synarthsewn katakermatismoy toy hypercube
	int cube_num_of_hi_functions;
	
	register int** cube_hashtables;
	// Disdiastatos me CUBE_k grammes (mia gia ka8e synarthsh katakermatismoy g_i) oses kai oi g_i
	// kai sthles oses kai to plh8os twn kadwn (poy ypologizetai diairwntas to plh8os twn dianysmatwn
	// me enan proka8orismeno ari8mo-diaireth). 
	// Ta stoixeia einai eite mhden eite 1. Ka8ena eks' aytwn antiproswpeyei enan kado.
	// O kpshfios dyadikos ari8mos poy sxhmatizetai apo ton katakermatismo enos dianysmatos me ka8e gi 
	// dinei thn koryfh toy yperkyboy poy eisagetai.
	
	// Metablhth plh8ous koryfwn yperkyboy 
	int num_of_vertices;
	
	// Ari8mos koryfhs yperkyboy
	int vertex_number;
	
	// O yperkybos
	void** hypercube;	
	
	// Pinakas pou 8a apo8hkeyei proswrina tis hamming apostaseis mias koryfhs apo tis ypoloipes
	int* hamming_distances;
	
	// Pinakas pou 8a apo8hkeysei proswrina tous ari8moys twn koryfwn
	int* vertex_numbers;
	
	// Plh8os allagwn se ka8e epanalhpsh
	int num_of_center_changes;
	
	// Oi dyo aytoi pinakes einai 1 pros 1. Taksinomontas ton prwto kai kratwntas thn 1 pros 1 sxesh me 
	// ton deytero diatassoyme tis koryfes symfwna me tis apostaseis hamming apo sygkekrimenh koryfh
	
	// Disdiastatos pinakas ka8e grammh toy opoioy antistoixei se mia koryfh kai  periexei taksinomhmenes 
	// probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
	// O en logw pinakas dieykolynei th grhgorh eyresh twn geitonikoterwn koryfwn 
	int** probes_array;	
	
	// Ari8mos koryfhs yperkyboy
	int vertex;
	
	// Apostash dyo dianysmatwn
	double distance;
	
	// Monodiastatos pinakas me ta vertices twn kentrwn
	int* vertices_of_current_centers;
	
	// Megistos ari8mos epanalhpsewn assign-update
	int max_iterations;
	
	// Epanalhpseis assign-update
	int iterations;
	
	// To mege8os enos cluster 
	int cluster_size;
	
	// To kentro enos cluster
	int cendroid;
	
	// Metablhtes ypologismoy xronoy
	clock_t start;
	clock_t end;
		
	// Metablhtes sihlouette
	double* s_array;
	int best_neihbour;
	double distance_of_best_neihbour;
	int cluster_num;
	int neihbour_cluster_num;
	int* current_cluster;
	int current_cluster_size;
	int* neihbour_cluster;
	int neihbour_cluster_size;	
	double sum_distance;
	double sum_distance_to_neihbours;	
	double a_i, b_i,vector_si, sum_vector_si, s_i,stotal;
	
	// Diavasma kai elegxos orismatwn
	if(CLUSTER_ARGS_get_cluster_args(argc,argv,&input_file,&configuration_file,&output_file,&complete,&method)<0)
		exit(-1);
	
	//CLUSTER_ARGS_print_cluster_args(input_file,configuration_file,output_file,complete,method);
	
	CLUSTER_ARGS_get_cluster_parameters(configuration_file,DEFAULT_K,DEFAULT_L,DEFAULT_LSH_k,DEFAULT_HYPERCUBE_M,DEFAULT_HYPERCUBE_k,DEFAULT_HYPERCUBE_probes,&K,&L,&LSH_k,&HYPERCUBE_M,&HYPERCUBE_k,&HYPERCUBE_probes,&lsh_divider,&lsh_m,&lsh_w,&cube_m,&cube_divider,&cube_w,&cube_num_of_hi_functions,&max_iterations);
		
	//CLUSTER_ARGS_print_cluster_parameters(K,L,LSH_k,HYPERCUBE_M,HYPERCUBE_k,HYPERCUBE_probes);
	
	// Diavasma dianysmatwn apo to arxeio eisodoy
	if(VECTOR_read_vectors_from_file(input_file,&vectors,&num_of_vectors,&d)<0){
		
		printf("Can not read input file!\n");
		exit(-1);
		
	}
	// Enarksh xronometroy 
	start = clock();
	
	// Desmeysh mnhmhs gia ton pinaka me ta kentra twn clusters
	cluster_centers=malloc(K*sizeof(int));
	
	// Desmeysh mnhmhs gia ton pinaka poy periexei gia ka8e dianysma to kentro toy cluster sto opoio anhkei
	cluster_center_of_vector=malloc(num_of_vectors*sizeof(int));
	
	// Desmeysh mnhmhs toy pinaka poy periexei gia ka8e dianysma to kentro toy cluster sto opoio anhkei
	distance_from_cluster_center=malloc(num_of_vectors*sizeof(double));
	
	
	// Desmeysh xwroy gia ta clusters
	clusters=malloc(K*sizeof(list*));
	
	for(i=0;i<K;i++)
		clusters[i]=LIST_create();
	
	INIT_k_means(vectors,num_of_vectors,METRIC_L2,cluster_centers,K,d);
		
	//********** Proetoimasia fashs assign ************//
	
	// Ean h me8odos einai LSH, tote prin apo toys kykloys assignment - update prepei na ginoyn orismenes energeies.
	// Sygkekrimena prepei na kataskeyastei o pinakas g, o pinakas twn dynamewn toy m kai oi pinakes katakermatismoy.
	// Epishs prepei na eisax8oun ta dianysmata stoys pinakes katakermatismoy.
	if(method==LSH){
		
		// Dhmioyrgia synarthsewn katakermatismoy
		g=HASH_create_hash_functions(L,LSH_k,d,lsh_w);
		
		// Ypologizetai to M = 2^(32/LSH_k)
		// Xrhsimo ston katakermatismo ka8e dianysmatos  me ka8e synarthsh h_i	
		M = 1;		
		for(i=0;i<32/LSH_k;i++)
			M*=2;
		
		// Dhmioyrgia twn dynamewn toy m
		m_powers=HASH_create_m_powers(d,lsh_m,M);
				
		buckets=num_of_vectors/lsh_divider;

		// Desmeyoyme mnhmh gia ton pinaka lsh_hashtables poy ylopoiei toys pinakes katakermatismoy 
		lsh_hashtables=malloc(L*sizeof(void**));
		for(i=0;i<L;i++)
			lsh_hashtables[i]=malloc(buckets*sizeof(void*));
		
		// Gia ka8e kado dhmioyrgoyme mia lista
		for(i=0;i<L;i++)
			for(j=0;j<buckets;j++)
			lsh_hashtables[i][j]=LIST_create();
			
		// Ka8e dianysma katakermatizetai me ka8e g_i kai mpainei stoys antistoixous kadous
		for(i=0;i<num_of_vectors;i++){
			for(j=0;j<L;j++){
				
				// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
				bucket_number=HASH_g_i(g[j],vectors[i],m_powers,lsh_w,d,M,LSH_k,buckets);
				
				// Eisagwgh toy dianysmatos sto bucket poy proekypse 
				LIST_insert(lsh_hashtables[j][bucket_number],i+1);
			}	
		}
		
		// Twra metatrepoyme ta buckets apo listes se dynamikoys pinakes gia na beltiwsoyme 
		// thn taxythta.
		for(i=0;i<L;i++){
			for(j=0;j<buckets;j++){
				
				// Kataskeyh bucket apo th sxetikh lista
				bucket_ptr=LIST_convert_to_bucket(lsh_hashtables[i][j]);
				
				// Adeiasma listas
				LIST_empty(lsh_hashtables[i][j]);
				
				// Apeley8erwsh xwroy listas
				free(lsh_hashtables[i][j]);
				
				// Ekxwrhsh bucket sthn antistoixh 8esh
				lsh_hashtables[i][j]=bucket_ptr;
						
			}
		
		}

		// Desmeysh mnhmhs gia ton didiastato pinaka me ta buckets twn kentrwn (L buckets gia ka8ena apo ta K kentra)
		buckets_of_current_centers=malloc(K*sizeof(int*));
		for(i=0;i<K;i++)
			buckets_of_current_centers[i]=malloc(L*sizeof(int));
		
	}
	
	// Ean h me8odos einai HYPERCUBE, tote prin apo toys kykloys assignment - update prepei na ginoyn orismenes energeies.
	// Sygkekrimena prepei na kataskeyastei o pinakas g, o pinakas twn dynamewn toy m kai oi pinakes katakermatismoy.
	// Epishs prepei na eisax8oun ta dianysmata ston yperkybo.	
	else if(method==HYPERCUBE){
				
		// Dhmioyrgia synarthsewn katakermatismoy
		g=HASH_create_hash_functions(HYPERCUBE_k,cube_num_of_hi_functions,d,cube_w);
		 
		// Ypologizetai to M = 2^(32/cube_num_of_hi_functions)
		// Xrhsimo ston katakermatismo ka8e dianysmatos  me ka8e synarthsh h_i	
		M = 1;		
		for(i=0;i<32/cube_num_of_hi_functions;i++)
			M*=2;
			
		// Dhmioyrgia twn dynamewn toy cube_m
		m_powers=HASH_create_m_powers(d,cube_m,M);

		buckets=num_of_vectors/cube_divider;
		
		// Desmeyoyme mnhmh gia ton pinaka cube_hashtables poy ylopoiei toys pinakes katakermatismoy 
		cube_hashtables=malloc(HYPERCUBE_k*sizeof(int*));
		for(i=0;i<HYPERCUBE_k;i++)
			cube_hashtables[i]=malloc(buckets*sizeof(int));
		
		// Gia ka8e kado antistoixoyme tyxaia to 0 'h to 1
		for(i=0;i<HYPERCUBE_k;i++)
			for(j=0;j<buckets;j++)
				cube_hashtables[i][j]=rand()%2;
			
		// Ypologismos plh8ous koryfwn toy yperkyboy
		num_of_vertices=1;
		for(i=0;i<HYPERCUBE_k;i++)
			num_of_vertices*=2;
		
		// Dhmioyrgoyme ton yperkybo. Arxika se ka8e koryfh antistoixoyme mia lista 
		hypercube=malloc(num_of_vertices*sizeof(void*));
		for(i=0;i<num_of_vertices;i++)
			hypercube[i]=LIST_create();
		
		// Ka8e dianysma katakermatizetai me ka8e g_i kai ypologizetai h korryfh toy yperkyboy 
		// sthn opoia eisagetai
		for(i=0;i<num_of_vectors;i++){
			
			vertex_number=0;
			for(j=0;j<HYPERCUBE_k;j++){
				
				// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
				bucket_number=HASH_g_i(g[j],vectors[i],m_powers,cube_w,d,M,cube_num_of_hi_functions,buckets);
			
				// Sxhmatismos toy ari8moy koryfhs
				vertex_number+=vertex_number+cube_hashtables[j][bucket_number];
				
			}	
			// Eisagwgh dianysmatos sthn koryfh
			LIST_insert(hypercube[vertex_number],i+1);
			
		}
				
		// Twra metatrepoyme tis koryfes apo listes se buckets pou periexoyn dynamikoys pinakes 
		// gia na beltiwsoyme thn taxythta.
		for(i=0;i<num_of_vertices;i++){
			
			// Kataskeyh bucket apo th sxetikh lista
			bucket_ptr=LIST_convert_to_bucket(hypercube[i]);
			
			// Adeiasma listas
			LIST_empty(hypercube[i]);
			
			// Apeley8erwsh xwroy listas
			free(hypercube[i]);
			
			// Ekxwrhsh bucket sthn antistoixh 8esh
			hypercube[i]=bucket_ptr;
				
		} 
		
		
		// Desmeysh xwroy gia ton pinaka pou 8a apo8hkeysei proswrina tis hamming apostaseis mias koryfhs 
		// apo tis ypoloipes
		hamming_distances =malloc(num_of_vertices*sizeof(int));
		
		// Desmeysh xwroy gia ton pinaka pou 8a apo8hkeysei proswrina tous ari8moys twn koryfwn
		vertex_numbers = malloc(num_of_vertices*sizeof(int));
		
		// Oi dyo aytoi pinakes einai 1 pros 1. Taksinomontas ton prwto kai kratwntas thn 1 pros 1 sxesh me 
		// ton deytero diatassoyme tis koryfes symfwna me tis apostaseis hamming apo sygkekrimenh koryfh
		
		// Desmeysh xwroy gia ton disdiastato pinakas ka8e grammh toy opoioy antistoixei se mia koryfh 
		// kai  periexei taksinomhmenes HYPERCUBE_probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
		probes_array=malloc(num_of_vertices*sizeof(int*));
		
		for(i = 0; i < num_of_vertices; ++i)
			probes_array[i] = malloc((HYPERCUBE_probes+1)*sizeof(int));
		
		for(vertex=0;vertex<num_of_vertices;vertex++){
				
			// Arxikopoihsh twn pinakwn CUBE_hammings kai vertex_ids
			for(j=0;j<num_of_vertices;j++){
				
				if(vertex!=j)
					hamming_distances[j]=CUBE_hamming_distance(vertex,j,num_of_vertices);
				else
					hamming_distances[j]= 0;
				
				vertex_numbers[j]=j;
				
			}
			
			// Taksinomoume ton pinaka hamming_distances me ayksousa seira diathrwntas thn 
			// 1-1 sxesh me ton vertex_numbers (Mono oi HYPERCUBE_probes to plh8os 8eseis gia oikonomia)	
			for(i=1;i<=HYPERCUBE_probes+1;i++){
				
				for(j=num_of_vertices-1;j>=i;j--){
					
					if(hamming_distances[j]<hamming_distances[j-1]){
						
						hamming_distances[j] = hamming_distances[j]+hamming_distances[j-1];
						hamming_distances[j-1] = hamming_distances[j]-hamming_distances[j-1];
						hamming_distances[j] = hamming_distances[j]-hamming_distances[j-1];
						
						vertex_numbers[j] = vertex_numbers[j]+vertex_numbers[j-1];
						vertex_numbers[j-1] = vertex_numbers[j]-vertex_numbers[j-1];
						vertex_numbers[j] = vertex_numbers[j]-vertex_numbers[j-1];
						
					}
				}
			}
			
			// Telos apothikeyoyme toys ypologismoys ston pinaka probes_array	
			for(i=0;i<HYPERCUBE_probes+1;i++)
				probes_array[vertex][i]=vertex_numbers[i];
				
		}
			
		// Desmeyetai mnhmh gia ton pinaka me ta vertices twn kentrwn
		vertices_of_current_centers=malloc(K*sizeof(int));
					
	}
	
	//************ Faseis assign - update (epanalhptika) ************//
	iterations=0;
	while(1){
		
		iterations++;
		
		//************ Fash assign ************//	
		
		// Periptwsh assign me lloyd
		if(method==CLASSIC){
			
			ASSIGN_lloyd(vectors,num_of_vectors,METRIC_L2,cluster_center_of_vector,distance_from_cluster_center,cluster_centers,K,d,clusters);
		
		}	
		// Periptwsh assign me reverse R me lsh
		else if(method==LSH){
			
			ASSIGN_reverse_R_lsh(vectors,num_of_vectors,METRIC_L2,lsh_hashtables,cluster_center_of_vector,distance_from_cluster_center, cluster_centers,buckets_of_current_centers,g,m_powers,L,K,d,lsh_w,M,LSH_k,buckets,clusters);
		
		}	
		// Periptwsh assign me reverse R me hypercube
		else if(method==HYPERCUBE){
			
			ASSIGN_reverse_R_hypercube(vectors,num_of_vectors,METRIC_L2,hypercube,cube_hashtables,cluster_center_of_vector,distance_from_cluster_center, cluster_centers,vertices_of_current_centers,HYPERCUBE_probes,probes_array,g,m_powers,K,d,cube_w,M,HYPERCUBE_k, cube_num_of_hi_functions, buckets,clusters);
		
		}
			
		//************ Fash update ************//
				
		num_of_center_changes=UPDATE_center(vectors,num_of_vectors,METRIC_L2, cluster_centers,K,d,clusters);

		// An o kyklos den odhghse se kapoia allagh kentroy, exoyme sygklish kai to programma termatizei
		if(num_of_center_changes==0)
			break;
		// An o ftasame tis megistes epanalhpseis, to programma termatizei
		if(iterations==max_iterations)
			break;
		else{
		// Diaforetika ka8arizeitai to cluster kai ksekinaei neos kyklos
			for(i=0;i<K;i++){
				free(((bucket*)clusters[i])->keys);
				free(clusters[i]);
				clusters[i]=LIST_create();
			}
			
		}
		
			
	}
	
	// Lhksh xronometroy 
	end = clock();
	
	//************ Fash ektypwshs apotelesmatwn ************//
	
	// Anoigma arxeioy eksodoy
	fp=fopen(output_file,"w");
	
	
	// Grapsimo sto arxeo sto zhtoymeno format
	fprintf(fp,"Algorithm: ");
	
	if(method==CLASSIC)
		fprintf(fp,"Lloyds\n");
	else if(method==LSH)
		fprintf(fp,"Range Search LSH\n");	
	else if(method==HYPERCUBE)
		fprintf(fp,"Range Search Hypercube\n");
	
	
	for(i=0;i<K;i++){
		
		fprintf(fp,"CLUSTER-%d { size: %d, cendroid: ",i,((bucket*)clusters[i])->length);
		vct = vectors[cluster_centers[i]-1];
		for(j=0;j<d;j++)			
			fprintf(fp,"%d ",vct[j]);			
		fprintf(fp,"}\n");
		
		
	}
	
	fprintf(fp,"Clustering_time: %lf\n",(double)(end - start) / CLOCKS_PER_SEC);
	
	
	//************ Ypologismos kai aksiologhsh silhouete ************//
	
	s_array=malloc(d*sizeof(double));
	
	fprintf(fp,"Silhouette:[");
	
	stotal=0;
	// Gia ka8e cluster...
	for(cluster_num=0;cluster_num<K;cluster_num++){
		
		current_cluster=((bucket*)clusters[cluster_num])->keys;
		
		current_cluster_size=((bucket*)clusters[cluster_num])->length;
				
		// ..gia ka8e dianysma toy cluster..
		for(i=0;i<current_cluster_size;i++){
			
			
			sum_vector_si=0;
			
			// ..ypologizoyme to kontinotero geitoniko kentro
			distance_of_best_neihbour=DBL_MAX;
			
			for(j=0;j<K;j++){
				
				if(j!=cluster_num){
					
					distance=METRIC_L2(vectors[current_cluster[i]-1],vectors[cluster_centers[j]-1],d);
					
					if(distance<distance_of_best_neihbour){
						
						distance_of_best_neihbour=distance;
						neihbour_cluster_num=j;
						
					}					
				}				
			}
			
			// Ypologizoyme th mesh apostash toy dianysmatos apo ta dianysmata toy cluster 
			sum_distance=0;
			for(j=0;j<current_cluster_size;j++){
				
				distance=METRIC_L2(vectors[current_cluster[i]-1],vectors[current_cluster[j]-1],d);
				sum_distance+=distance;
				
			}
			
			neihbour_cluster=((bucket*)clusters[neihbour_cluster_num])->keys;
			neihbour_cluster_size=((bucket*)clusters[neihbour_cluster_num])->length;
			
			// Ypologizoyme th mesh apostash toy dianysmatos apo ta dianysmata toy geitonikoy cluster 
			sum_distance_to_neihbours=0;
			for(j=0;j<neihbour_cluster_size;j++){
				
				distance=METRIC_L2(vectors[current_cluster[i]-1],vectors[neihbour_cluster[j]-1],d);
				sum_distance_to_neihbours+=distance;
				
			}
			
			a_i=sum_distance/(current_cluster_size-1);
			b_i=sum_distance_to_neihbours/neihbour_cluster_size;

			vector_si=1-a_i/b_i;
			if(a_i>b_i)
				vector_si=-vector_si;
			
			sum_vector_si+=vector_si;
			
		}
		
		s_i=sum_vector_si/current_cluster_size;
		
		fprintf(fp,"%lf,",s_i);
		
		stotal+=s_i;
		
	}
	stotal/=K;
	fprintf(fp,"%lf]\n",stotal);
		
	free(s_array);
	
	
	// Se periptwsh poy exei do8ei shmaia complete typwnontai kai ta dianysmata ekastoy cluster	
	if(complete){
		
		for(i=0;i<K;i++){
			
			cendroid=cluster_centers[i];
			
			fprintf(fp,"CLUSTER-%d { %d",i,cendroid);
			
			cluster_size = ((bucket*)clusters[i])->length;
			vct = ((bucket*)clusters[i])->keys;
			
			for(j=0;j<cluster_size;j++)
				if(vct[j]!=cendroid)
					fprintf(fp,",%d ",vct[j]);			
			fprintf(fp,"}\n");
						
		}
				
	}	
			
	
	fclose(fp);
	// Kleisimo arxeioy eksodoy //
	
	//************ Fash ka8arismoy mnhmhs ************//	
		
	// Ka8arismos mnhmhs

	if(method==LSH){
					
		// Apeley8erwsh synarthsewn katakermatismoys
		HASH_free_hash_functions(g,L,LSH_k);
	
		// Apeley8erwsh pinaka m-dianysmatos
		free(m_powers);	
		
		// Apeley8erwsh mnhmhs twn hashtables
		for(i=0;i<L;i++){
			for(j=0;j<buckets;j++){
				
				free(((bucket*)(lsh_hashtables[i][j]))->keys);
				free(lsh_hashtables[i][j]);
				
			}
			free(lsh_hashtables[i]);		
			
		}
		free(lsh_hashtables);	
		
		for(i=0;i<K;i++)
			free(buckets_of_current_centers[i]);
		free(buckets_of_current_centers);		
	
	}
	else if(method==HYPERCUBE){
		
		// Apeley8erwsh mnhmhs  pinaka me ta vertices twn kentrwn
		free(vertices_of_current_centers);
		
		// Apeley8erwsh mnhmhs pinaka me tis apostaseis hamming
		free(hamming_distances);
	
		//  Apeley8erwsh mnhmhs pinaka pou apo8hkeyame proswrina tous ari8moys twn koryfwn
		free(vertex_numbers);
	
		// Apeley8erwsh mnhmhs disdiastatoy pinaka ka8e grammh toy opoioy antistoixoyse se mia koryfh 
		// kai  perieixe taksinomhmenes probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
		for(i=0;i<num_of_vertices;++i)
			free(probes_array[i]);
		free(probes_array);
				
		// Apeley8erwsh mnhmhs toy yperkyboy.
		for(i=0;i<num_of_vertices;i++){
			free(((bucket*)(hypercube[i]))->keys);
			free((bucket*)(hypercube[i]));
		}
		free(hypercube);
	
		// Apeley8erwsh synarthsewn katakermatismoys
		HASH_free_hash_functions(g,HYPERCUBE_k,cube_num_of_hi_functions);
		
		// Apeley8erwsh mnhmhs toy pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
		for(i=0;i<HYPERCUBE_k;i++)
			free(cube_hashtables[i]);	
		free(cube_hashtables);
		
		// Apeley8erwsh pinaka m-dianysmatos
		free(m_powers);
		
	}
	
	// Apeley8erwnetai h mnhmh me ta dianysmata
	for(i=0;i<num_of_vectors;i++)
		free(vectors[i]);
	free(vectors);
	
		
	// Apeley8erwsh mnhmhs toy pinaka poy periexei gia ka8e dianysma to kentro toy cluster sto opoio anhkei
	free(cluster_center_of_vector);
	
		
	// Apeley8erwsh mnhmhs toy pinaka me tis apostaseis twn dianysmatwn apo ta antisoixa kentra twn klasewn toys 
	free(distance_from_cluster_center);
	
	// Apeley8erwnetai h mnhmh me ta kentra
	free(cluster_centers);
		
	// Apeley8erwnetai h mnhmh twn clusters
	for(i=0;i<K;i++){
		free(((bucket*)clusters[i])->keys);
		free(clusters[i]);
	}
	free(clusters);
	
	exit(0);
	
}












