#include "assign.h"

// Synarthsh assign lloyd
int ASSIGN_lloyd(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int), int* cluster_center_of_vector, double* distance_from_cluster_center, int* cluster_centers, int K, int d,list** clusters){
	
	register int i,j;
	
	// Ari8mos kentrou cluster
	int cluster_center;
	
	// Boh8htikh metablhth apostashs
	double distance;
	
	// Shmaia
	int flag;

	// Pinakas 1 pros 1 me ta dianysmata.
	// Krata to index toy pinaka cluster_centers toy kentroy toy cluster poy mpainei to dianysma alla
	int* cluster_indexes;
		
	cluster_indexes=malloc(num_of_vectors*sizeof(int));	

	// Ta kentra twn cluster arxikopoioyntai sto 0.
	// Oi antistoixes apostaseis sto DBL_MAX
	for(i=0;i<num_of_vectors;i++){
		
		cluster_center_of_vector[i]=0; 
		distance_from_cluster_center[i]=DBL_MAX; 
		
	}
	
	// Sarwnontai ta dianysmata. Ka8e ena 8a mpei sto cluster toy kontinoterou cluster center
	for(i=0;i<num_of_vectors;i++){
	
		// Apo ekei kai pera ypologizontai oi apostaseis apo ta ypoloipa kentra kai epilegetai to kentroy
		// pou apexei ligotero apo ta ypoloipa
		for(j=0;j<K;j++){
						
		
			distance = metric(vectors[cluster_centers[j]-1],vectors[i],d);	
			
			if(distance<distance_from_cluster_center[i]){
				
				distance_from_cluster_center[i] = distance;
				cluster_center_of_vector[i] = cluster_centers[j];
				
				cluster_indexes[i]=j;
			}
			
		}

	}

	// Enhmerwsh listas dianysmatwn ka8e cluster
	for(i=0;i<num_of_vectors;i++)		
		LIST_insert(clusters[cluster_indexes[i]],i+1);
	
	free(cluster_indexes);
	
}

