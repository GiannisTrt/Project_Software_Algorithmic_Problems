#include "search_args.h"

// Synarthsh diavasmatos orismatwn 
int ARGS_get_search_args(int argc, char* argv[],char** input_file,char** query_file,char** output_file,int* arlhorithm,int* k,int* L,int* M,int* probes,int* metric,double* delta,int default_lsh_k,int default_hypercube_k,int default_L,int default_M,int default_probes,int default_metric){
    
	// Arxikopoihsh 8eshs twn shmaiwn ths grammhs entolwn
    int i_flag_position=0;
    int q_flag_position=0;
    int k_flag_position=0;
    int L_flag_position=0;
    int o_flag_position=0;
    int M_flag_position=0;
	int delta_flag_position=0;
	int algorithm_flag_position=0;
	int probes_flag_position=0;
	int metric_flag_position=0;
    
	// Boh8htikh metavlhth
    int i;
    
	// Boh8htikh metavlhth
	int length;
	
	// Boh8htikos pinakas
	char argorithm_str[32]; // 32 xarakthres einai yper arketoi gia ton algori8mo
	
	// Boh8htikos pinakas
	char metric_str[32]; // 32 xarakthres einai yper arketoi gia thn metrikh
	
	// Prepei na dinontai ta <input file>, <output file>, <query file>, <algorithm>
	// Apo ekei kai pera:
	// Ston LSH dianysmatwn mporei na dinontai epipleon k kai L
	// Ston Hypercube dianysmatwn mporei na dinontai epipleon k, M, probes
	// Ston LSH kampylwn me Discreate Frechet dinetai epipleon delta,metric kai mporei na dinontai k kai L
	// Ston LSH kampylwn me Continous Frechet dinetai epipleon delta,metric kai mporei na dinontai k kai L
	// Telika:
	// Ston LSH dianysmatwn mporei na dinontai apo 9 mexri 13 orismata
	// Ston LSH kampylwn me Discreate Frechet mporei na dinontai apo 13 mexri 17 orismata
	// Ston LSH kampylwn me Continous Frechet mporei na dinontai apo 13 mexri 17 orismata
	
	// Ginetai enas arxikos elegxos plh8ous dianysmatwn
    if(argc!=9 && argc!=11 && argc!=13 && argc!=15 && argc!=17){
        
        printf("Invalid number of arguments\n");
        return -1;
        
    }
   	
	// Ypologizoume tis 8eseis twn ypoxrewtikwn shmaiwn
	for(i=1;i<argc;i+=2){
        
        if(!strcmp(argv[i],"-i"))
            i_flag_position=i;
        else if(!strcmp(argv[i],"-q"))
            q_flag_position=i;
        else if(!strcmp(argv[i],"-o"))
            o_flag_position=i;
        else if(!strcmp(argv[i],"-algorithm"))
            algorithm_flag_position=i;
    }
	
	
	// An den exei do8ei shmaia arxeioy eisodoy epistrefetai sfalma
    if(i_flag_position==0){
        
        printf("Missing -i flag.\n");
        return -1;
        
    }
	// Diaforetika diey8eteitai h emmesh epistrofh ths diadromhs toy arxeioy eisodoy
	else
		*input_file = argv[i_flag_position+1];
	
    // An den exei do8ei shmaia arxeioy eperwthsewn epistrefetai sfalma   
	if(q_flag_position<-1){
        
        printf("Missing -q flag.\n");
        return -1;
        
    }
	// Diaforetika diey8eteitai h emmesh epistrofh ths diadromhs toy arxeioy eperwthsewn
	else
		*query_file = argv[q_flag_position+1];
	
	
	// An den exei do8ei shmaia arxeioy eksodoy epistrefetai sfalma
    if(o_flag_position==0){
        
        printf("Missing -o flag.\n");
        return -1;
        
    }
	// Diaforetika diey8eteitai h emmesh epistrofh ths diadromhs toy arxeioy eksodoy
	else
		*output_file = argv[o_flag_position+1];
	
	
	// An den exei do8ei shmaia algori8moy epistrefetai sfalma
    if(algorithm_flag_position==0){
        
        printf("Missing -argorithm flag.\n");
        return -1;
        
    }
	// Diaforetika elegxetai kai diey8eteitai h emmesh epistrofh toy algori8moy
	// ypo morfh kwdikoy
	else{
		
		strncpy(argorithm_str,argv[algorithm_flag_position+1],31);
		
		// Metatrepoyme se peza tyxon kefalaia
		length=strlen(argorithm_str);
		
		for(i=0;i<length;i++){
			
			if(argorithm_str[i]<='Z')
				argorithm_str[i]+=32;
			
		}

		
		if(strcmp(argorithm_str,"lsh")==0)
			*arlhorithm=LSH;
		else if(strcmp(argorithm_str,"hypercube")==0)
			*arlhorithm=HYPERCUBE;
		else if(strcmp(argorithm_str,"frechet")==0)
			*arlhorithm=FRECHET;
		else{
			
			printf("Invalid argorithm input!\n");
			return -1;			
			
		}

	}	

	// Periptwsh lsh (aploy lsh gia dianysmata)
	if(*arlhorithm==LSH){

		// Sthn periptwsh ayth den mporoyme na exoyme 17 orismata
		if(argc==17){
			
			printf("Too many arguments for vertex LSH\n");
			return -1;
			
		}
		
		// An den exoyme problhma me to plh8os twn orismatwn, ypologizoume ta k, L an dinontai
		// 'h diaforetika dinoyme tis default times 
		// Ypologizoume tis 8eseis twn ypoxrewtikwn shmaiwn
		for(i=1;i<argc;i+=2){
			
			if(!strcmp(argv[i],"-k"))
				k_flag_position=i;
			else if(!strcmp(argv[i],"-L"))
				L_flag_position=i; 
						
		}
	
		// An den exei do8ei shmaia -k ekxwreitai sto k h proepilegmenh timh sfalma
		if(k_flag_position==0)
			*k=default_lsh_k;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*k=atoi(argv[k_flag_position+1]);
			
		// An to k einai mh 8etikos ari8mos epistrefetai sfalma
		if(*k<=0){
			
			printf("Invalid k value.\n");
			return -1;
			
		}

		// An den exei do8ei shmaia -L ekxwreitai sto L h proepilegmenh timh sfalma
		if(L_flag_position==0)
			*L=default_L;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*L=atoi(argv[L_flag_position+1]);
			
		// An to L einai mh 8etikos ari8mos epistrefetai sfalma
		if(*L<=0){
			
			printf("Invalid L value.\n");
			return -1;
			
		}

	
	}
	// Periptwsh hypecube (gia dianysmata)
	else if(*arlhorithm==HYPERCUBE){
		
		// An den exoyme problhma me to plh8os twn orismatwn, ypologizoume ta k, L an dinontai
		// 'h diaforetika dinoyme tis default times 
		// Ypologizoume tis 8eseis twn ypoxrewtikwn shmaiwn
		for(i=1;i<argc;i+=2){
			
			if(!strcmp(argv[i],"-k"))
				k_flag_position=i;
			else if(!strcmp(argv[i],"-M"))
				M_flag_position=i; 
			else if(!strcmp(argv[i],"-probes"))
				probes_flag_position=i; 
						
		}
		
		// An den exei do8ei shmaia -k ekxwreitai sto k h proepilegmenh timh sfalma
		if(k_flag_position==0)
			*k=default_hypercube_k;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*k=atoi(argv[k_flag_position+1]);
			
		// An to k einai mh 8etikos ari8mos epistrefetai sfalma
		if(*k<=0){
			
			printf("Invalid k value.\n");
			return -1;
			
		}
				
		// An den exei do8ei shmaia -M ekxwreitai sto M h proepilegmenh timh sfalma
		if(M_flag_position==0)
			*M=default_M;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*M=atoi(argv[M_flag_position+1]);
					
		// An to M einai mh 8etikos ari8mos epistrefetai sfalma
		if(*M<=0){
			
			printf("Invalid M value.\n");
			return -1;
			
		}	
		
		// An den exei do8ei shmaia -probes ekxwreitai sto probes h proepilegmenh timh sfalma
		if(probes_flag_position==0)
			*probes=default_probes;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*probes=atoi(argv[probes_flag_position+1]);
			
		// An to probes einai mh 8etikos ari8mos epistrefetai sfalma
		if(*probes<=0){
			
			printf("Invalid probes value.\n");
			return -1;
			
		}
		
	}
	else{
		
		
		// Se diaforetikh periptwsh den mporoyme na exoyme ligotera apo 13 orismata
		if(argc<13){
			
			printf("Too few arguments for curves LSH\n");
			return -1;
			
		}
		
		// An den exoyme problhma me to plh8os twn orismatwn, ypologizoume ta k, L an dinontai
		// 'h diaforetika dinoyme tis default times 
		// Ypologizoume tis 8eseis twn ypoxrewtikwn shmaiwn
		for(i=1;i<argc;i+=2){
			
			if(!strcmp(argv[i],"-k"))
				k_flag_position=i;
			else if(!strcmp(argv[i],"-L"))
				L_flag_position=i; 
						
		}
	
		// An den exei do8ei shmaia -k ekxwreitai sto k h proepilegmenh timh sfalma
		if(k_flag_position==0)
			*k=default_lsh_k;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*k=atoi(argv[k_flag_position+1]);
			
		// An to k einai mh 8etikos ari8mos epistrefetai sfalma
		if(*k<=0){
			
			printf("Invalid k value.\n");
			return -1;
			
		}

		// An den exei do8ei shmaia -L ekxwreitai sto L h proepilegmenh timh sfalma
		if(L_flag_position==0)
			*L=default_L;	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*L=atoi(argv[L_flag_position+1]);
			
		// An to L einai mh 8etikos ari8mos epistrefetai sfalma
		if(*L<=0){
			
			printf("Invalid L value.\n");
			return -1;
			
		}
				
		// Epishs 8a prepei opwsdhpote na orizetai metrikh kai delta
			
		// Ypologizoume tis 8eseis twn ypoxrewtikwn shmaiwn ths ypoperiptwshs
		for(i=1;i<argc;i+=2){
			
			if(!strcmp(argv[i],"-metric"))
				metric_flag_position=i;
			else if(!strcmp(argv[i],"-delta"))
				delta_flag_position=i;
			
		}
    		
		
		// An den exei do8ei shmaia metrikhs epilegetai discrete
		if(metric_flag_position==0){
			
		   *metric=DISCRETE;
			
		}
		// Diaforetika elegxetai kai diey8eteitai h emmesh epistrofh toy metrikhs
		// ypo morfh kwdikoy
		else{
			
			strncpy(metric_str,argv[metric_flag_position+1],31);
			
			// Metatrepoyme se peza tyxon kefalaia
			length=strlen(metric_str);
			
			for(i=0;i<length;i++){
				
				if(metric_str[i]<='Z')
					metric_str[i]+=32;
				
			}
			
			if(strcmp(metric_str,"discrete")==0)
				*metric=DISCRETE;
			else if(strcmp(metric_str,"continuous")==0)
				*metric=CONTINUOUS;
			else{
				
				printf("Invalid metric input!\n");
				return -1;			
				
			}

		}
		
		
		// An den exei do8ei shmaia -delta epistrefetai sfalma 
		if(delta_flag_position==0){
			
			printf("Missing -delta flag.\n");
			return -1;        
			
		}	
		// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
		else
			*delta=atof(argv[delta_flag_position+1]);
			
		// An to delta einai mh 8etikos ari8mos epistrefetai sfalma
		if(*delta<=0){
			
			printf("Invalid delta value.\n");
			return -1;
		}
		
	}

	return 0;	
	
}



