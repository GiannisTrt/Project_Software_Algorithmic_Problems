#include <iostream>
#include <random>
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <chrono>
using namespace std::chrono;

#include "constants.h"
#include "vector.h"
#include "metrics.h"
#include "list.h"
#include "hash.h"
#include "bucket.h"
#include "search_args.h"
#include "hamming.h"
#include "curve.h"

#include "config.hpp" 
#include "curve.hpp" 
#include "frechet.hpp" 
#include "interval.hpp" 
#include "point.hpp" 
#include "simplification.hpp"



int main(int argc, char* argv[]){
	
	
	// Parametropoihsh metrikhs
	double (*distance_core_metric)(double*,double*,int) = METRIC_L2;
	
	// To arxeio eisodoy
	char* input_file;
	
	// To arxeio eperwthsewn
	char* query_file;
		
	// To arxeio configuration
	char* configuration_file;
	
	// To arxeio eksodoy
	char* output_file;
	
	// H metrikh (mono gia frechet)
	int metric;
		
	// H timh delta (mono gia frechet)
	double delta;
	
	// H shmaia complete
	int complete;	
	
	// O ari8mos twn probes
	int probes;
	
	// To k toy lsh 'h toy hypercube
	int k;
	
	// O algori8mos (ypo th morfh kwdikoy)
	int algorithm;
	
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
			
	// Dynamikos pinakas me toys ari8moys buckets poy psaxnei o lsh 
	int* search_bucket_numbers;
	
	// Dynamikos pinakas me toys ari8modeiktes sygxwneyshs twn buckets 
	int* search_bucket_indeces;
	
	// Dynamikos pinakas me ta buckets poy psaxnei o lsh 
	bucket** search_buckets; 
			
	// Megethos pinaka sygxwneyshs kadwn
	int merge_bucket_length;
	
	// Pinakas kampylwn eisodoy opws diavazontai gia discrete frechet
	double** discrete_frechet_curves;
			
	// Pinakas dianysmatwn eisodoy
	double** vectors;
		
	// Pinakas onomatwn dianysmatwn eisodoy
	char** vector_names;
	
	// Pinakas onomatwn kamlypwn eisodoy
	char** curve_names;
	
	// Plh8os kamlypwn eisodoy
	int	num_of_curves;
	
	// Pinakas query-dianysmatwn 
	double** queries;
		
	// Pinakas onomatwn query-dianysmatwn 
	char** query_names;
	
	// Plh8os query-dianysmatwn
	int num_of_queries;
	
	// Pinakas queries sthn periptwsh toy discrete frechet
	double** discrete_frechet_queries;
	
	// Boh8htikh metablhth
	int* vct;
	
	// Deikths arxeioy eksodoy
	FILE* fp;
	
	// O parakatw pinakas einai 1 pros 1 me tis grammes toy pinaka vectors
	// Ka8e stoixeio antistoixei se ena dianysma kai periexei to kentro tou cluster sto opoio anhkei to dianysma 
	int* cluster_center_of_vector;
	
	// Oi apostaseis twn dianysmatwn apo ta antisoixa kentra twn klasewn toys (pinakas)
	double* distance_from_search_center;
	
	// Plh8os dianysmatwn eisodoy
	int num_of_vectors;
	
	// Diastash dianysmatwn eisodoy
	int d;
	
	// Boh8htikoi ari8modeiktes
	register int i;
	register int j;
	register int v;
	register int q;

	// Dinontai oi defaut times alla aytes mporoyn na allaksoyn apo sxetiko arxeio
	double bigM = BIG_M;
	double epsilon = EPSILON;
		
	// Boh8htikh metablhth
	int num_of_vectors_in_vertex;
	
	// Ta clusters ylopoioyntai san pinakes apo listes
	list** clusters;
	
	// To m toy lsh
	unsigned int lsh_m;
	
	// Pinakas me ta kentra twn klasewn
	int* cluster_centers;
	
	// To w toy lsh
	double lsh_w;
	
	// To w toy hypercube
	double cube_w;
	
	// O diaireths poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
	int divider;
	
	// Epistrefomeno ths fscanf (kyriws gia na mh bgazei warnings o metaglwttisths)
	int fscanf_result;
	
	// Shmaia periorismoy h mh ston ari8mo twn elegxomenwn apo thn lsh dianysmatwn 
	int reduce_search;
	
	// Pollaplasiasths toy L poy dinei to megisto ari8mo dianysmatwn sthn periptwsh periorismoy 
	// twn elegxomenwn apo thn lsh dianysmatwn 
	int reducing_multiplier;
	
	// Boh8htikes metablhtes
	int current_center_index;
	int current_vector_index;
	register double*** g;
	
	// Trisdiastatos pinakas ton opoio voleyei na vlepoume ws disdiastato pinaka deiktwn typoy double.
	// Apo aythn thn optikh gwnia ka8e grammh toy pinaka antistoixei se mia synarthsh katakermatismoy g_i
	// enw ka8e keli antistoixei se mia synarthsh h_i.
	// Synepws, ston LSH exoyme L grammes toy pinaka afot exoyme L synarthseis katakermatismoy g_i kai k sthles afoy ka8e 
	// synarthsh katakermatismoy g_i apoteleitai apo k to plh8os synarthseis katakermatismoy h_i.
		
	register double** h;
	// Disdiastatos pinakas ton opoio voleyei na vlepoume ws monodiastato pinaka deiktwn typoy double.
	// Apo aythn thn optikh gwnia ka8e keli antistoixei se mia synarthsh h_i.
	// Synepws exoyme k grammes toy pinaka afoy exoyme k synarthseis katakermatismoy g_i kai 
	// Ka8e synarthsh hi ekfrazetai san dianysma me d to plh8os syntetagmenes.
	// Oi syntetagmenes aytes epilegontai tyxaia sto diasthma (0, w] opoy to w 8ewreitai gnwsto.  
	
	// Ston HYPERCUBE exoyme k grammes toy pinaka afoy exoyme k synarthseis katakermatismoy g_i kai 
	// cube_num_of_hi_functions sthles afoy ka8e synarthsh katakermatismoy g_i apoteleitai apo cube_num_of_hi_functions
	// to plh8os synarthseis katakermatismoy h_i.
	// Ka8e synarthsh hi ekfrazetai san dianysma me d to plh8os syntetagmenes.
	// Oi syntetagmenes aytes epilegontai tyxaia sto diasthma (0, W] opoy to W 8ewreitai gnwsto.  
	
	// Pinakas timwn r
	register int* r_values;
	
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
	
	
	register int** cube_hashtables;
	// Disdiastatos me k grammes (mia gia ka8e synarthsh katakermatismoy h_i) oses kai oi h_i
	// kai sthles oses kai to plh8os twn kadwn (poy ypologizetai diairwntas to plh8os twn dianysmatwn
	// me enan proka8orismeno ari8mo-diaireth). 
	// Ta stoixeia einai eite mhden eite 1. Ka8ena eks' aytwn antiproswpeyei enan kado.
	// O kpshfios dyadikos ari8mos poy sxhmatizetai apo ton katakermatismo enos dianysmatos me ka8e gi 
	// dinei thn koryfh toy yperkyboy poy eisagetai.
	
	// Symvolizoyme me M thn posothta 2^(32/k). Xrhsimh ston katakermatismo ka8e dianysmatos 
	// me ka8e synarthsh h_i
	int M_LSH;
	
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
	
	// To megisto plh8os dianysmatwn poy elegxontai ston yperkybo
	int M;
	
	// O divider toy hypercube opws ton eidame sto prwto meros
	int cube_divider;
	
	// Diastash query dianysmatwn
	int d_q;
	
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
	
	// Pinakes apostasewn
	double* lsh_distances;
	double* brutal_search_distances;
	
	// Dynamikos pinakas gia thn apo8hkeysh twn ari8mwn dianysmatwn
	int* vector_numbers;

	// Dynamikos pinakas gia thn apo8hkeysh twn ari8mwn kampylwn
	int* curve_numbers;
	
	
	// To plh8os twn hi synarthsewn sto hypercube
	int num_of_hi_functions;
	
	// Metablhtes ypologismoy xronoy
	timespec start;
	timespec end;
	auto start_tm= high_resolution_clock::now();;
	auto stop_tm= high_resolution_clock::now();;
	auto duration = duration_cast<microseconds>(stop_tm - start_tm);
	

	// Plh8os dianysmatwn poy telika periexei o pinakas sygxwneyshs
	int merge_bucket_num_of_vectors;
	
	// O dynamikos pinakas sygxwneyshs twn kadwn
	int* merge_bucket;
	
	// Boh8htikh shmaia 
	int max_flag;
	
	// Megistos ari8mos dianysmatos (boh8htikh metablhth ston pinaka sygxwneyshs)
	int max=0;
	
	// O antistoixos ari8mos bucket
	int max_bucket=0;

	// Elaxistes apostaseis
	double n_distance;
	int n_position;

	// Metablhthes gia thn eyresh toy kontinoteroy lsh dianysmatos kata lsh
	double min_lsh_distance;
	double lsh_distance;
	
	// Metablhthes gia thn eyresh toy kontinoteroy lsh dianysmatos kata hypercube
	double min_hypercube_distance;
	double hypercube_distance;
	
	// Metablhthes gia thn eyresh toy pragmatika kontinoteroy dianysmatos
	double min_distance;
	double distance;
		
	// Index kontinoteroy dianysmatos kata lsh
	int closest_lsh_vector_index;
		
	// Index kontinoteroy dianysmatos 
	int closest_vector_index;
	
	// Index kontinoteroy dianysmatos kata hypercube
	int closest_hypercube_vector_index;
	
	// Oi zhtoymenoi xronoi
	double lsh_time, hypercube_time, brutal_search_time;
	
	// Pinakas o opoios periexei to plh8os twn dianysmatwn poy elegxontai apo ka8e koryfh
	int* vectors_to_check;
	
	// Oi ari8moi twn dianysmatwn poy elegxontai
	int* checking_vectors;
	
	// Oi antistoixes apostaseis
	double* checked_vector_distances;
		
	// Metablhtes sihlouette
	double* s_array;
	int best_neihbour;
	double distance_of_best_neihbour;
	int cluster_num;
	int neihbour_search_num;
	int* current_cluster;
	int current_search_size;
	int* neihbour_cluster;
	int neihbour_search_size;	
	double sum_distance;
	double sum_distance_to_neihbours;	
	double a_i, b_i,vector_si, sum_vector_si, s_i,stotal;
	
	// Synolikos xronos ypologismoy kontinoteroy dianysmatos symfwna me ton epilegmeno algori8mo
	double sumTimeApproximate;
	
	// Synolikos xronos ypologismoy pragmatika kontinoteroy dianysmatos 
	double sumTimeTrue;
	
	// Metrhths koryfwn (Boh8htikh metablhth)
	int vertex_counter;
	
	// Approximate factor enos ypologismoy
	double approximateFactor;
	
	// Megistos approximate factor
	double maxApproximateFactor;
	
	// Shmaia h opoia otan ypsw8ei o megistos approximate factor 8ewreitai oti teinei sto apeiro
	int approximateFactorInfinityFlag;
	
	double*** snappedCurves;
	// Dynamikos pinakas me tis snapped curves. H prwth syntetagmenh antistoixei ston ari8mo 
	// toy pinaka katakermatismoy, h deyterh ston ayksonta ari8mo kampyljs,  h trith sthn syntetagmenh 
	// ths kampylhs poy exei ginei snap
	
	double***  snappedQueries;
	// Dynamikos pinakas me ta snapped queries. H prwth syntetagmenh antistoixei ston ari8mo 
	// toy pinaka katakermatismoy, h deyterh ston ayksonta ari8mo kampyljs,  h trith sthn syntetagmenh 
	// toy query poy exei ginei snap
	
	// Pinakas me tis times poy pairnei h parametros t se ka8e pinaka katakermatismoy 	
	double** tValues;
	
	// H parametros t gia ena grid
	double* t;
	
	// Epilogeas tyxaioy ari8moy apo 0 mexri 1
	// 8a xrhsimopoih8ei stis epiloges tyxaiwn ari8mwn sto diasthma (0,d)
	uniform_real_distribution<double> unif(0,1);
	
	// Gennhtria tyxaiothtas
	default_random_engine re;
			
	// H kontinoterh kampylh  kata discrete frechet me lsh 
	int closest_lsh_discrete_frechet_index;
	
	// H pragmatika kontinoterh  kata discrete frechet 
	int closest_discrete_frechet_index;
	
	// Ari8mos kadoy
	int bucket_num;
			
	// Deikths se query shmeio
	Point* queryPt;
	
	// Ta shmeia poy orizoyn ena query
	Points* queryPoints;
	
	// Kampylh eperwthshs sth morfh ypologismoy frechet
	Curve* queryCurve;
			
	// Deikths se shmeio ypopshfias kontinoterhs kampylhs
	Point* candidatePt;
	
	// Ta shmeia poy orizoyn mia ypopshfia kontinoterh kampylh
	Points* candidatePoints;
	
	// Ypopshfia kontinoterh kampylh sth morfh ypologismoy frechet
	Curve* candidateCurve;
	
	// Ypologizomenh apostash kampylwn kata discrete frechet
	double discrete_frechet_distance;
	
	// Elaxisth apostash kampylwn kata discrete frechet me lsh
	double min_lsh_discrete_frechet_distance;
	
	// Elaxisth apostash kampylwn kata discrete frechet
	double min_discrete_frechet_distance;
	
	// H kontinoterh kampylh kata continuous frechet me lsh
	int closest_lsh_continuous_frechet_index;
	
	// Ypologizomenh apostash kampylwn kata continuous frechet
	double continuous_frechet_distance;	
	
	// Elaxisth apostash kampylwn kata continuous frechet me lsh
	double min_lsh_continuous_frechet_distance;
	
	// H pragmatika elaxisth apostash kampylwn kata continuous frechet
	double min_continuous_frechet_distance;
	
	// H pragmatika kontinoterh kampylh kata continuous frechet 
	int closest_continuous_frechet_index;
	
	int l;

	// Pinakas kampylwn eisodoy opws diavazontai gia continuous frechet
	double** continuous_frechet_curves;
	
	// Pinakas queries opws diavazontai gia continuous frechet
	double** continuous_frechet_queries;
	
	// Boh8htikos xwros proswrinhs apo8hkeyshs kampylhs gia tis diadikasies ths twn filtering, snap , diathrhshs
	// minima - maxima, apaloifhs twn synexomenwn diplotypwn kai paffing
	double* temp_curve;
	
	// Meta to snap kai thn apaleifontai ta synexomena diplotypa
	// Diathroyme enan pinaka o opoios krata to mhkos opws prokyptei kata th diarkeia ths diadikasias
	int* snapped_curve_lengths;
	
	// Boh8htikes metablhtes gia ton taxytero ypologismo apolytwn sth diadikasia toy filtering
	double fabs_a_minus_b;
	double fabs_b_minus_c;
	
	// Oi metablhtes minima maxima toy sxetikoy algori8moy
	double minima;
	double maxima;
	
	// Boh8htikh metablhth
	int discrete_frechet_curve_length;
	
	// To mhkos ths kampylhs meta to filtering kai thn apaleifh twn diplotypwn
	int continuous_frechet_curve_length;
		
	// Metablhth epistrefomenhs discrete apostashs ws antikeimenoy ths sxetikhs dosmenhs klashs
	Frechet::Discrete::Distance d_dst ;
	
	// Metablhth epistrefomenhs continuous apostashs ws antikeimenoy ths sxetikhs dosmenhs klashs
	Frechet::Continuous::Distance c_dst ;
	
	// Diavasma kai elegxos orismatwn
	if(ARGS_get_search_args(argc,argv,&input_file,&query_file,&output_file,&algorithm,&k,&L,&M,&probes,&metric,&delta,DEFAULT_LSH_k,DEFAULT_L,DEFAULT_HYPERCUBE_k,DEFAULT_HYPERCUBE_M,DEFAULT_HYPERCUBE_probes,DEFAULT_FRECHET_METRIC)<0)
		exit(-1);

	// Uncomment gia ektypwsh orismatwn
	// SEARCH_ARGS_print_search_args(input_file,query_file,output_file,algorithm,k,L,M,probes,metric,delta);
		
	// Diabazontai oi times eplsion kai M apo arxeio an yparxei alliws menoyn oi default
	fp=fopen("search.conf","r");
	
	if(fp){
		
		fscanf_result=fscanf(fp,"%lf",&epsilon);
		fscanf_result=fscanf(fp,"%lf",&bigM);
		
		fclose(fp);
		
	}

	// Periptwsh LSH dianysmatwn
	if(algorithm==LSH){
				
		printf("LSH vectors case.\n");
				
		// Anoigma toy arxeiou configure
		fp=fopen(LSH_CONFIG,"r");
		
		// Se periptwsh apotyxias to programma kanei eksodo 
		if(fp==NULL)
			exit(-1);
			
		// Diavasma toy M ths parametropoihshs twn h_i synarthsewn
		fscanf_result=fscanf(fp,"%u",&M_LSH);
			
		// Diavasma toy diaireth poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
		fscanf_result=fscanf(fp,"%d",&divider);
		
		// Ean einai mhden to programma termatizei
		if(divider==0)
			exit(-1);
		
		// Diavasma ths parametroy w ths 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
		fscanf_result=fscanf(fp,"%lf",&lsh_w);
		
		// Diavasma shmaias periorismoy h mh ston ari8mo twn elegxomenwn apo thn lsh dianysmatwn 	
		fscanf_result=fscanf(fp,"%d",&reduce_search);
			
		// Diavasma toy pollaplasiasth toy L poy dinei to megisto ari8mo dianysmatwn sthn periptwsh
		// periorismoy twn elegxomenwn apo thn lsh dianysmatwn 
		fscanf_result=fscanf(fp,"%d",&reducing_multiplier);
		
		// Kleisimo arxeiou configure
		fclose(fp);
				
		// Se periptwsh poy apotyxei to diavsma h shmaia periorismoy ti8etai mhden 
		// aneksarthta apo thn timh ths
		if(fscanf_result==0)
			reduce_search=0;
		
		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(VECTOR_read_vectors_from_file(input_file,&vectors,&vector_names,&num_of_vectors,&d)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}
				  
		// Dhmioyrgia synarthsewn katakermatismoy
		g=HASH_create_hash_functions(L,k,d,lsh_w);

		// Dhmioyrgia timwn r
		r_values=HASH_create_r_values(d);
				
		buckets=num_of_vectors/divider;

		// Desmeyoyme mnhmh gia ton pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
		lsh_hashtables=(void***)malloc(L*sizeof(void**));
		for(i=0;i<L;i++)
			lsh_hashtables[i]=(void**)malloc(buckets*sizeof(void*));
		
		// Gia ka8e kado dhmioyrgoyme mia lista
		for(i=0;i<L;i++)
			for(j=0;j<buckets;j++)
			lsh_hashtables[i][j]=LIST_create();
			
		// Ka8e dianysma katakermatizetai me ka8e g_i kai mpainei stoys antistoixous kadous
		for(i=0;i<num_of_vectors;i++){
			for(j=0;j<L;j++){
				
				// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
				bucket_number=HASH_g_i(g[j],vectors[i],r_values,lsh_w,d,M_LSH,k,buckets);
				
				// Eisagwgh toy dianysmatos sto bucket poy proekypse 
				LIST_insert((list*)(lsh_hashtables[j][bucket_number]),i);
			}	
		}
		
		// Twra metatrepoyme ta buckets apo listes se dynamikoys pinakes gia na beltiwsoyme 
		// thn taxythta.
		for(i=0;i<L;i++){
			for(j=0;j<buckets;j++){
					
				// Kataskeyh bucket apo th sxetikh lista
				bucket_ptr=(bucket*)LIST_convert_to_bucket((list*)(lsh_hashtables[i][j]));
				
				// Adeiasma listas
				LIST_empty((list*)(lsh_hashtables[i][j]));
				
				// Apeley8erwsh xwroy listas
				free(lsh_hashtables[i][j]);
				
				// Ekxwrhsh bucket sthn antistoixh 8esh
				lsh_hashtables[i][j]=bucket_ptr;
						
			}
		
		}
		
		// Diavasma query-dianysmatwn apo arxeio
		VECTOR_read_vectors_from_file(query_file,&queries,&query_names,&num_of_queries,&d_q);
		
		// Elegxos ean sympiptoyn oi diastaseis (prepei na sympiptoyn)
		if(d_q!=d){
			
			printf("Dimensions of query vectors are different from input vectors!\n");
			
			VECTOR_free_vectors(vectors,num_of_vectors,d);
			VECTOR_free_vectors(queries,num_of_queries,d_q);
		
			HASH_free_hash_functions(g,L,k);
			
			exit(-1);
			
		}
	
	

		// Desmeysh pinaka me toys ari8moys buckets ka8e pinaka katakermatismoy 
		// poy psaxnei o lsh gia ka8e query 
		search_bucket_numbers=(int*)malloc(L*sizeof(int));
		
		// Desmeysh pinaka me toys deiktes sta bucketska8e pinaka katakermatismoy 
		// poy psaxnei o lsh gia ka8e query 
		search_buckets=(bucket**)malloc(L*sizeof(bucket*));
		
		// Desmeysh pinaka me toys ari8modeiktes sygxwneyshs twn buckets 
		search_bucket_indeces=(int*)malloc(L*sizeof(int));
		
		// Desmeysh pinaka me toys ayksontes ari8moys dianysmatwn
		vector_numbers=(int*)malloc(num_of_vectors*sizeof(int));

		// Desmeysh pinaka me tis apostaseis enos query dianysmatos apo ta ypoloipa 
		brutal_search_distances=(double*)malloc(num_of_vectors*sizeof(double));
		
		
		fp=fopen(output_file,"w");
		
		// Mhdenismos a8roismatwn xronwn poy prokeitai na ypologistoyn
		sumTimeApproximate=0;
		sumTimeTrue=0;
			
		// Arxikopoihsh megistoy approximate factor
		maxApproximateFactor=0;
	
		// Arxikopoihsh shmaias h opoia otan ypsw8ei o megistos approximate factor 8ewreitai oti teinei sto apeiro
		approximateFactorInfinityFlag=0;
		
		for(q=0;q<num_of_queries;q++){
			
			
			// Arxikopoihsh megethoys pinaka sygxwneyshs kadwn
			merge_bucket_length=0;
				
			// Briskoyme toys kadoys poy 8a ginei anazhthsh
			for(j=0;j<L;j++){
				
				// Ypologismos ari8moy bucket gia ton j pinaka katakermatismoy
				search_bucket_numbers[j]=HASH_g_i(g[j],queries[q],r_values,lsh_w,d,M_LSH,k,buckets);
				
				// Ypologismos diey8ynshs toy en logw bucket
				search_buckets[j]=(bucket*)lsh_hashtables[j][search_bucket_numbers[j]];
							
				// Enhmerwsh megethoys pinaka sygxwneyshs kadwn
				merge_bucket_length+=search_buckets[j]->length;
				
			}	
			
			// Ean doyleyoyme me periorismo anazhthsewn, anaprosarmozoyme to bucket sygxwneyshs
			if(reduce_search){
				
				if(merge_bucket_length>reducing_multiplier*L)
					merge_bucket_length=reducing_multiplier*L;
				
			}
			
			// Desmeysh mnhmhs dynamikoy pinaka sygxwneyshs twn kadwn
			merge_bucket=(int*)malloc(merge_bucket_length*sizeof(int));
			
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
			start_tm = high_resolution_clock::now();
			
			// Desmeysh pinaka me tis apostaseis enos query dianysmatos ta dianysmata poy orizei o lsh  
			min_lsh_distance=distance_core_metric(vectors[merge_bucket[0]],queries[q],d);
			closest_lsh_vector_index=0;
			
			// Ypologismos apostasewn toy trexontos query me ka8ena apo ta dianysmata poy orizei o lsh
			for(i=1;i<merge_bucket_num_of_vectors;i++){		
				
				lsh_distance=distance_core_metric(vectors[merge_bucket[i]],queries[q],d);
				
				if(lsh_distance<min_lsh_distance){
					
					min_lsh_distance=lsh_distance;
					closest_lsh_vector_index=i;					
				}
				
			}
						
			// Lhksh xronometroy xronoy lsh
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			lsh_time = duration.count();

			// Enarksh xronometroy xronoy eksantlhtikhs anazhthshs
			start_tm = high_resolution_clock::now();
		
			// Arxikopoihshs pragmatikhs apostashs
			min_distance=distance_core_metric(vectors[i],queries[q],d);
			closest_vector_index=i;
					
			// Ypologismos apostasewn toy trexontos query me ka8ena apo ta ypoloipa dianysmata
			for(i=1;i<num_of_vectors;i++){		
			
				distance=distance_core_metric(vectors[i],queries[q],d);
				
				if(distance<min_distance){
					
					min_distance=distance;
					closest_vector_index=i;					
				}				
			}
			
			// Lhksh xronometroy xronoy eksantlhtikhs anazhthshs	
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			brutal_search_time = duration.count();
			
			fprintf(fp,"Query: %s\n",query_names[q]);			
			fprintf(fp,"Algorithm: LSH_vector\n");
			fprintf(fp,"Approximate Nearest neighbor: %s\n",vector_names[closest_lsh_vector_index]);
			fprintf(fp,"True Nearest neighbor: %s\n",vector_names[closest_vector_index]);
			fprintf(fp,"distanceApproximate: %.6lf\n",min_lsh_distance);
			fprintf(fp,"distanceTrue: %.6lf\n",min_distance);
							
			// Ypologismos toy approximate factor ths trexoysas periptwshs
			
			// An h pragmatikh apostash den einai mhdenikh ypologizetai o approximate factor
			// apo to phliko ths proseggistikhs apostashs dia thn pragmatikh
			if(min_distance!=0){
				
				approximateFactor=min_lsh_distance/min_distance;
				
			}
			else{
			// Diaforetika an h pragmatikh apostash einai mhdenikh exoyme dyo ypoperiptwseis
				
				// Ean kai h proseggistikh einai mhden, o approximate factor ti8etai mhden
				if(min_lsh_distance==0)
					approximateFactor=0;
				// Diaforetika o approximate factor apaeirizetai opote ypzwnetai h sxetikh shmaia
				else
					approximateFactorInfinityFlag=1;
			}
			
			// Enhmerwsh toy megistoy approximate factor
			if(approximateFactorInfinityFlag!=1){
				
				if(approximateFactor>maxApproximateFactor)
					maxApproximateFactor=approximateFactor;
				
				
			}
			
			// Enhmerwsh a8roismatos xronwn proseggistikhs anazhthshs
			sumTimeApproximate+=lsh_time;
			
			// Enhmerwsh a8roismatos xronwn eksantlhtikhs anazhthshs
			sumTimeTrue+=brutal_search_time;
					
			// Apeley8erwsh mnhmhs pinaka sygxwneyshs bucket		
			free(merge_bucket);
			
			// Apeley8erwsh mnhmhs pinaka me tis apostaseis enos query dianysmatos ta dianysmata poy orizei o lsh  
			free(lsh_distances);
			
		}
		
		// Ypologismos kai typwsh toy mesoy xronoy proseggistikhs anazhthshs
		fprintf(fp,"tApproximateAverage: %.3lf microseconds\n",sumTimeApproximate/num_of_queries);
		
		// Ypologismos kai typwsh toy mesoy xronoy eksanlthtikhs anazhthshs
		fprintf(fp,"tTrueAverage: %.3lf microseconds\n",sumTimeTrue/num_of_queries);	
				
			
		// An o maximum approximate factor exei apeiristh ws timh toy sto arxei grafetai h symboloseira "infinity"
		if(approximateFactorInfinityFlag==1){
			
			fprintf(fp,"MAF: infinity\n");
			
		}
		else{
		// Diaforetika grafetai h timh poy exei ypologistei

			fprintf(fp,"MAF: %.3lf \n",maxApproximateFactor);			
			
		}
		
		fclose(fp);
		
		// Apodesmeysh mnhmhs
		
		// Apodesmeysh toy hashtable toy lsh
		// Apeley8erwsh mnhmhs toy pinaka lsh_hashtables poy ylopoiei toys pinakes katakermatismoy 
		for(i=0;i<L;i++){
			for(j=0;j<buckets;j++){
				
				free(((bucket*)(lsh_hashtables[i][j]))->keys);
				free(lsh_hashtables[i][j]);
				
			}
			free(lsh_hashtables[i]);		
			
		}
		free(lsh_hashtables);	
		
		// Apodesmeysh pinaka me toys ari8moys buckets ka8e pinaka katakermatismoy 
		// poy psaxnei o lsh gia ka8e query 
		free(search_bucket_numbers);
		
		// Apodesmeysh pinaka me toys deiktes sta bucketska8e pinaka katakermatismoy 
		// poy psaxnei o lsh gia ka8e query 
		free(search_buckets);
		
		// Apodesmeysh pinaka me toys ari8modeiktes sygxwneyshs twn buckets 
		free(search_bucket_indeces);
		
		// Apodesmeysh pinaka me toys ayksontes ari8moys dianysmatwn
		free(vector_numbers);

		// Apodesmeysh pinaka me tis apostaseis enos query dianysmatos apo ta ypoloipa 
		free(brutal_search_distances);
				
		// Apeley8erwsh pinakwn dianysmatwn eisodoy kai dianysmatwn queries
		VECTOR_free_vectors(vectors,num_of_vectors,d);
		VECTOR_free_vectors(queries,num_of_queries,d);
	
		// Apeley8erwsh synarthsewn katakermatismoys
		HASH_free_hash_functions(g,L,k);
	
		// Apodesmeysh onomatwn twn dianysmatwn
		for(i=0;i<num_of_vectors;i++)
			free(vector_names[i]);
		
		free(vector_names);
		
		// Apodesmeysh onomatwn twn queries
		for(i=0;i<num_of_queries;i++)
			free(query_names[i]);
		
		free(query_names);
		
		// Apodesmeysh twn r_values
		free(r_values);
		
		
		exit(0);
		
	}
	// Periptwsh HYPERCUBE dianysmatwn
	else if(algorithm==HYPERCUBE){
		
		printf("Hypercube vectors case.\n");
		
		// Anoigma toy arxeiou configure
		fp=fopen(CUBE_CONFIG,"r");
		
		// Se periptwsh apotyxias to programma kanei eksodo 
		if(fp==NULL){
			
			printf("Can not open configure file!\n");
			exit(-1);
			
		}
			
		// Diavasma toy diaireth poy diairoyme to plh8os twn dianysmatwn gia na orisoyme to plh8os twn kadwn 
		fscanf_result=fscanf(fp,"%d",&divider);
		
		// Ean einai mhden to programma termatizei
		if(divider<=0){
			
			printf("Invilid divider!\n");
			exit(-1);
			
		}
		
		// Diavasma ths parametroy w ths 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
		fscanf_result=fscanf(fp,"%lf",&cube_w);
			
		// Se periptwsh poy apotyxei to diavsma to programma epistrefei
		if(fscanf_result==0){
			
			printf("Error in reading configure fille!\n");
			exit(-1);		
			
		}
		
		// Kleisimo arxeioy config
		fclose(fp);
		
		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(VECTOR_read_vectors_from_file(input_file,&vectors,&vector_names,&num_of_vectors,&d)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}
		// Dhmioyrgia synarthshs katakermatismoy h
		h=HASH_create_hash_function(k,d,cube_w);
		
		buckets=num_of_vectors/divider;

		// Desmeyoyme mnhmh gia ton pinaka cube_hashtables poy ylopoiei toys pinakes katakermatismoy 
		cube_hashtables=(int**)malloc(k*sizeof(int*));
		for(i=0;i<k;i++)
			cube_hashtables[i]=(int*)malloc(buckets*sizeof(int));
		
		// Gia ka8e kado antistoixoyme tyxaia to 0 'h to 1
		for(i=0;i<k;i++)
			for(j=0;j<buckets;j++)
				cube_hashtables[i][j]=rand()%2;
				
		// Ypologismos plh8ous koryfwn toy yperkyboy
		num_of_vertices=1;
		for(i=0;i<k;i++)
			num_of_vertices*=2;

		// Dhmioyrgoyme ton yperkybo. Arxika se ka8e koryfh antistoixoyme mia lista 
		hypercube=(void**)malloc(num_of_vertices*sizeof(void*));
		for(i=0;i<num_of_vertices;i++)
			hypercube[i]=LIST_create();

		// Ka8e dianysma katakermatizetai me ka8e g_i kai ypologizetai h korryfh toy yperkyboy 
		// sthn opoia eisagetai
		for(i=0;i<num_of_vectors;i++){
			
			vertex_number=0;
			for(j=0;j<k;j++){
				
				bucket_number=HASH_h_i(h[j],vectors[i],1/cube_w,d)%buckets;
			
				vertex_number+=vertex_number+cube_hashtables[0][bucket_number];
				
			}	
			// Eisagwgh dianysmatos sthn koryfh
			LIST_insert((list*)hypercube[vertex_number],i);

		}
			
		// Twra metatrepoyme tis koryfes apo listes se buckets pou periexoyn dynamikoys pinakes 
		// gia na beltiwsoyme thn taxythta.
		for(i=0;i<num_of_vertices;i++){
			
			// Kataskeyh bucket apo th sxetikh lista
			bucket_ptr=LIST_convert_to_bucket((list*)hypercube[i]);
			
			// Adeiasma listas
			LIST_empty((list*)hypercube[i]);
			
			// Apeley8erwsh xwroy listas
			free((list*)hypercube[i]);
			
			// Ekxwrhsh bucket sthn antistoixh 8esh
			hypercube[i]=bucket_ptr;
				
		}
	
		// Desmeysh xwroy gia ton pinaka pou 8a apo8hkeysei proswrina tis hamming apostaseis mias koryfhs 
		// apo tis ypoloipes
		hamming_distances =(int*)malloc(num_of_vertices*sizeof(int));
		
		// Desmeysh xwroy gia ton pinaka pou 8a apo8hkeysei proswrina tous ari8moys twn koryfwn
		vertex_numbers =(int*)malloc(num_of_vertices*sizeof(int));
		
		// Oi dyo aytoi pinakes einai 1 pros 1. Taksinomontas ton prwto kai kratwntas thn 1 pros 1 sxesh me 
		// ton deytero diatassoyme tis koryfes symfwna me tis apostaseis hamming apo sygkekrimenh koryfh
		
		// Desmeysh xwroy gia ton disdiastato pinakas ka8e grammh toy opoioy antistoixei se mia koryfh 
		// kai  periexei taksinomhmenes probes+1 to plh8os koryfes symfwna me thn apostash hamming apo aythn
		probes_array=(int**)malloc(num_of_vertices*sizeof(int*));
		
		for(i = 0; i < num_of_vertices; ++i)
			probes_array[i] = (int*)malloc((probes+1)*sizeof(int));
		
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
		
		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(VECTOR_read_vectors_from_file(query_file,&queries,&query_names,&num_of_queries,&d_q)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}		
		
		// Elegxos ean sympiptoyn oi diastaseis (prepei na sympiptoyn)
		if(d_q!=d){
			
			printf("Dimensions of query vectors are different from input vectors!\n");
			
			VECTOR_free_vectors(vectors,num_of_vectors,d);
			VECTOR_free_vectors(queries,num_of_queries,d_q);
		
			HASH_free_hash_function(h,num_of_hi_functions);
			
			exit(-1);
			
		}
			
		// Desmeysh pinaka o opoios periexei to plh8os twn dianysmatwn poy elegxontai apo ka8e koryfh
		vectors_to_check=(int*)malloc((probes+1)*sizeof(int));
		
		// Desmeysh pinaka o opoios periexei toys ari8moys twn dianysmatwn poy elegxontai
		checking_vectors=(int*)malloc(M*sizeof(int));
		
		// Desmeysh pinaka o opoios periexei tis antistoixes apostaseis
		checked_vector_distances=(double*)malloc(M*sizeof(double));
		
		// To moirasma ginetai isomerws me kapoies ek twn kontinoterwn koryfwn na dinoyn isws ena dianysma
		// pararapanw (analoga me th diairesh toy synolikoy plh8oys twn eksetazomenwn dianysmatwn dia 
		// to plh8os twn eksetazomenwn koryfwn)
		for(i=0;i<probes+1;i++)
			vectors_to_check[i]=M/(probes+1);
		
		for(i=0;i<M%(probes+1);i++)
			vectors_to_check[i]++;
		
		// Desmeysh pinaka me tis apostaseis enos query dianysmatos apo ta ypoloipa 
		brutal_search_distances=(double*)malloc(num_of_vectors*sizeof(double));
		
		// Anoigma arxeioy eksodoy
		fp = fopen(output_file,"w");
			
		// Mhdenismos a8roismatwn xronwn poy prokeitai na ypologistoyn
		sumTimeApproximate=0;
		sumTimeTrue=0;
		
		// Arxikopoihsh megistoy approximate factor
		maxApproximateFactor=0;
	
		// Arxikopoihsh shmaias h opoia otan ypsw8ei o megistos approximate factor 8ewreitai oti teinei sto apeiro
		approximateFactorInfinityFlag=0;
		
		// Ka8e query katakermatizetai me ka8e g_i kai ypologizetai h koryfh toy yperkyboy 
		// sthn opoia antistoixei
		for(q=0;q<num_of_queries;q++){
			
			// Enarksh xronometroy xronoy hypercube
			start_tm = high_resolution_clock::now();
			
			vertex_number=0;
			for(j=0;j<k;j++){
				
				// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
				bucket_number=HASH_h_i(h[j],queries[q],1/cube_w,d)%buckets;
				
				// Sxhmatismos toy ari8moy koryfhs
				vertex_number+=vertex_number+cube_hashtables[j][bucket_number];
				
			}	
		
			vertex_counter=0;
			
			// Vriskoyme ta dianysmata pou 8a elegx8oun
			for(i=0;i<probes+1;i++){
				
				
				num_of_vectors_in_vertex=((bucket*)(hypercube[probes_array[vertex_number][i]]))->length;
				for(j=0;j<vectors_to_check[i];j++){
					
					// An teleiwsan ta dianysmata sthn koryfh, kanoyme break kai pame sthn epomenh
					if(j==num_of_vectors_in_vertex)
						break;
					
					checking_vectors[vertex_counter++]=((bucket*)(hypercube[probes_array[vertex_number][i]]))->keys[j];
									
				}
				
			}
		
			// Ypologismos apostasewn toy trexontos query me ka8ena apo ta dianysmata poy orizei o hypercube
			min_hypercube_distance=distance_core_metric(vectors[checking_vectors[0]],queries[q],d);
			closest_hypercube_vector_index=0;
			
			// Ypologismos apostasewn toy trexontos query me ka8ena apo ta dianysmata poy orizei o lsh
			for(i=1;i<vertex_counter;i++){		
						
				hypercube_distance=distance_core_metric(vectors[checking_vectors[i]],queries[q],d);
				
				if(hypercube_distance<min_hypercube_distance){
					
					min_hypercube_distance=hypercube_distance;
					closest_hypercube_vector_index=i;
					
				}
				
			}
			
			// Lhksh xronometroy xronoy hypercube
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			hypercube_time = duration.count();

			// Enarksh xronometroy xronoy eksantlhtikhs anazhthshs
			start_tm = high_resolution_clock::now();
			
			// Arxikopoihshs pragmatikhs apostashs
			min_distance=distance_core_metric(vectors[i],queries[q],d);
			closest_vector_index=i;
					
			// Ypologismos apostasewn toy trexontos query me ka8ena apo ta ypoloipa dianysmata
			for(i=1;i<num_of_vectors;i++){		
			
				distance=distance_core_metric(vectors[i],queries[q],d);
				
				if(distance<min_distance){
					
					min_distance=distance;
					closest_vector_index=i;
					
				}
			
			}
			
			// Lhksh xronometroy xronoy eksantlhtikhs anazhthshs	
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			brutal_search_time = duration.count();
		
			fprintf(fp,"Query: %s\n",query_names[q]);			
			fprintf(fp,"Algorithm: Hypercube\n");
			fprintf(fp,"Approximate Nearest neighbor: %s\n",vector_names[closest_hypercube_vector_index]);
			fprintf(fp,"True Nearest neighbor: %s\n",vector_names[closest_vector_index]);
			fprintf(fp,"distanceApproximate: %.6lf\n",min_hypercube_distance);
			fprintf(fp,"distanceTrue: %.6lf\n",min_distance);
			
			// Ypologismos toy approximate factor ths trexoysas periptwshs
			
			// An h pragmatikh apostash den einai mhdenikh ypologizetai o approximate factor
			// apo to phliko ths proseggistikhs apostashs dia thn pragmatikh
			if(min_distance!=0){
				
				approximateFactor=min_hypercube_distance/min_distance;
				
			}
			else{
			// Diaforetika an h pragmatikh apostash einai mhdenikh exoyme dyo ypoperiptwseis
				
				// Ean kai h proseggistikh einai mhden, o approximate factor ti8etai mhden
				if(min_hypercube_distance==0)
					approximateFactor=0;
				// Diaforetika o approximate factor apaeirizetai opote ypzwnetai h sxetikh shmaia
				else
					approximateFactorInfinityFlag=1;
			}
			
			// Enhmerwsh toy megistoy approximate factor
			if(approximateFactorInfinityFlag!=1){
				
				if(approximateFactor>maxApproximateFactor)
					maxApproximateFactor=approximateFactor;
				
				
			}
					
			// Enhmerwsh a8roismatos xronwn proseggistikhs anazhthshs
			sumTimeApproximate+=hypercube_time;
			
			// Enhmerwsh a8roismatos xronwn eksantlhtikhs anazhthshs
			sumTimeTrue+=brutal_search_time;
					
		}
					
		// Ypologismos kai typwsh toy mesoy xronoy proseggistikhs anazhthshs
		fprintf(fp,"tApproximateAverage: %.3lf microseconds\n",sumTimeApproximate/num_of_queries);
		
		// Ypologismos kai typwsh toy mesoy xronoy eksanlthtikhs anazhthshs
		fprintf(fp,"tTrueAverage: %.3lf microseconds\n",sumTimeTrue/num_of_queries);	
			
		// An o maximum approximate factor exei apeiristh ws timh toy sto arxei grafetai h symboloseira "infinity"
		if(approximateFactorInfinityFlag==1){
			
			fprintf(fp,"MAF: infinity\n");
			
		}
		else{
		// Diaforetika grafetai h timh poy exei ypologistei

			fprintf(fp,"MAF: %.3lf \n",maxApproximateFactor);			
			
		}
				
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
		HASH_free_hash_function(h,k);
		
		// Apeley8erwsh mnhmhs toy pinaka cube_hashtables poy ylopoiei toys pinakes katakermatismoy 
		for(i=0;i<k;i++)
			free(cube_hashtables[i]);	
		free(cube_hashtables);
				
		// Apodesmeysh onomatwn twn dianysmatwn
		for(i=0;i<num_of_vectors;i++)
			free(vector_names[i]);
		
		free(vector_names);
		
		// Apodesmeysh onomatwn twn queries
		for(i=0;i<num_of_queries;i++)
			free(query_names[i]);
		
		free(query_names);
		
		exit(0);
		
	}
	
	else if(algorithm==FRECHET && metric==DISCRETE){
				
		printf("Discrete Frechet curves case.\n");
		
		// Anoigma toy arxeiou configure
		fp=fopen(LSH_CONFIG,"r");
		
		// Se periptwsh apotyxias to programma kanei eksodo 
		if(fp==NULL)
			exit(-1);
			
		// Diavasma toy M ths parametropoihshs twn h_i synarthsewn
		fscanf_result=fscanf(fp,"%u",&M_LSH);
			
		// Diavasma toy diaireth poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
		fscanf_result=fscanf(fp,"%d",&divider);
		
		// Ean einai mhden to programma termatizei
		if(divider==0)
			exit(-1);
		
		// Diavasma ths parametroy w ths 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
		fscanf_result=fscanf(fp,"%lf",&lsh_w);
		
		// Diavasma shmaias periorismoy h mh ston ari8mo twn elegxomenwn apo thn lsh dianysmatwn 	
		fscanf_result=fscanf(fp,"%d",&reduce_search);
			
		// Diavasma toy pollaplasiasth toy L poy dinei to megisto ari8mo dianysmatwn sthn periptwsh
		// periorismoy twn elegxomenwn apo thn lsh dianysmatwn 
		fscanf_result=fscanf(fp,"%d",&reducing_multiplier);
		
		// Kleisimo arxeiou configure
		fclose(fp);

		
		// Se periptwsh poy apotyxei to diavsma h shmaia periorismoy ti8etai mhden 
		// aneksarthta apo thn timh ths
		if(fscanf_result==0)
			reduce_search=0;

		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(CURVE_read_discrete_frechet_curves_from_file(input_file,&discrete_frechet_curves,&curve_names,&num_of_curves,&d)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}

		// Dhmioyrgia toy pinaka me tis t parametroys (ena t gia ka8e grid)
		
		// Desmeysh xwroy gia ton pinaka
		tValues=(double**)malloc(L*sizeof(double*));
	
		for(i=0;i<L;i++)
			tValues[i]=(double*)malloc(d*sizeof(double));

		// Apodosh timwn stis parametroys tApproximateAverage
		for(i=0;i<L;i++)
			for(j=0;j<d;j++)
				tValues[i][j]=delta*unif(re);

		// Dhmioyrgeitai o pinakas gia tis snapped curves
		snappedCurves=(double***)malloc(L*sizeof(double**));

		for(i=0;i<L;i++)
			snappedCurves[i]=(double**)malloc(num_of_curves*sizeof(double*));

		for(i=0;i<L;i++){
			for(j=0;j<num_of_curves;j++){
				snappedCurves[i][j]=(double*)malloc(2*d*sizeof(double));
			}
		}
		
		temp_curve=(double*)malloc(2*d*sizeof(double));

		// Oi kampyles ginontai snap sta grids
		for(i=0;i<L;i++){
			for(j=0;j<num_of_curves;j++){
				for(v=0;v<d;v++){
					
					snappedCurves[i][j][2*v]=floor((discrete_frechet_curves[j][2*v]-tValues[i][v])/delta+0.5)*delta+tValues[i][v];
					
					snappedCurves[i][j][2*v+1]=floor((discrete_frechet_curves[j][2*v+1]-tValues[i][v])/delta+0.5)*delta+tValues[i][v];	
					
				}
				
				// Akolou8ei h apaleifh diplotypwn;
				discrete_frechet_curve_length=0;
				
				temp_curve[0]=snappedCurves[i][j][0];
				temp_curve[0]=snappedCurves[i][j][1];
				
				
				discrete_frechet_curve_length=2;
				
				for(v=2;v<2*d;v+=2){
					
					if(temp_curve[v]!=temp_curve[v-2] || temp_curve[v+1]!=temp_curve[v-1] ){
						
						temp_curve[discrete_frechet_curve_length]=snappedCurves[i][j][v];
						discrete_frechet_curve_length++;		
						temp_curve[discrete_frechet_curve_length]=snappedCurves[i][j][v+1];
						discrete_frechet_curve_length++;		
						
					}
				}
				
				// Diadikasia oloklhrwnetai me to padding	
				for(v=discrete_frechet_curve_length;v<2*d;v++){
					
					temp_curve[v]=bigM;
				}			
			
				for(v=0;v<2*d;v++)
					snappedCurves[i][j][v]=temp_curve[v];
				
				
				
			}
			
			
		}
				
		// Dhmioyrgia synarthsewn katakermatismoy
		g=HASH_create_hash_functions(L,k,2*d,lsh_w);

		// Dhmioyrgia timwn r
		r_values=HASH_create_r_values(2*d);
				
		// Ypologismos ari8moy buckets 
		buckets=num_of_curves/divider;
		
		// Desmeyoyme mnhmh gia ton pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
		lsh_hashtables=(void***)malloc(L*sizeof(void**));
		for(i=0;i<L;i++)
			lsh_hashtables[i]=(void**)malloc(buckets*sizeof(void*));
		
		// Gia ka8e kado dhmioyrgoyme mia lista
		for(i=0;i<L;i++)
			for(j=0;j<buckets;j++)
				lsh_hashtables[i][j]=LIST_create();
		
		// Ka8e snapped kampylh katakermatizetai me ka8e g_i kai mpainei stoys antistoixous kadous
		for(i=0;i<L;i++){
			for(j=0;j<num_of_curves;j++){
				
				// Ypologismos bucket ths kampylhs i gia ton pinaka j
				bucket_number=HASH_g_i(g[i],snappedCurves[i][j],r_values,lsh_w,2*d,M_LSH,k,buckets);
						
				// Eisagwgh ths kampylhs sto bucket poy proekypse 
				LIST_insert((list*)(lsh_hashtables[i][bucket_number]),j);
			}	
		}
	
		// Twra metatrepoyme ta buckets apo listes se dynamikoys pinakes gia na beltiwsoyme 
		// thn taxythta.
		for(i=0;i<L;i++){
			
			for(j=0;j<buckets;j++){
					
				// Kataskeyh bucket apo th sxetikh lista
				bucket_ptr=(bucket*)LIST_convert_to_bucket((list*)(lsh_hashtables[i][j]));
				
				// Adeiasma listas
				LIST_empty((list*)(lsh_hashtables[i][j]));
				
				// Apeley8erwsh xwroy listas
				free(lsh_hashtables[i][j]);
				
				// Ekxwrhsh bucket sthn antistoixh 8esh
				lsh_hashtables[i][j]=bucket_ptr;
						
			}
		
		}
				
		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(CURVE_read_discrete_frechet_curves_from_file(query_file,&discrete_frechet_queries,&query_names,&num_of_queries,&d_q)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}

		// Elegxos ean sympiptoyn oi diastaseis (prepei na sympiptoyn)
		if(d_q!=d){
			
			printf("Dimensions of query vectors are different from input vectors!\n");
		
			CURVE_free_discrete_frechet_curves(discrete_frechet_curves,num_of_curves,d);
			CURVE_free_discrete_frechet_curves(discrete_frechet_queries,num_of_queries,d_q);
		
			HASH_free_hash_functions(g,L,k);
			
			exit(-1);
			
		}
			
		// Dhmioyrgeitai o pinakas gia tis snapped queries
		snappedQueries=(double***)malloc(L*sizeof(double**));
	
		for(i=0;i<L;i++)
			snappedQueries[i]=(double**)malloc(num_of_queries*sizeof(double*));

		for(i=0;i<L;i++){
			for(j=0;j<num_of_queries;j++){
				snappedQueries[i][j]=(double*)malloc(2*d*sizeof(double));
			}
		}
			
		// Ta queriess ginontai snap sta grids
		for(i=0;i<L;i++){
			for(j=0;j<num_of_queries;j++){
				for(v=0;v<d;v++){
					
					snappedQueries[i][j][2*v]=floor((discrete_frechet_queries[j][2*v]-tValues[i][v])/delta+0.5)*delta+tValues[i][v];
					
					snappedQueries[i][j][2*v+1]=floor((discrete_frechet_queries[j][2*v+1]-tValues[i][v])/delta+0.5)*delta+tValues[i][v];	
					
				}
				
				// Akolou8ei h apaleifh diplotypwn;
				discrete_frechet_curve_length=0;
				
				temp_curve[0]=snappedQueries[i][j][0];
				temp_curve[0]=snappedQueries[i][j][1];
				
				
				discrete_frechet_curve_length=2;
				
				for(v=2;v<2*d;v+=2){
					
					if(temp_curve[v]!=temp_curve[v-2] || temp_curve[v+1]!=temp_curve[v-1] ){
						
						temp_curve[discrete_frechet_curve_length]=snappedQueries[i][j][v];
						discrete_frechet_curve_length++;		
						temp_curve[discrete_frechet_curve_length]=snappedQueries[i][j][v+1];
						discrete_frechet_curve_length++;		
						
					}
				}
				
				// Diadikasia oloklhrwnetai me to padding	
				for(v=discrete_frechet_curve_length;v<2*d;v++){
					
					temp_curve[v]=bigM;
				}			
			
				for(v=0;v<2*d;v++)
					snappedQueries[i][j][v]=temp_curve[v];
				
				
			}
		}
		
		free(temp_curve);
		
		fp=fopen(output_file,"w");
		
		// Mhdenismos a8roismatwn xronwn poy prokeitai na ypologistoyn
		sumTimeApproximate=0;
		sumTimeTrue=0;
			
		// Arxikopoihsh megistoy approximate factor
		maxApproximateFactor=0;
	
		// Arxikopoihsh shmaias h opoia otan ypsw8ei o megistos approximate factor 8ewreitai oti teinei sto apeiro
		approximateFactorInfinityFlag=0;
		
		// Arxikopoihsh ths kontinoterhs kampylhs kata discrete frechet me lsh 
		closest_lsh_discrete_frechet_index=-1;
				
		// Epeksergasia ka8e query		
		for(q=0;q<num_of_queries;q++){
			
			// Enarksh xronometroy xronoy lsh discrete frechet
			start_tm = high_resolution_clock::now();
			
			// Arxikopoihsh megethoys pinaka sygxwneyshs kadwn
			//merge_bucket_length=0;
				
			// Gia ka8e pinaka katakermatismoy...
			for(j=0;j<L;j++){
				
				// ... fernoyme to antistoixo snapped query sthn katallhlh morfh.
				queryPoints= new Points(2);
				
				for(i=0;i<d;i++){
										
					queryPt = new Point(2);
					
					queryPt->set(0,snappedQueries[j][q][2*i]);
					queryPt->set(1,snappedQueries[j][q][2*i+1]);
					
					queryPoints->add(*queryPt);
					
					delete queryPt;
				}
				
				queryCurve=new Curve(*queryPoints,query_names[q]);
										
				// Ypologismos ari8moy bucket toy query gia ton trexonta pinaka katakermatismoy
				bucket_number=HASH_g_i(g[j],snappedQueries[j][q],r_values,lsh_w,2*d,M_LSH,k,buckets);
				
				// Eyresh toy idiou tou bucket
				bucket_ptr=(bucket*)lsh_hashtables[j][bucket_number];
							
				// Sarwnontai oi kampyles toy bucket			
				for(v=0;v<bucket_ptr->length;v++){
					
					// Fernoyme thn ekastote kampylh sthn katallhlh morfh
					candidatePoints= new Points(2);
		
					for(i=0;i<d;i++){
						
						candidatePt = new Point(2);
						
						candidatePt->set(0,snappedCurves[j][bucket_ptr->keys[v]][2*i]);
						candidatePt->set(1,snappedCurves[j][bucket_ptr->keys[v]][2*i+1]);
						
						candidatePoints->add(*candidatePt);
						
						delete candidatePt;
					}
				
					candidateCurve=new Curve(*candidatePoints,curve_names[bucket_ptr->keys[v]]);
					
					// Ypologizetai h apostash kata discrete frechet toy query apo thn kampylh
					d_dst = Frechet::Discrete::distance(*queryCurve,*candidateCurve);
					
					delete candidateCurve;
					
					discrete_frechet_distance = d_dst.value;
					
					// Periptwsh arxikopoihshs elaxisths discrete frechet apostashs
					if(closest_lsh_discrete_frechet_index<0){
						
						min_lsh_discrete_frechet_distance=discrete_frechet_distance;
						
						closest_lsh_discrete_frechet_index=bucket_ptr->keys[v];
						
						
					}
					else{
					// Periptwsh enhmerwshs elaxisths discrete frechet apostashs	
						
						if(discrete_frechet_distance<min_lsh_discrete_frechet_distance){
							
							min_lsh_discrete_frechet_distance=discrete_frechet_distance;
						
							closest_lsh_discrete_frechet_index=bucket_ptr->keys[v];	
							
							
						}
												
					}					
															
				}				
				
			}	
							
			// Lhksh xronometroy xronoy lsh discrete frechet
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			lsh_time = duration.count();

			// Enarksh xronometroy xronoy eksantlhtikhs anazhthshs
			start_tm = high_resolution_clock::now();
			
			// Arxikopoihsh ths pragmatika kontinoterhs kampylhs kata discrete frechet
			closest_discrete_frechet_index=-1;
			
			for(v=0;v<L;v++){
						
				// Ypologismos apostasewn toy trexontos query me ka8ena apo ta ypoloipa dianysmata
				for(i=0;i<num_of_curves;i++){		
					
					// Fernoyme thn ekastote kampylh sthn katallhlh morfh
					candidatePoints= new Points(2);
		
					for(j=0;j<d;j++){
						
						candidatePt = new Point(2);
						
						candidatePt->set(0,snappedCurves[v][i][2*j]);
						candidatePt->set(1,snappedCurves[v][i][2*j+1]);
						
						candidatePoints->add(*candidatePt);
						
						delete candidatePt;
					}
										
					candidateCurve=new Curve(*candidatePoints,curve_names[bucket_ptr->keys[v]]);
					
					// Ypologizetai h apostash kata discrete frechet toy query apo thn kampylh
					d_dst = Frechet::Discrete::distance(*queryCurve,*candidateCurve);
						
					delete candidateCurve;
					
					discrete_frechet_distance = d_dst.value;
					
					// Periptwsh arxikopoihshs elaxisths discrete frechet apostashs
					if(closest_discrete_frechet_index<0){
						
						min_discrete_frechet_distance=discrete_frechet_distance;
						
						closest_discrete_frechet_index=i;
						
						
					}
					else{
					// Periptwsh enhmerwshs elaxisths discrete frechet apostashs	
						
						if(discrete_frechet_distance<min_discrete_frechet_distance){
							
							min_discrete_frechet_distance=discrete_frechet_distance;
						
							closest_discrete_frechet_index=i;	
							
							
						}
												
					}					
					
				
				}
			
			}			
			// Lhksh xronometroy xronoy eksantlhtikhs anazhthshs	
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			brutal_search_time = duration.count();


			fprintf(fp,"Query: %s\n",query_names[q]);			
			fprintf(fp,"Algorithm: LSH_Frechet_Discrete\n");			
			fprintf(fp,"Approximate Nearest neighbor: %s\n",curve_names[closest_lsh_discrete_frechet_index]);
			fprintf(fp,"True Nearest neighbor: %s\n",curve_names[closest_discrete_frechet_index]);
			fprintf(fp,"distanceApproximate: %.6lf\n",min_lsh_discrete_frechet_distance);
			fprintf(fp,"distanceTrue: %.6lf\n",min_discrete_frechet_distance);
							
			// Ypologismos toy approximate factor ths trexoysas periptwshs
			
			// An h pragmatikh apostash den einai mhdenikh ypologizetai o approximate factor
			// apo to phliko ths proseggistikhs apostashs dia thn pragmatikh
			if(min_discrete_frechet_distance!=0){
				
				approximateFactor=min_lsh_discrete_frechet_distance/min_discrete_frechet_distance;
				
			}
			else{
			// Diaforetika an h pragmatikh apostash einai mhdenikh exoyme dyo ypoperiptwseis
				
				// Ean kai h proseggistikh einai mhden, o approximate factor ti8etai mhden
				if(min_lsh_discrete_frechet_distance==0)
					approximateFactor=0;
				// Diaforetika o approximate factor apaeirizetai opote ypzwnetai h sxetikh shmaia
				else
					approximateFactorInfinityFlag=1;
			}
			
			// Enhmerwsh toy megistoy approximate factor
			if(approximateFactorInfinityFlag!=1){
				
				if(approximateFactor>maxApproximateFactor)
					maxApproximateFactor=approximateFactor;
				
				
			}
			
			// Enhmerwsh a8roismatos xronwn proseggistikhs anazhthshs
			sumTimeApproximate+=lsh_time;
			
			// Enhmerwsh a8roismatos xronwn eksantlhtikhs anazhthshs
			sumTimeTrue+=brutal_search_time;
					
			delete queryCurve;
			
		}
		
		
		// Ypologismos kai typwsh toy mesoy xronoy proseggistikhs anazhthshs
		fprintf(fp,"tApproximateAverage: %.3lf microseconds\n",sumTimeApproximate/num_of_queries);
		
		// Ypologismos kai typwsh toy mesoy xronoy eksanlthtikhs anazhthshs
		fprintf(fp,"tTrueAverage: %.3lf microseconds\n",sumTimeTrue/num_of_queries);	
				
			
		// An o maximum approximate factor exei apeiristh ws timh toy sto arxei grafetai h symboloseira "infinity"
		if(approximateFactorInfinityFlag==1){
			
			fprintf(fp,"MAF: infinity\n");
			
		}
		else{
		// Diaforetika grafetai h timh poy exei ypologistei

			fprintf(fp,"MAF: %.3lf \n",maxApproximateFactor);			
			
		}
		
		fclose(fp);
				
		// Apodesmeysh mnhmhs
		CURVE_free_discrete_frechet_curves(discrete_frechet_curves,num_of_curves,d);
		CURVE_free_discrete_frechet_curves(discrete_frechet_queries,num_of_queries,d);
		
		// Apeley8erwsh synarthsewn katakermatismoys
		HASH_free_hash_functions(g,L,k);
	
		// Apodesmeysh onomatwn twn dianysmatwn
		for(i=0;i<num_of_curves;i++)
			free(curve_names[i]);
		
		free(curve_names);
		
		// Apodesmeysh onomatwn twn queries
		for(i=0;i<num_of_queries;i++)
			free(query_names[i]);
		
		free(query_names);
		
		// Apodesmeysh twn r_values
		free(r_values);
		
		
		// Apodesmeysh toy hashtable toy lsh
		// Apeley8erwsh mnhmhs toy pinaka lsh_hashtables poy ylopoiei toys pinakes katakermatismoy 
		for(i=0;i<L;i++){
			for(j=0;j<buckets;j++){
				
				free(((bucket*)(lsh_hashtables[i][j]))->keys);
				free(lsh_hashtables[i][j]);
				
			}
			free(lsh_hashtables[i]);		
			
		}
		free(lsh_hashtables);	
		

		exit(0);
		
	}
	
	else if(algorithm==FRECHET && metric==CONTINUOUS){
			
		printf("Continuous Frechet curves case.\n");
		
		// Anoigma toy arxeiou configure
		fp=fopen(LSH_CONFIG,"r");
		
		// Se periptwsh apotyxias to programma kanei eksodo 
		if(fp==NULL)
			exit(-1);
			
		// Diavasma toy M ths parametropoihshs twn h_i synarthsewn
		fscanf_result=fscanf(fp,"%u",&M_LSH);
			
		// Diavasma toy diaireth poy diairoyme to plh8os twn dianysmatwn gia na orisoyme toplh8os twn kadwn 
		fscanf_result=fscanf(fp,"%d",&divider);
		
		// Ean einai mhden to programma termatizei
		if(divider==0)
			exit(-1);
		
		// Diavasma ths parametroy w ths 8ewrias me bash to opoio dhmioyrrgoyntai h synarthseis katakermatismoy
		fscanf_result=fscanf(fp,"%lf",&lsh_w);
		
		// Diavasma shmaias periorismoy h mh ston ari8mo twn elegxomenwn apo thn lsh dianysmatwn 	
		fscanf_result=fscanf(fp,"%d",&reduce_search);
			
		// Diavasma toy pollaplasiasth toy L poy dinei to megisto ari8mo dianysmatwn sthn periptwsh
		// periorismoy twn elegxomenwn apo thn lsh dianysmatwn 
		fscanf_result=fscanf(fp,"%d",&reducing_multiplier);
		
		// Kleisimo arxeiou configure
		fclose(fp);

		
		// Se periptwsh poy apotyxei to diavsma h shmaia periorismoy ti8etai mhden 
		// aneksarthta apo thn timh ths
		if(fscanf_result==0)
			reduce_search=0;

		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(CURVE_read_continuous_frechet_curves_from_file(input_file,&continuous_frechet_curves,&curve_names,&num_of_curves,&d)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}
				
		// Desmeysh mnhmhs gia ton boh8htiko pinaka proswrinhs apo8hkeyshs kampylhs gia thn diadikasia ths 
		// apaloifhs twn synexomenwn diplotypwn
		temp_curve=(double*)malloc(num_of_curves*sizeof(double));
	
		// Desmeysh mnhmhs gia ton boh8htiko me ta mhkh twn snapped kampylwn
		snapped_curve_lengths=(int*)malloc(num_of_curves*sizeof(int));
	
		// Gia ka8e kampylh ginetai snap sto grid, meta apaloifh twn synexomenwn diplotypwn kai meta padding
		for(i=0;i<num_of_curves;i++){
			
			// Ksekiname me filtering ths kampylhs 			
			temp_curve[0]=continuous_frechet_curves[i][0];
			
			snapped_curve_lengths[i]=1;
			
			// Sto plaisio toy filtrarismatos epiviwnoyn oi times poy prepei, oi yploipes apaleifontai 
			for(j=1;j<d-1;j++){
				
				// Ypologismos twn dyo diadoxikwn apolytwn diaforwn toy filtering
				fabs_a_minus_b=continuous_frechet_curves[i][j]-continuous_frechet_curves[i][j-1];
				fabs_b_minus_c=continuous_frechet_curves[i][j+1]-continuous_frechet_curves[i][j];
				
				
				if(fabs_a_minus_b<0)
					fabs_a_minus_b=-fabs_a_minus_b;
				
				if(fabs_b_minus_c<0)
					fabs_b_minus_c=-fabs_b_minus_c;
				
				// Eggrafetai h paraleipetai (filtraretai) h epomenh timh symfwna me ton kanona toy
				// filtrarismatos
				if(!(fabs_a_minus_b<=epsilon && fabs_b_minus_c<=epsilon)){
					
					temp_curve[snapped_curve_lengths[i]]=continuous_frechet_curves[i][j];					
					snapped_curve_lengths[i]++;					
					
				}
				
			}
			
			temp_curve[snapped_curve_lengths[i]]=continuous_frechet_curves[i][d-1];
			snapped_curve_lengths[i]++;
			
			// Twra 8a prepei na ginei h snap sto grid
			for(j=0;j<snapped_curve_lengths[i];j++){
				
				continuous_frechet_curves[i][j]=floor(temp_curve[j]/delta)*delta;
								
			}
			
			// Diathrhsh minima kai maxima sto temp antigrafo
			// To mhkos endexetai na allaksei epomenws 8a epanypologistei
			continuous_frechet_curve_length=snapped_curve_lengths[i];
								
			// Ksekiname me to prwto stoixeio  			
			temp_curve[0]=continuous_frechet_curves[i][0];
			snapped_curve_lengths[i]=1;
			j=1;
			
			
			while(j<continuous_frechet_curve_length){
				
				if(continuous_frechet_curves[i][j-1]<continuous_frechet_curves[i][j+1]){
					
					minima=continuous_frechet_curves[i][j-1];
					maxima=continuous_frechet_curves[i][j+1];
					
				}
				else{
					
					minima=continuous_frechet_curves[i][j+1];
					maxima=continuous_frechet_curves[i][j-1];					
					
				}
				
				// Periptwsh apaleifhs
				if(continuous_frechet_curves[i][j]>=minima && continuous_frechet_curves[i][j]<=maxima){
					
					temp_curve[snapped_curve_lengths[i]]=continuous_frechet_curves[i][j+1];

					snapped_curve_lengths[i]++;
					j+=2;	
					
				}
				else{
					
					// Periptwsh mh apaleifhs
					temp_curve[snapped_curve_lengths[i]]=continuous_frechet_curves[i][j];

					snapped_curve_lengths[i]++;
					j++;	
					
				}
				
			}
			
			// Akolou8ei h apaleifh diplotypwn;
			continuous_frechet_curve_length=0;
			
			continuous_frechet_curves[i][0]=temp_curve[0];
			continuous_frechet_curve_length++;
			
			for(j=1;j<snapped_curve_lengths[i];j++){
				
				if(temp_curve[j]!=temp_curve[j-1]){
					
					continuous_frechet_curves[i][continuous_frechet_curve_length]=temp_curve[j];
					continuous_frechet_curve_length++;		
					
				}
			}
			
			// Diadikasia oloklhrwnetai me to padding	
			for(j=continuous_frechet_curve_length;j<d;j++){
				
				continuous_frechet_curves[i][j]=bigM;
			}			
			
		}
	
		// Diavasma dianysmatwn apo to arxeio eisodoy
		if(CURVE_read_continuous_frechet_curves_from_file(query_file,&continuous_frechet_queries,&query_names,&num_of_queries,&d)<0){
			
			printf("Can not read input file!\n");
			exit(-1);
			
		}
			
		// Gia ka8e kampylh ginetai snap sto grid, meta apaloifh twn synexomenwn diplotypwn kai meta padding
		for(i=0;i<num_of_queries;i++){
			
			// Ksekiname me filtering ths kampylhs 			
			temp_curve[0]=continuous_frechet_queries[i][0];
			
			snapped_curve_lengths[i]=1;
			
			// Sto plaisio toy filtrarismatos epiviwnoyn oi times poy prepei, oi yploipes apaleifontai 
			for(j=1;j<d-1;j++){
				
				// Ypologismos twn dyo diadoxikwn apolytwn diaforwn toy filtering
				fabs_a_minus_b=continuous_frechet_queries[i][j]-continuous_frechet_queries[i][j-1];
				fabs_b_minus_c=continuous_frechet_queries[i][j+1]-continuous_frechet_queries[i][j];
				
				
				if(fabs_a_minus_b<0)
					fabs_a_minus_b=-fabs_a_minus_b;
				
				if(fabs_b_minus_c<0)
					fabs_b_minus_c=-fabs_b_minus_c;
				
				// Eggrafetai h paraleipetai (filtraretai) h epomenh timh symfwna me ton kanona toy
				// filtrarismatos
				if(!(fabs_a_minus_b<=epsilon && fabs_b_minus_c<=epsilon)){
					
					temp_curve[snapped_curve_lengths[i]]=continuous_frechet_queries[i][j];					
					snapped_curve_lengths[i]++;					
					
				}
				
			}
			
			temp_curve[snapped_curve_lengths[i]]=continuous_frechet_queries[i][d-1];
			snapped_curve_lengths[i]++;
			
			// Twra 8a prepei na ginei h snap sto grid
			for(j=0;j<snapped_curve_lengths[i];j++){
				
				continuous_frechet_queries[i][j]=floor(temp_curve[j]/delta)*delta;
								
			}
			
			// Diathrhsh minima kai maxima sto temp antigrafo
			// To mhkos endexetai na allaksei epomenws 8a epanypologistei
			continuous_frechet_curve_length=snapped_curve_lengths[i];
			
			// Ksekiname me to prwto stoixeio  			
			temp_curve[0]=continuous_frechet_queries[i][0];
			snapped_curve_lengths[i]=1;
			j=1;
						
			while(j<continuous_frechet_curve_length){
				
				if(continuous_frechet_queries[i][j-1]<continuous_frechet_queries[i][j+1]){
					
					minima=continuous_frechet_queries[i][j-1];
					maxima=continuous_frechet_queries[i][j+1];
					
				}
				else{
					
					minima=continuous_frechet_queries[i][j+1];
					maxima=continuous_frechet_queries[i][j-1];					
					
				}
				
				// Periptwsh apaleifhs
				if(continuous_frechet_queries[i][j]>=minima && continuous_frechet_queries[i][j]<=maxima){
					
					temp_curve[snapped_curve_lengths[i]]=continuous_frechet_queries[i][j+1];

					snapped_curve_lengths[i]++;
					j+=2;	
					
				}
				else{
					
					// Periptwsh mh apaleifhs
					temp_curve[snapped_curve_lengths[i]]=continuous_frechet_queries[i][j];

					snapped_curve_lengths[i]++;
					j++;	
					
				}
				
			}
			
			// Akolou8ei h apaleifh diplotypwn;
			continuous_frechet_curve_length=0;
			
			continuous_frechet_queries[i][0]=temp_curve[0];
			continuous_frechet_curve_length++;
			
			for(j=1;j<snapped_curve_lengths[i];j++){
				
				if(temp_curve[j]!=temp_curve[j-1]){
					
					continuous_frechet_queries[i][continuous_frechet_curve_length]=temp_curve[j];
					continuous_frechet_curve_length++;		
					
				}
			}
			
			// Diadikasia oloklhrwnetai me to padding	
			for(j=continuous_frechet_curve_length;j<d;j++){
				
				continuous_frechet_queries[i][j]=bigM;
			}			
					
		}
		
		// Opws exei ginei safes kai sth syzhthsh doyleyoyme me 1 pinaka katakermatismoy ara L=1
		L=1;
		
		// Dhmioyrgia synarthshs katakermatismoy
		g=HASH_create_hash_functions(L,k,d,lsh_w);

		// Dhmioyrgia timwn r
		r_values=HASH_create_r_values(d);
				
		// Ypologismos ari8moy buckets 
		buckets=num_of_curves/divider;
		
		// Desmeyoyme mnhmh gia ton pinaka hashtables poy ylopoiei toys pinakes katakermatismoy 
		lsh_hashtables=(void***)malloc(L*sizeof(void**));
		for(i=0;i<L;i++)
			lsh_hashtables[i]=(void**)malloc(buckets*sizeof(void*));
		
		// Gia ka8e kado dhmioyrgoyme mia lista
		for(i=0;i<L;i++)
			for(j=0;j<buckets;j++)
				lsh_hashtables[i][j]=LIST_create();
		
		// Ka8e snapped kampylh katakermatizetai me ka8e g_i kai mpainei stoys antistoixous kadous
		for(i=0;i<L;i++){
			for(j=0;j<num_of_curves;j++){
				
				// Ypologismos bucket ths kampylhs i gia ton pinaka j
				bucket_number=HASH_g_i(g[i],continuous_frechet_curves[j],r_values,lsh_w,d,M_LSH,k,buckets);
						
				// Eisagwgh ths kampylhs sto bucket poy proekypse 
				LIST_insert((list*)(lsh_hashtables[i][bucket_number]),j);
			}	
		}
	
		// Twra metatrepoyme ta buckets apo listes se dynamikoys pinakes gia na beltiwsoyme 
		// thn taxythta.
		for(i=0;i<L;i++){
			
			for(j=0;j<buckets;j++){
					
				// Kataskeyh bucket apo th sxetikh lista
				bucket_ptr=(bucket*)LIST_convert_to_bucket((list*)(lsh_hashtables[i][j]));
				
				// Adeiasma listas
				LIST_empty((list*)(lsh_hashtables[i][j]));
				
				// Apeley8erwsh xwroy listas
				free(lsh_hashtables[i][j]);
				
				// Ekxwrhsh bucket sthn antistoixh 8esh
				lsh_hashtables[i][j]=bucket_ptr;
						
			}
		
		}

		fp=fopen(output_file,"w");
		free(temp_curve);
		// Mhdenismos a8roismatwn xronwn poy prokeitai na ypologistoyn
		sumTimeApproximate=0;
		sumTimeTrue=0;
			
		// Arxikopoihsh megistoy approximate factor
		maxApproximateFactor=0;
	
		// Arxikopoihsh shmaias h opoia otan ypsw8ei o megistos approximate factor 8ewreitai oti teinei sto apeiro
		approximateFactorInfinityFlag=0;
					
		// Arxikopoihsh ths kontinoterhs kampylhs kata discrete frechet me lsh 
		closest_lsh_continuous_frechet_index=-1;		
				
		// Epeksergasia ka8e query		
		for(q=0;q<num_of_queries;q++){
			
			// Enarksh xronometroy xronoy lsh
			start_tm = high_resolution_clock::now();
			
			// Arxikopoihsh megethoys pinaka sygxwneyshs kadwn
			//merge_bucket_length=0;
				
			// Gia ka8e pinaka katakermatismoy...
			for(j=0;j<L;j++){
				
				// ... fernoyme to antistoixo snapped query sthn katallhlh morfh.
				queryPoints= new Points(2);
				
				for(i=0;i<d;i+=2){
										
					queryPt = new Point(2);
					
					queryPt->set(0,continuous_frechet_queries[q][2*i]);
					queryPt->set(1,continuous_frechet_queries[q][2*i+1]);
					
					queryPoints->add(*queryPt);
					
					delete queryPt;
				}
				
				queryCurve=new Curve(*queryPoints,query_names[q]);
										
				// Ypologismos ari8moy bucket toy query gia ton trexonta pinaka katakermatismoy
				bucket_number=HASH_g_i(g[j],continuous_frechet_queries[q],r_values,lsh_w,d,M_LSH,k,buckets);
				
				// Eyresh toy idiou tou bucket
				bucket_ptr=(bucket*)lsh_hashtables[j][bucket_number];
							
				// Sarwnontai oi kampyles toy bucket			
				for(v=0;v<bucket_ptr->length;v++){
					
					// Fernoyme thn ekastote kampylh sthn katallhlh morfh
					candidatePoints= new Points(2);
		
					for(i=0;i<d;i+=2){
						
						candidatePt = new Point(2);
						
						candidatePt->set(0,continuous_frechet_curves[bucket_ptr->keys[v]][2*i]);
						candidatePt->set(1,continuous_frechet_curves[bucket_ptr->keys[v]][2*i+1]);
						
						candidatePoints->add(*candidatePt);
						
						delete candidatePt;
					}
					
					
					candidateCurve=new Curve(*candidatePoints,curve_names[bucket_ptr->keys[v]]);
					
					// Ypologizetai h apostash kata continuous frechet toy query apo thn kampylh
					c_dst = Frechet::Continuous::distance(*queryCurve,*candidateCurve);
					
					delete candidateCurve;
		
					continuous_frechet_distance = c_dst.value;
					
					// Periptwsh arxikopoihshs elaxisths continuous frechet apostashs
					if(closest_lsh_continuous_frechet_index<0){
						
						min_lsh_continuous_frechet_distance=continuous_frechet_distance;
						
						closest_lsh_continuous_frechet_index=bucket_ptr->keys[v];
						
						
					}
					else{
					// Periptwsh enhmerwshs elaxisths continuous frechet apostashs	
						
						if(continuous_frechet_distance<min_lsh_continuous_frechet_distance){
							
							min_lsh_continuous_frechet_distance=continuous_frechet_distance;
						
							closest_lsh_continuous_frechet_index=bucket_ptr->keys[v];	
							
							
						}
												
					}					
															
				}				
				
			}	
			
			// Lhksh xronometroy xronoy lsh continuous frechet
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			lsh_time = duration.count();

			// Enarksh xronometroy xronoy eksantlhtikhs anazhthshs
			start_tm = high_resolution_clock::now();
			
			// Arxikopoihsh ths pragmatika kontinoterhs kampylhs kata continuous frechet
			closest_continuous_frechet_index=-1;
			
			for(v=0;v<L;v++){
				
				
				// Ypologismos apostasewn toy trexontos query me ka8ena apo ta ypoloipa dianysmata
				for(i=0;i<num_of_curves;i++){		
			
					// Fernoyme thn ekastote kampylh sthn katallhlh morfh
					candidatePoints= new Points(2);
		
					for(j=0;j<d;j+=2){
						
						candidatePt = new Point(2);
						
						candidatePt->set(0,continuous_frechet_curves[i][2*j]);
						candidatePt->set(1,continuous_frechet_curves[i][2*j+1]);
						
						candidatePoints->add(*candidatePt);
						
						delete candidatePt;
					}
					
					
					candidateCurve=new Curve(*candidatePoints,curve_names[bucket_ptr->keys[v]]);
					
					// Ypologizetai h apostash kata continuous frechet toy query apo thn kampylh
					c_dst = Frechet::Continuous::distance(*queryCurve,*candidateCurve);
					
					delete candidateCurve;
					
					continuous_frechet_distance = c_dst.value;
					
					// Periptwsh arxikopoihshs elaxisths continuous frechet apostashs
					if(closest_continuous_frechet_index<0){
						
						min_continuous_frechet_distance=continuous_frechet_distance;
						
						closest_continuous_frechet_index=i;
						
						
					}
					else{
					// Periptwsh enhmerwshs elaxisths continuous frechet apostashs	
						
						if(continuous_frechet_distance<min_continuous_frechet_distance){
							
							min_continuous_frechet_distance=continuous_frechet_distance;
						
							closest_continuous_frechet_index=i;	
							
						}
												
					}
				
				}
			
			}			
			// Lhksh xronometroy xronoy eksantlhtikhs anazhthshs	
			stop_tm = high_resolution_clock::now();	
			duration = duration_cast<microseconds>(stop_tm - start_tm);	
			brutal_search_time = duration.count();
										
			fprintf(fp,"Query: %s\n",query_names[q]);			
			fprintf(fp,"Algorithm: LSH_Frechet_Continuous\n");
			fprintf(fp,"Approximate Nearest neighbor: %s\n",curve_names[closest_lsh_continuous_frechet_index]);
			fprintf(fp,"True Nearest neighbor: %s\n",curve_names[closest_continuous_frechet_index]);
			fprintf(fp,"distanceApproximate: %.6lf\n",min_lsh_continuous_frechet_distance);
			fprintf(fp,"distanceTrue: %.6lf\n",min_continuous_frechet_distance);
							
			// Ypologismos toy approximate factor ths trexoysas periptwshs
			
			// An h pragmatikh apostash den einai mhdenikh ypologizetai o approximate factor
			// apo to phliko ths proseggistikhs apostashs dia thn pragmatikh
			if(min_continuous_frechet_distance!=0){
				
				approximateFactor=min_lsh_continuous_frechet_distance/min_continuous_frechet_distance;
				
			}
			else{
			// Diaforetika an h pragmatikh apostash einai mhdenikh exoyme dyo ypoperiptwseis
				
				// Ean kai h proseggistikh einai mhden, o approximate factor ti8etai mhden
				if(min_lsh_continuous_frechet_distance==0)
					approximateFactor=0;
				// Diaforetika o approximate factor apaeirizetai opote ypzwnetai h sxetikh shmaia
				else
					approximateFactorInfinityFlag=1;
			}
			
			// Enhmerwsh toy megistoy approximate factor
			if(approximateFactorInfinityFlag!=1){
				
				if(approximateFactor>maxApproximateFactor)
					maxApproximateFactor=approximateFactor;
				
				
			}
						
			// Enhmerwsh a8roismatos xronwn proseggistikhs anazhthshs
			sumTimeApproximate+=lsh_time;
			
			// Enhmerwsh a8roismatos xronwn eksantlhtikhs anazhthshs
			sumTimeTrue+=brutal_search_time;
			
			delete queryCurve;
					
		}
		
		
		// Ypologismos kai typwsh toy mesoy xronoy proseggistikhs anazhthshs
		fprintf(fp,"tApproximateAverage: %.3lf microseconds\n",sumTimeApproximate/num_of_queries);
		
		// Ypologismos kai typwsh toy mesoy xronoy eksanlthtikhs anazhthshs
		fprintf(fp,"tTrueAverage: %.3lf microseconds\n",sumTimeTrue/num_of_queries);	
				
			
		// An o maximum approximate factor exei apeiristh ws timh toy sto arxei grafetai h symboloseira "infinity"
		if(approximateFactorInfinityFlag==1){
			
			fprintf(fp,"MAF: infinity\n");
			
		}
		else{
		// Diaforetika grafetai h timh poy exei ypologistei

			fprintf(fp,"MAF: %.3lf \n",maxApproximateFactor);			
			
		}
		
		fclose(fp);
		// Apodesmeysh mnhmhs
		CURVE_free_continuous_frechet_curves(continuous_frechet_curves,num_of_curves,d);
		CURVE_free_continuous_frechet_curves(continuous_frechet_queries,num_of_queries,d);
		
		// Apeley8erwsh synarthsewn katakermatismoys
		HASH_free_hash_functions(g,L,k);
	
		// Apodesmeysh onomatwn twn dianysmatwn
		for(i=0;i<num_of_curves;i++)
			free(curve_names[i]);
		
		free(curve_names);
		
		// Apodesmeysh onomatwn twn queries
		for(i=0;i<num_of_queries;i++)
			free(query_names[i]);
		
		free(query_names);
		
		// Apodesmeysh twn r_values
		free(r_values);
		
		
		// Apodesmeysh toy hashtable toy lsh
		// Apeley8erwsh mnhmhs toy pinaka lsh_hashtables poy ylopoiei toys pinakes katakermatismoy 
		for(i=0;i<L;i++){
			for(j=0;j<buckets;j++){
				
				free(((bucket*)(lsh_hashtables[i][j]))->keys);
				free(lsh_hashtables[i][j]);
				
			}
			free(lsh_hashtables[i]);		
			
		}
		free(lsh_hashtables);	
		
		
		exit(0);
		
	}
	
	exit(0);
	
}