// Synarthsh assign reverse R me lsh
int ASSIGN_reverse_R_lsh(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int),void*** lsh_hashtables, int* cluster_center_of_vector, double* distance_from_cluster_center, int* cluster_centers, int** buckets_of_current_centers,double*** g,int* m_powers, int L, int K,int d,double lsh_w, int M,int LSH_k,int buckets,list** clusters){
	
	register int i,j,v;
	
	// Boh8htikh metablhth
	int current_center_index;
	
	// Mege8os kadoy
	int bucket_length;
	
	// Ari8mos kentrou cluster
	int cluster_center;
	
	// Boh8htikh metablhth apostashs
	double distance;
	
	// Elaxisths kai megisth apostash metaksy dyo kentrwn
	double min_center_distance=DBL_MAX;
	double max_center_distance=0;
	
	// Aktina antisrofhs ekxwrhshs
	double R;
	
	// Plh8os diasysmatwn se ena bucket
	int num_of_vectors_in_bucket;
		
	// Metrhths dianysmatwn gia ta opoia exei ginei assign
	int assigned_vectors_counter;
	
	// Deikths se bucket
	bucket* bucket_ptr;
		
	// Boh8htikh metablhth
	int current_vector_index;
	
	// Pinakas 1 pros 1 me ta dianysmata.
	// Krata to index toy pinaka cluster_centers toy kentroy toy cluster poy mpainei to dianysma alla
	int* cluster_indexes;
		
	cluster_indexes=malloc(num_of_vectors*sizeof(int));	
	
	// Arxikopoish ths elaxisths kai ths megisths apostashs metaksy dyo kentrwn
	min_center_distance=DBL_MAX;
	max_center_distance=0;	
	
	// Briskoyme oloys toys kadoys ka8e kentroy. Xreiazontai giati apo ekei 8a antlhsoyme dianysmata 
	// sthn antistrofh ana8esh
	for(i=0;i<K;i++){
		for(j=0;j<L;j++){
			
			// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
			buckets_of_current_centers[i][j]=HASH_g_i(g[j],vectors[cluster_centers[i]-1],m_powers,lsh_w,d,M,LSH_k,buckets);
			
		}	
	}
	
	// Gia ka8e kentro...
	for(i=0;i<K-1;i++){
		
		// ...ypologizetai h apostash toy apo ola ta ypoloipa...
		for(j=i+1;j<K;j++){
			
			distance=METRIC_L2(vectors[cluster_centers[i]-1],vectors[cluster_centers[j]-1], d);
			
			// An h apostash einai mikroterh apo thn elaxisth enhmerwnetai h elaxisth
			if(distance<min_center_distance)
				min_center_distance=distance;
			
			// An h apostash einai megalyterh apo thn megisth enhmerwnetai h megisth
			if(distance>min_center_distance)
				max_center_distance=distance;
			
			
		}
		
	}
		
	// Arxikopoioyme thn apostash ekastoy dianysmatos apo to  kentro (sthn oysia cluster)
	// Den xreiazetai arxikopoihsh kai toy kentroy dioti 8a allaksei sthn prwth sygkrish
	for(i=0;i<num_of_vectors;i++){
		
		distance_from_cluster_center[i]=DBL_MAX;
		
	}
	
	// H aktina arxikopoieitai sto miso ths elaxisths apostashs metaksy dyo kentrwn
	R=min_center_distance/2;
	
	// Arxikopoieitai o metrhths dianysmatwn poy exoyn ginei assign
	assigned_vectors_counter=0;
	
	// Psaxnoyme diplasiazontas thn aktina mexri ayth na ginei to miso ths megisths apostashs metaksy dyo kentrwn 
	// 'h ta ginoyn assign ola ta dianysmata
	while(R<2*max_center_distance){
		
		/// Briskoyme oloys toys kadoys ka8e kentroy. Xreiazontai giati apo ekei 8a antlhsoyme dianysmata 
		// sthn antistrofh ana8esh
		for(i=0;i<K;i++){
			
			// Ypologizoyme th 8esh toy trexontos kentroy ston pinaka twn dianysmatwn
			current_center_index=cluster_centers[i]-1;
			
			for(j=0;j<L;j++){
				
				bucket_ptr = lsh_hashtables[j][buckets_of_current_centers[i][j]];
				
				bucket_length=bucket_ptr->length;
				for(v=0;v<bucket_length;v++){
					
					// Ypologizoyme th 8esh toy trexontos dianysmatos pou 8eloume na kanoyme assign ston pinaka twn dianysmatwn
					current_vector_index=bucket_ptr->keys[v]-1;
					
					distance=METRIC_L2(vectors[current_vector_index],vectors[current_center_index],d);
					
					if(distance<R){
					
						// An to dianysma den exei ginei assign, ginetai kai ayksanetai o antistoixos metrhths
						if( distance_from_cluster_center[current_vector_index]==DBL_MAX){
														
							cluster_center_of_vector[current_vector_index]=current_center_index+1;
							distance_from_cluster_center[current_vector_index]=distance;
							
							cluster_indexes[current_vector_index]=i;
							
							assigned_vectors_counter++;
														
						}
						// Diaforetika eksetazetai an prepei na ginei enhmerwsh
						else if(distance<distance_from_cluster_center[current_vector_index]){
							
							cluster_center_of_vector[current_vector_index]=current_center_index+1;
							distance_from_cluster_center[current_vector_index]=distance;
							
							cluster_indexes[current_vector_index]=i;
														
						}
					
					}
				}
				
			}	
			
		}
						
		if(assigned_vectors_counter==num_of_vectors)
			break;
		R*=2;
		
		
	}
	
	// An den exoyn ginei assign ola ta dianysmata kanoyme assign ayta poy den exoyn ginei
	if(assigned_vectors_counter!=num_of_vectors){
		
		// Gia ka8e dianysma...
		for(i=0;i<num_of_vectors;i++){
			
			// ...eksetazetai an exei ginei assign 'h oxi. An den exei ginei...
			if(distance_from_cluster_center[i]==DBL_MAX){
				
				//...ypologizontai oi apostaseis apo ta kentra kai epilegetai to kontinoterou
				for(j=0;j<K;j++){
					
					distance=METRIC_L2(vectors[i],vectors[cluster_centers[j]-1],d);
					
					if(distance<distance_from_cluster_center[i]){
						
						cluster_center_of_vector[i]=cluster_centers[j];
						distance_from_cluster_center[i]=distance;
						
						cluster_indexes[i]=j;
													
					}
								
				}
				
				assigned_vectors_counter++;					
			}					
		}			
	}

	// Enhmerwsh listas dianysmatwn ka8e cluster
	for(i=0;i<num_of_vectors;i++)		
		LIST_insert(clusters[cluster_indexes[i]],i+1);
		
	free(cluster_indexes);
		
}