void SEARCH_ARGS_print_search_args(char* input_file,char* query_file,char* output_file,int arlhorithm,int k,int L,int M,int probes,int metric,double delta){
		
	if(arlhorithm==LSH){
		
		printf("Case of LSH vectors.\n");
		printf("input file:%s\n",input_file);	
		printf("query file:%s\n",query_file);	
		printf("output file:%s\n",output_file);	
		printf("k=%d\n",k);	
		printf("L=%d\n",L);	
		printf("\n");	
		
	}
	else if(arlhorithm==HYPERCUBE){
		
		printf("Case of HYPERCUBE vectors.\n");
		printf("input file:%s\n",input_file);	
		printf("query file:%s\n",query_file);	
		printf("output file:%s\n",output_file);	
		printf("k=%d\n",k);	
		printf("M=%d\n",M);	
		printf("probes=%d\n",probes);	
		printf("\n");	
		
	}
	else if(arlhorithm==FRECHET){
		
		if(metric==DISCRETE){
			
			printf("Case of DISCRETE FRECHET curves.\n");
			printf("input file:%s\n",input_file);	
			printf("query file:%s\n",query_file);	
			printf("output file:%s\n",output_file);	
			printf("k=%d\n",k);	
			printf("L=%d\n",L);	
			printf("delta=%lf\n",delta);
			printf("\n");				
			
		}
		else if(metric==CONTINUOUS){
			
			printf("Case of CONTINUOUS FRECHET curves.\n");
			printf("input file:%s\n",input_file);	
			printf("query file:%s\n",query_file);	
			printf("output file:%s\n",output_file);	
			printf("k=%d\n",k);	
			printf("L=%d\n",L);	
			printf("delta=%lf\n",delta);			
			printf("\n");	
			
		}	
		
	}	
	
}



