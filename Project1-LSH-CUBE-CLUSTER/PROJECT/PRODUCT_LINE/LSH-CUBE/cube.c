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

#define CUBE_k 4
#define CUBE_M 5
#define CUBE_N 1
#define CUBE_R 10000
#define CUBE_probes 2
#define CUBE_CONFIG "cube.conf"

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


int main(int argc, char* argv[]){
	
	// Parametropoihsh metrikhs
	double (*metric)(int*,int*,int) = METRIC_L2;
		
	// Diadromh arxeioy eisodoy dianysmatwn  
	char* input_file;
	 
	// Diadromh arxeioy eperwthsewn  
	char* query_file;
	
	// Diadromh arxeioy apotelesmatwn 
	char* output_file;	
	
	// To k edw einai h diastash toy yperkyvou dhladh to plh8os twn alplified synarthsewn katakermatismoy g_i
	int k;
	
	// Plh8os plhsiesterwn geitonwn
	int N;
	
	// Aktina plhsiesterwn geitonwn
	double R;
	
	// Plh8os geitonikwn twn koryfwn toy yperkybou pou elegxontai 
	int probes;
	
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
	
	// To megisto plh8os dianysmatwn poy elegxontai. Dinetai apo th grammh entolwn ws timh pou
	// akolou8ei th shmaia -M. To symbolizoume me M_arg gia na to ksexwrizoyme apo to gnwsto M
	// pou xrhsimopoihsame kai ston LSH
	int M_arg;
	
	// To plh8os twn hi synarthsewn poy sygrotoun mia gi
	int num_of_hi_functions;
	
	// Symvolizoyme me M thn posothta 2^(32/num_of_hi_functions). Xrhsimh ston katakermatismo ka8e dianysmatos 
	// me ka8e synarthsh h_i
	int M;
	
	register double*** g;
	// Trisdiastatos pinakas ton opoio voleyei na vlepoume ws disdiastato pinaka deiktwn typoy double.
	// Apo aythn thn optikh gwnia ka8e grammh toy pinaka antistoixei se mia synarthsh katakermatismoy g_i
	// enw ka8e keli antistoixei se mia synarthsh h_i.
	// Synepws exoyme k grammes toy pinaka afoy exoyme k synarthseis katakermatismoy g_i kai 
	// num_of_hi_functions sthles afoy ka8e synarthsh katakermatismoy g_i apoteleitai apo num_of_hi_functions
	// to plh8os synarthseis katakermatismoy h_i.
	// Ka8e synarthsh hi ekfrazetai san dianysma me d to plh8os syntetagmenes.
	// Oi syntetagmenes aytes epilegontai tyxaia sto diasthma (0, w] opoy to w 8ewreitai gnwsto.  
	
	// Pinakas dynamewn toy m
	register int* m_powers;
			
	register int** hashtables;
	// Disdiastatos me k grammes (mia gia ka8e synarthsh katakermatismoy g_i) oses kai oi g_i
	// kai sthles oses kai to plh8os twn kadwn (poy ypologizetai diairwntas to plh8os twn dianysmatwn
	// me enan proka8orismeno ari8mo-diaireth). 
	// Ta stoixeia einai eite mhden eite 1. Ka8ena eks' aytwn antiproswpeyei enan kado.
	// O kpshfios dyadikos ari8mos poy sxhmatizetai apo ton katakermatismo enos dianysmatos me ka8e gi 
	// dinei thn koryfh toy yperkyboy poy eisagetai.
	
	// H parametros w th 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
	double w;
	
	// O ari8mos bucket enos dianysmatos, opw prokyptei apo ton katakermatismo toy 
	// me mia g_i
	int bucket_number;
	
	// Deikths se bucket
	bucket* bucket_ptr;
		
	// Ari8mos koryfhs yperkyboy (Boh8htikh metablhth)
	int vertex;
	
	// Metrhths koryfwn (Boh8htikh metablhth)
	int vertex_counter;
		
	// Pinakas pou 8a apo8hkeyei proswrina tis hamming apostaseis mias koryfhs apo tis ypoloipes
	int* hamming_distances;
	
	// Pinakas pou 8a apo8hkeysei proswrina tous ari8moys twn koryfwn
	int* vertex_numbers;
	
	// Oi dyo aytoi pinakes einai 1 pros 1. Taksinomontas ton prwto kai kratwntas thn 1 pros 1 sxesh me 
	// ton deytero diatassoyme tis koryfes symfwna me tis apostaseis hamming apo sygkekrimenh koryfh
	
	// Disdiastatos pinakas ka8e grammh toy opoioy antistoixei se mia koryfh kai  periexei taksinomhmenes 
	// probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
	// O en logw pinakas dieykolynei th grhgorh eyresh twn geitonikoterwn koryfwn 
	int** probes_array;
	
	// Ginontai typikes arxikopoihseis (kyriws gia na mh bgazei warnings o metaglwttisths)

	// Pinakas apostasewn
	double* brutal_search_distances;
	
	// O ari8mos koryfhs sthn opoia eisagetai ena dianysma
	int vertex_number;
	
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
	double hypercube_time, brutal_search_time;
	
	// Metablhtes ypologismoy xronoy
	struct timespec start, stop;

	// Elaxistes apostaseis
	double n_distance;
	int n_position;
	
	// Deikths sto arxeio configure toy HYPERCUBE
	FILE* fp_config;
	
	// Epistrefomeno ths fscanf (kyriws gia na mh bgazei warnings o metaglwttisths)
	int fscanf_result;
	
	// Boh8htikh metablhth
	int num_of_vectors_in_vertex;
	
	// O yperkybos
	void** hypercube;
	
	// To plh8os twn koryfwn toy
	int num_of_vertices;
	
	// Pinakas o opoios periexei to plh8os twn dianysmatwn poy elegxontai apo ka8e koryfh
	int* vectors_to_check;
	
	// Oi ari8moi twn dianysmatwn poy elegxontai
	int* checking_vectors;
	
	// Oi antistoixes apostaseis
	double* checked_vector_distances;
	
	// Diavasma orismatwn
	if(ARGS_get_cube_args(argc,argv,&input_file,&query_file,&k,&M_arg,&output_file,&N,&R,&probes,CUBE_k,CUBE_M,CUBE_N,CUBE_R,CUBE_probes)<0)
		exit(-1);
	
	// Anoigma toy arxeiou configure
	fp_config=fopen(CUBE_CONFIG,"r");
	
	// Se periptwsh apotyxias to programma kanei eksodo 
	if(fp_config==NULL){
		
		printf("Can not open configure file!\n");
		exit(-1);
		
	}
		
	// Diavasma toy m ths parametropoihshs twn h_i synarthsewn
	fscanf_result=fscanf(fp_config,"%u",&m);
		
	// Diavasma toy diaireth poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
	fscanf_result=fscanf(fp_config,"%d",&divider);
	
	// Ean einai mhden to programma termatizei
	if(divider<=0){
		
		printf("Invilid divider!\n");
		exit(-1);
		
	}
	
	// Diavasma ths parametroy w ths 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
	fscanf_result=fscanf(fp_config,"%lf",&w);
		
	// Se periptwsh poy apotyxei to diavsma to programma epistrefei
	if(fscanf_result==0){
		
		printf("Error in reading configure fille!\n");
		exit(-1);		
		
	}
			
	// Diavasma toy plh8ous twn h_i synarthsewn
	fscanf_result=fscanf(fp_config,"%d",&num_of_hi_functions);
	
	// Kleisimo arxeioy configure
	fclose(fp_config);
	
	// Se periptwsh esfalmenoy plh8ous (mhdenikoy 'h arnhtikoy) to programma kanei eksodo 
	if(num_of_hi_functions<=0){
		
		printf("Number of hi functions must be a positive integer!\n");
		exit(-1);
		
	}	
	
	// Diavasma dianysmatwn apo arxeio
	VECTOR_read_vectors_from_file(input_file,&vectors,&num_of_vectors,&d);

	// Dhmioyrgia synarthsewn katakermatismoy
	g=HASH_create_hash_functions(k,num_of_hi_functions,d,w);
	
	// Ypologizetai to M = 2^(32/num_of_hi_functions)
	// Xrhsimo ston katakermatismo ka8e dianysmatos  me ka8e synarthsh h_i	
	M = 1;		
	for(i=0;i<32/num_of_hi_functions;i++)
		M*=2;
		
	// Dhmioyrgia twn dynamewn toy m
	m_powers=HASH_create_m_powers(d,m,M);

	buckets=num_of_vectors/divider;

	// Desmeyoyme mnhmh gia ton pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
	hashtables=malloc(k*sizeof(int*));
	for(i=0;i<k;i++)
		hashtables[i]=malloc(buckets*sizeof(int));
	
	// Gia ka8e kado antistoixoyme tyxaia to 0 'h to 1
	for(i=0;i<k;i++)
		for(j=0;j<buckets;j++)
			hashtables[i][j]=rand()%2;
		
	// Ypologismos plh8ous koryfwn toy yperkyboy
	num_of_vertices=1;
	for(i=0;i<k;i++)
		num_of_vertices*=2;
	
	// Dhmioyrgoyme ton yperkybo. Arxika se ka8e koryfh antistoixoyme mia lista 
	hypercube=malloc(num_of_vertices*sizeof(void*));
	for(i=0;i<num_of_vertices;i++)
		hypercube[i]=LIST_create();
	
	// Ka8e dianysma katakermatizetai me ka8e g_i kai ypologizetai h korryfh toy yperkyboy 
	// sthn opoia eisagetai
	for(i=0;i<num_of_vectors;i++){
		
		vertex_number=0;
		for(j=0;j<k;j++){
			
			// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
			bucket_number=HASH_g_i(g[j],vectors[i],m_powers,w,d,M,num_of_hi_functions,buckets);
		
			// Sxhmatismos toy ari8moy koryfhs
			vertex_number+=vertex_number+hashtables[j][bucket_number];
			
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
	// kai  periexei taksinomhmenes probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
	probes_array=malloc(num_of_vertices*sizeof(int*));
	
	for(i = 0; i < num_of_vertices; ++i)
		probes_array[i] = malloc((probes+1)*sizeof(int));
	
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
		// 1-1 sxesh me ton vertex_numbers (Mono oi probes to plh8os 8eseis gia oikonomia)	
		for(i=1;i<=probes+1;i++){
			
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
		
		// Telos apothikeyoyme toys ypologismoyσ ston pinaka probes_array	
		for(i=0;i<probes+1;i++)
			probes_array[vertex][i]=vertex_numbers[i];
		
	}
			
	// Diavasma query-dianysmatwn apo arxeio
	VECTOR_read_vectors_from_file(query_file,&queries,&num_of_queries,&d_q);
	
	// Elegxos ean sympiptoyn oi diastaseis (prepei na sympiptoyn)
	if(d_q!=d){
		
		printf("Diamensions of query vectors are different from input vectors!\n");
		
		VECTOR_free_vectors(vectors,num_of_vectors,d);
		VECTOR_free_vectors(queries,num_of_queries,d_q);
	
		HASH_free_hash_functions(g,k,num_of_hi_functions);
		
		exit(-1);
		
	}
		
	// Desmeysh pinaka o opoios periexei to plh8os twn dianysmatwn poy elegxontai apo ka8e koryfh
	vectors_to_check=malloc((probes+1)*sizeof(int));
	
	// Desmeysh pinaka o opoios periexei toys ari8moys twn dianysmatwn poy elegxontai
	checking_vectors=malloc(M_arg*sizeof(int));
	
	// Desmeysh pinaka o opoios periexei tis antistoixes apostaseis
	checked_vector_distances=malloc(M_arg*sizeof(double));
	
	// To moirasma ginetai isomerws me kapoies ek twn kontinoterwn koryfwn na dinoyn isws ena dianysma
	// pararapanw (analoga me th diairesh toy synolikoy plh8oys twn eksetazomenwn dianysmatwn dia 
	// to plh8os twn eksetazomenwn koryfwn)
	for(i=0;i<probes+1;i++)
		vectors_to_check[i]=M_arg/(probes+1);
	
	for(i=0;i<M_arg%(probes+1);i++)
		vectors_to_check[i]++;
			
	// Desmeysh pinaka me toys ayksontes ari8moys dianysmatwn
	vector_numbers=malloc(num_of_vectors*sizeof(int));

	// Desmeysh pinaka me tis apostaseis enos query dianysmatos apo ta ypoloipa 
	brutal_search_distances=malloc(num_of_vectors*sizeof(double));

	// Anoigma arxeioy eksodoy
	fp = fopen(output_file, "w");
		
	// Ka8e query katakermatizetai me ka8e g_i kai ypologizetai h korryfh toy yperkyboy 
	// sthn opoia antistoixei
	for(q=0;q<num_of_queries;q++){
		
		// Lhksh xronometroy xronoy hypercube
		clock_gettime(CLOCK_REALTIME,&start);
		
		vertex_number=0;
		for(j=0;j<k;j++){
			
			// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
			bucket_number=HASH_g_i(g[j],queries[q],m_powers,w,d,M,num_of_hi_functions,buckets);
		
			// Sxhmatismos toy ari8moy koryfhs
			vertex_number+=vertex_number+hashtables[j][bucket_number];
			
		}	
	
		vertex_counter=0;
		
		// Vriskoyme ta dianysmata pou 8a elegx8oun
		for(i=0;i<probes+1;i++){
			
			//printf("Checking %d vectors in vertex %d\n",vectors_to_check[i],probes_array[vertex_number][i]);
			num_of_vectors_in_vertex=((bucket*)(hypercube[probes_array[vertex_number][i]]))->length;
			for(j=0;j<vectors_to_check[i];j++){
				
				// An teleiwsan ta dianysmata sthn koryfh, kanoyme break kai pame sthn epomenh
				if(j==num_of_vectors_in_vertex)
					break;
				
				checking_vectors[vertex_counter++]=((bucket*)(hypercube[probes_array[vertex_number][i]]))->keys[j];
								
			}
			
		}
		
		// Ypologismos apostasewn toy trexontos query me ka8ena apo ta dianysmata poy orizei o hypercube
		for(i=0;i<vertex_counter;i++)			
			checked_vector_distances[i]=metric(vectors[checking_vectors[i]-1],queries[q],d);
		
		// Taksinomhsh twn N dianysmatwn me th mikroterh apostash apo to dianysma-query		
		for(i=1;i<=N;i++){
			
			for(j=N-1;j>=i;j--){
				
				if(checked_vector_distances[j]<checked_vector_distances[j-1]){
					
					temp_double=checked_vector_distances[j];
					checked_vector_distances[j]=checked_vector_distances[j-1];
					checked_vector_distances[j-1]=temp_double;
					
					temp_int=checking_vectors[j];
					checking_vectors[j]=checking_vectors[j-1];
					checking_vectors[j-1]=temp_int;			
					
				}				
			}			
		}		
		
		// Lhksh xronometroy xronoy hypercube
		clock_gettime(CLOCK_REALTIME,&stop);

		// Ypologismos xronoy hypercube
		hypercube_time = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec);
		
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

		// Ypologismos xronoy eksantlhtikhs anazhthshs	
		brutal_search_time = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec - start.tv_nsec);
				
		fprintf(fp,"Query id: %d\n", q+1);
		
		// Eggrafh apotelesmatwn
		for(i=0;i<N;i++){
			
			fprintf(fp,"Nearest neighbor-%d: %d\n",i+1,checking_vectors[i]);
			fprintf(fp,"distanceCUBE: %.6lf\n",checked_vector_distances[i]);
			fprintf(fp,"distanceTrue: %.6lf\n",brutal_search_distances[i]);
		}
		fprintf(fp,"tCUBE: %.3lf microseconds\n",hypercube_time/1000);
		fprintf(fp,"tTrue: %.3lf microseconds\n",brutal_search_time/1000);	
		
		// Emfanish twn R-nearest neighbors (exoyme hdh apo8hkeymenes tis apostaseis)
		fprintf(fp,"%.3lf-nearest neighbors:\n",R);		
		for(i=0;i<vertex_number;i++){
			
			if(checked_vector_distances[i]<R){
				
				fprintf(fp,"%d\n",checking_vectors[i]);
				
			}			
			
		}
		fprintf(fp,"\n");
			
	}	
		
	// Kleisimo arxeioy eksodoy
	fclose(fp);	
	
	// Apeley8erwsh mnhmhs pinaka me tis apostaseis hamming
	free(hamming_distances);
	
	//  Apeley8erwsh mnhmhs pinaka pou apo8hkeyame proswrina tous ari8moys twn koryfwn
	free(vertex_numbers);
	
	// Apeley8erwsh mnhmhs disdiastatoy pinaka ka8e grammh toy opoioy antistoixoyse se mia koryfh 
	// kai  perieixe taksinomhmenes probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
	for(i=0;i<num_of_vertices;++i)
		free(probes_array[i]);
	free(probes_array);
	
	// Apeley8erwsh mnhmhs pinaka me tis apostaseis opw prokyptoyn apo eksantlhtikh anazhthsh
	free(brutal_search_distances);

	// Apeley8erwsh mnhmhs pinaka ari8mwn dianysmatwn
	free(vector_numbers);	
	
	// Apeley8erwsh mnhmhs toy yperkyboy.
	for(i=0;i<num_of_vertices;i++){
		free(((bucket*)(hypercube[i]))->keys);
		free((bucket*)(hypercube[i]));
	}
	free(hypercube);
		
	// Apeley8erwsh mnhmhs pinaka o opoios periexei to plh8os twn dianysmatwn poy elegxontai apo ka8e koryfh
	free(vectors_to_check);
	
	// Apeley8erwsh mnhmhs pinaka o opoios periexei toys ari8moys twn dianysmatwn poy elegxontai
	free(checking_vectors);
	
	// Apeley8erwsh mnhmhs pinaka o opoios periexei tis antistoixes apostaseis
	free(checked_vector_distances);
	
	// Apeley8erwsh pinakwn dianysmatwn eisodoy kai dianysmatwn queries
	VECTOR_free_vectors(vectors,num_of_vectors,d);
	VECTOR_free_vectors(queries,num_of_queries,d);
	
	// Apeley8erwsh synarthsewn katakermatismoys
	HASH_free_hash_functions(g,k,num_of_hi_functions);
	
	// Apeley8erwsh mnhmhs toy pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
	for(i=0;i<k;i++)
		free(hashtables[i]);	
	free(hashtables);
	
	// Apeley8erwsh pinaka m-dianysmatos
	free(m_powers);

	exit(0);
	
}