// Synarthsh assign reverse R me hypercube
int ASSIGN_reverse_R_hypercube(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int),void** hypercube, int** cube_hashtables, int* cluster_center_of_vector, double* distance_from_cluster_center, int* cluster_centers, int* vertices_of_current_centers,int HYPERCUBE_probes,int** probes_array,double*** g,int* m_powers,int K,int d,double cube_w,int M, int HYPERCUBE_k,int cube_num_of_hi_functions,int buckets,list** clusters){
	
	// Boh8htikes metablhtes
	register int i,j,v;
	
	// Boh8htikh metablhth apostashs
	double distance;
	
	// Elaxisths kai megisth apostash metaksy dyo kentrwn
	double min_center_distance=DBL_MAX;
	double max_center_distance=0;
	
	// Aktina antisrofhs ekxwrhshs
	double R;
	
	// To plh8os twn dianysmatwn poy exoyn antistoixh8ei se mia koryfh
	int num_of_vectors_in_vertex;
	
	// Metrhths dianysmatwn gia ta opoia exei ginei assign
	int assigned_vectors_counter;
	
	// Ari8mos kadoy
	int bucket_number;
	
	// Boh8htikos deikths
	bucket* bucket_ptr;
	
	// Boh8htikoi ari8modeiktes
	int current_center_index;
	int current_vector_index;
	
	// Pinakas 1 pros 1 me ta dianysmata.
	// Krata to index toy pinaka cluster_centers toy kentroy toy cluster poy mpainei to dianysma alla
	int* cluster_indexes;
			
	cluster_indexes=malloc(num_of_vectors*sizeof(int));	
	
	// Ka8e kentro katakermatizetai me ka8e g_i kai ypologizetai h koryfh toy yperkyboy 
	// sthn opoia eisagetai
	for(i=0;i<K;i++){
		
		vertices_of_current_centers[i]=0;
		for(j=0;j<HYPERCUBE_k;j++){
			
			// Ypologismos bucket toy dianysmatos i+1 gia ton pinaka j
			bucket_number=HASH_g_i(g[j],vectors[i],m_powers,cube_w,d,M,cube_num_of_hi_functions,buckets);
		
			// Sxhmatismos toy ari8moy koryfhs
			vertices_of_current_centers[i]+=vertices_of_current_centers[i]+cube_hashtables[j][bucket_number];
			
		}	
		
					
	}
	
	// Gia ka8e kentro...
	for(i=0;i<K-1;i++){
		
		// ...ypologizetai h apostash toy apo ola ta ypoloipa...
		for(j=i+1;j<K;j++){
			
			distance=METRIC_L2(vectors[cluster_centers[i]-1],vectors[cluster_centers[j]-1], d);
			
			// An h apostash einai mikroterh apo thn elaxisth enhmerwnetai h elaxisth
			if(distance<min_center_distance)
				min_center_distance=distance;
			
			// An h apostash einai megalyterh apo thn megisth enhmerwnetai h megisth
			if(distance>min_center_distance)
				max_center_distance=distance;
			
			
		}
		
	}
	
	// Arxikopoioyme thn apostash ekastoy dianysmatos apo to  kentro (sthn oysia cluster)
	// Den xreiazetai arxikopoihsh kai toy kentroy dioti 8a allaksei sthn prwth sygkrish
	for(i=0;i<num_of_vectors;i++){
		
		distance_from_cluster_center[i]=DBL_MAX;
		
	}
	
	// H aktina arxikopoieitai sto miso ths elaxisths apostashs metaksy dyo kentrwn
	R=min_center_distance/2;
	
	// Arxikopoieitai o metrhths dianysmatwn poy exoyn ginei assign
	assigned_vectors_counter=0;
	
	while(R<max_center_distance){
				
		// Gia ka8e kentro
		for(i=0;i<K;i++){				
			
			for(j=0;j<HYPERCUBE_probes+1;j++){
							
				bucket_ptr=((bucket*)(hypercube[probes_array[vertices_of_current_centers[i]][j]]));
				
				num_of_vectors_in_vertex=bucket_ptr->length; 
				
				current_center_index=cluster_centers[i]-1;
				
				
				for(v=0;v<num_of_vectors_in_vertex;v++){
					
					// Ypologizoyme th 8esh toy trexontos dianysmatos pou 8eloume na kanoyme assign ston pinaka twn dianysmatwn
					current_vector_index=bucket_ptr->keys[v]-1;
					
					distance=METRIC_L2(vectors[current_vector_index],vectors[current_center_index],d);
					
					if(distance<R){
					
						// An to dianysma den exei ginei assign, ginetai kai ayksanetai o antistoixos metrhths
						if( distance_from_cluster_center[current_vector_index]==DBL_MAX){
														
							cluster_center_of_vector[current_vector_index]=current_center_index+1;
							distance_from_cluster_center[current_vector_index]=distance;
								
							cluster_indexes[current_vector_index]=i;
								
							assigned_vectors_counter++;
														
						}
						// Diaforetika eksetazetai an prepei na ginei enhmerwsh
						else if(distance<distance_from_cluster_center[current_vector_index]){
							
							cluster_center_of_vector[current_vector_index]=current_center_index+1;
							distance_from_cluster_center[current_vector_index]=distance;
							
							cluster_indexes[current_vector_index]=i;
														
						}
					
					}
					
				}			
			
			}

		}
						
		R*=2;
		
		if(assigned_vectors_counter==num_of_vectors)
			break;
		
	}
			
	// An den exoyn ginei assign ola ta dianysmata kanoyme assign ayta poy den exoyn ginei
	if(assigned_vectors_counter!=num_of_vectors){
		
		// Gia ka8e dianysma...
		for(i=0;i<num_of_vectors;i++){
			
			// ...eksetazetai an exei ginei assign 'h oxi. An den exei ginei...
			if(distance_from_cluster_center[i]==DBL_MAX){
									
				//...ypologizontai oi apostaseis apo ta kentra kai epilegetai to kontinoterou
				for(j=0;j<K;j++){
					
					distance=METRIC_L2(vectors[i],vectors[cluster_centers[j]-1],d);
					
					if(distance<distance_from_cluster_center[i]){
						
						cluster_center_of_vector[i]=cluster_centers[j];
						distance_from_cluster_center[i]=distance;
						
						cluster_indexes[i]=j;
					}
								
				}
				
				assigned_vectors_counter++;					
			}					
		}			
	}

	// Enhmerwsh listas dianysmatwn ka8e cluster
	for(i=0;i<num_of_vectors;i++)		
		LIST_insert(clusters[cluster_indexes[i]],i+1);
	
	free(cluster_indexes);
		
}