// Synarthsh diavasmatos parametrwn apo to arxeio configuration 
int SEARCH_ARGS_get_search_parameters(char* configuration_file,int default_K,int default_L,int default_LSH_k,int default_HYPERCUBE_M,int default_HYPERCUBE_k,int default_HYPERCUBE_probes,int* K,int *L,int *LSH_k,int *HYPERCUBE_M,int *HYPERCUBE_k,int *HYPERCUBE_probes,int* lsh_divider,unsigned int* lsh_m,double *lsh_w,unsigned int* cube_m, int* cube_divider,double* cube_w,int* max_iterations){
	
	// Deikths sto arxeio configuration 
	FILE* fp;
	
	// Symbololoseira gia diabasma frasewn apo to arxeio configuration
	// 128 xarakthres einai yper-arketoi
	char phrase[128];
	
	// Symbololoseira gia diabasma grammwn apo to arxeio configuration
	// 128 xarakthres einai yper-arketoi
	char line[128];	
	
	// Boh8htikh metablhth gia to diabasma ekasths akeraias timhs mesa sto arxeio configuration 
	unsigned int value;
		
	// Gia arxh arxikopoioyme tis parametroys stis default times
	*K=default_K;
	*L=default_L;
	*LSH_k=default_LSH_k;
	*HYPERCUBE_M=default_HYPERCUBE_M;
	*HYPERCUBE_k=default_HYPERCUBE_k;
	*HYPERCUBE_probes=default_HYPERCUBE_probes;
	
	// Sth syexei a 8a anoiksoyme to arxeio kai 8a tropopoihsoyme oses parametroys dinontai ekei
	
	// Anoigma arxeioy configuration
	fp=fopen(configuration_file,"r");
	
	// Efoson anoiksei to arxeio, oses parametroi periexontai se ayto 8a allaksoyn tis times toys
	// kai 8a paroyn tis times poy orizontai sto arxeio
	if(fp!=NULL){
		
		while(!feof(fp)){
						
			if(!fgets(line,127,fp))
				continue;
							
			sscanf(line,"%s %u",phrase,&value);
			
			if(!strcmp(phrase,"number_of_clusters:"))
				*K=value;
			else if(!strcmp(phrase,"number_of_vector_hash_tables:"))
				*L=value;
			else if(!strcmp(phrase,"number_of_vector_hash_functions:"))
				*LSH_k=value;
			else if(!strcmp(phrase,"max_number_M_hypercube:"))
				*HYPERCUBE_M=value;
			else if(!strcmp(phrase,"number_of_hypercube_dimensions:"))
				*HYPERCUBE_k=value;
			else if(!strcmp(phrase,"number_of_probes:"))
				*HYPERCUBE_probes=value;
			else if(!strcmp(phrase,"lsh_divider:"))
				*lsh_divider=value;
			else if(!strcmp(phrase,"lsh_m:"))
				*lsh_m=value;
			else if(!strcmp(phrase,"lsh_w:")){
			// Prosoxh! To 	lsh_w einai double, opote ksanadiabazetai
				sscanf(line,"%s %lf",phrase,lsh_w);
				
			}		
			else if(!strcmp(phrase,"cube_m:"))
				*cube_m=value;
			else if(!strcmp(phrase,"cube_divider:"))
				*cube_divider=value;
			else if(!strcmp(phrase,"cube_w:")){
			// Prosoxh! To 	cube_w einai double, opote ksanadiabazetai
				sscanf(line,"%s %lf",phrase,cube_w);
				
			}
			else if(!strcmp(phrase,"max_iterations:"))
				*max_iterations=value;

		
		}
		
		fclose(fp);
		
		return 0;
	}
	
	return -1;
}

// Synarthsh emfanishs parametrwn (boh8htikh gia logoys elegxoy) 
void SEARCH_ARGS_print_search_parameters(int K,int L,int LSH_k,int HYPERCUBE_M,int HYPERCUBE_k,int HYPERCUBE_probes,int max_iterations){
	
	printf("Cluster parameters:\n");
	printf("K = %d\n",K);	
	printf("L = %d\n",L);	
	printf("LSH_k = %d\n",LSH_k);	
	printf("HYPERCUBE_M = %d\n",HYPERCUBE_M);	
	printf("HYPERCUBE_k = %d\n",HYPERCUBE_k);	
	printf("HYPERCUBE_probes = %d\n",HYPERCUBE_probes);	
	printf("max_iterations = %d\n",max_iterations);	
			
}

