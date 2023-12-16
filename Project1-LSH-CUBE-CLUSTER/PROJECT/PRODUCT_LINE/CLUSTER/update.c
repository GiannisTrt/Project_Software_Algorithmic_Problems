#include "update.h"

// Arxikopoihsh ths systadopoihshs me kmeans++
int UPDATE_center(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int), int* cluster_centers,int K, int d,list** clusters){
	
	register int i,j, cluster_num;
	
	// To a8roisma twn apostasewn enos dianysmatos apo ola ta ypoloipa dianysmata toy cluster
	double sum_distance;
	
	// Deikths se kado
	bucket* bucket_ptr;
	
	// Plh8os dianysmatwn se ena cluster
	int num_of_vectors_in_cluster;
	
	// Deikths pros ta dianysmata enos cluster (gia apofygh diplhs diey8ynsiodothshs kai aykshsh ths talythtas)
	int* vectors_in_cluster;
	
	// To elaxisto a8roisma twn apostasewn enos dianysmatos apo ola ta ypoloipa dianysmata toy cluster
	double min_sum_distance;
	
	double** matrix_distances;
	
	// Tp neo kentro ekastoy cluster
	int new_center;

	// Deikths se vector
	int* vct;

	// Metrhths allagwn kentrwn
	int center_changes_counter;

	center_changes_counter=0;
	
	// Twra metatrepoyme tis koryfes apo listes se buckets pou periexoyn dynamikoys pinakes 
	// gia na beltiwsoyme thn taxythta.
	for(i=0;i<K;i++){
		
		// Kataskeyh bucket apo th sxetikh lista
		bucket_ptr=LIST_convert_to_bucket(clusters[i]);
		
		// Adeiasma listas
		LIST_empty(clusters[i]);
		
		// Apeley8erwsh xwroy listas
		free(clusters[i]);
		
		// Ekxwrhsh bucket sthn antistoixh 8esh
		clusters[i]=(void*)bucket_ptr;
			
	}
		
	// Gia ka8e cluster...
	for(cluster_num=0;cluster_num<K;cluster_num++){
		
		// ...entopizetai to plh8os twn dianysmatwn...
		num_of_vectors_in_cluster=((bucket*)clusters[cluster_num])->length;
		
			// ...kai apospatai o pinakas me ta dianysmata (dhladh toys ari8moys twn dianysmatwn)
		vectors_in_cluster=((bucket*)clusters[cluster_num])->keys;
	
		// Desmeyetai xwros gia disdiastato pinaka apostasewn olwn twn dianysmatwn toy cluster 
		// apo ola ta ypoloipa dianysmata toy cluster
		matrix_distances=malloc(num_of_vectors_in_cluster*sizeof(double*));
		
		for(i=0;i<num_of_vectors_in_cluster;i++)
			matrix_distances[i]=malloc(num_of_vectors_in_cluster*sizeof(double));

		// Ypologizontai oi apostaseis olwn twn dianysmatwn toy cluster 
		// apo ola ta ypoloipa dianysmata toy claster
		// Wstoso gia logoys taxyhtas, 8a ypologistei mono to anw trigwniko meros toy pinaka.
		// To kate trigwniko 8a einai symmetriko toy anw.
		// Ayto shmainei oti den 8a exoyme diploys ypologismoys apostasewn.
		// Shmeiwnoyme oti tastoixeia ths diagwnioy einai mhdenika.
		for(i=0;i<num_of_vectors_in_cluster;i++){
			
			// Ekxwreitai ston deikth vct to dianysma toy opoioy 8a ypologistoyn oi aostaseis apo ta ypoloipa
			// (Gia logoys epitaxysnhs sto plaisio apofyghw ths polypepipedhs diey8ynsiodothshs)
			vct=vectors[vectors_in_cluster[i]-1];

			for(j=num_of_vectors_in_cluster-1;j>i;j--){
				
				matrix_distances[i][j]=metric(vct,vectors[vectors_in_cluster[j]-1],d); 
				matrix_distances[j][i]=matrix_distances[i][j];
				
			}	
		
			// To diagwnio stoixeio einai pantote mhdeniko (apostash dianysmatos apo ton eayto toy)
			matrix_distances[i][i]=0;
			
		}

		// Arxikopoihsh toy elaxistoy a8roismatos apostasewn dianysmatos apo ta ypoloipa
		min_sum_distance=DBL_MAX;
		
		for(i=0;i<num_of_vectors_in_cluster;i++){
			
			sum_distance=0;
			
			for(j=0;j<num_of_vectors_in_cluster;j++){
				
				sum_distance+=matrix_distances[i][j];
				
			}	
			
			if(sum_distance<min_sum_distance){
				
				min_sum_distance=sum_distance;
				new_center=vectors_in_cluster[i];
				
			}		
			
		}
		
		// An to kentro allaje ayksanetai o antistoixos metrhths
		if(new_center!=cluster_centers[cluster_num]){
			
			cluster_centers[cluster_num]=new_center;
			center_changes_counter++;
			
		}
			
	
		// Apeley8erwsh mnhmhs disdiastatoy boh8htikoy pinaka
		for(i=0;i<num_of_vectors_in_cluster;i++)
			free(matrix_distances[i]);
		free(matrix_distances);
		
	}
	
	
	return center_changes_counter;
	
}
