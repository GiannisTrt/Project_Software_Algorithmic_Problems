#include "cluster_args.h"


// Synarthsh diavasmatos orismatwn 
int CLUSTER_ARGS_get_cluster_args(int argc, char* argv[],char** input_file,char** configuration_file,char** output_file,int* complete,int* method){
    
	// Arxikopoihsh 8eshs twn shmaiwn ths grammhs entolwn
    int i_flag_position=0;
    int c_flag_position=0;
    int o_flag_position=0;
    int complete_flag_position=0;
    int m_flag_position=0;
 
	// Boh8htikes metavlhtes
    int i;
	int length;
    	
	// Boh8htikos pinakas
	char method_str[32]; // 32 xarakthres einai yper arketoi gia thn me8odo
	
	// 8a prepei na exoym 9 'h 10 orismata analoga me to an exei do8ei shmaia
	// complete 'h oxi
    if(argc!=9 && argc!=10){
        
        printf("Invalid number of arguments\n");
        return -1;
        
    }
   	
	// Ypologizoume tis 8eseis twn shmaiwn
    for(i=1;i<argc;i++){
        
        if(!strcmp(argv[i],"-i"))
            i_flag_position=i;
        else if(!strcmp(argv[i],"-c"))
            c_flag_position=i;
        else if(!strcmp(argv[i],"-o"))
            o_flag_position=i;
        else if(!strcmp(argv[i],"-complete"))
            complete_flag_position=i;        
        else if(!strcmp(argv[i],"-m"))
            m_flag_position=i;
		
    }
    		
    // An den exei do8ei shmaia arxeioy eisodoy epistrefetai sfalma
    if(i_flag_position==0){
        
        printf("Missing -i flag.\n");
        return -1;
        
    }
	// Diaforetika diey8eteitai h emmesh epistrofh ths diadromhs toy arxeioy eisodoy
	else
		*input_file = argv[i_flag_position+1];
	
    // An den exei do8ei configure arxeioy eisodoy epistrefetai sfalma
    if(c_flag_position==0){
        
        printf("Missing -c flag.\n");
        return -1;
        
    }
	// Diaforetika diey8eteitai h emmesh epistrofh ths diadromhs toy configure arxeioy
	else
		*configuration_file = argv[c_flag_position+1];
	
   	// An den exei do8ei shmaia arxeioy eksodoy epistrefetai sfalma
    if(o_flag_position==0){
        
        printf("Missing -o flag.\n");
        return -1;
        
    }
	// Diaforetika diey8eteitai h emmesh epistrofh ths diadromhs toy arxeioy eksodoy
	else
		*output_file = argv[o_flag_position+1];
	
	
	// An den exei do8ei shmaia me8odoy epistrefetai sfalma
    if(m_flag_position==0){
        
        printf("Missing -o flag.\n");
        return -1;
        
    }
	// Diaforetika elegxetai kai diey8eteitai h emmesh epistrofh ths me8doy
	// ypo morfh kwdikoy
	else{
		
		strncpy(method_str,argv[m_flag_position+1],31);
		
		// Metatrepoyme se peza tyxon kefalaia
		length=strlen(method_str);
		
		for(i=0;i<length;i++){
			
			if(method_str[i]<='Z')
				method_str[i]+=32;
			
		}
		
		if(strcmp(method_str,"classic")==0)
			*method=CLASSIC;
		else if(strcmp(method_str,"lsh")==0)
			*method=LSH;
		else if(strcmp(method_str,"hypercube")==0)
			*method=HYPERCUBE;
		else{
			
			printf("Invalid method input!\n");
			return -1;			
			
		}

	}
	
	// Ean exoyn do8ei 9 orismata kai exei do8ei shmaia complete epistrefetai sfalma
	if(argc==9 && complete_flag_position!=0){
		
		printf("Invalid syntax!\n");
		return -1;
		
	}
	
	// Ean exoyn do8ei 10 orismata alla den exei do8ei shmaia complete epistrefetai sfalma
	if(argc==10 && complete_flag_position==0){
		
		printf("Invalid syntax!\n");
		return -1;
	}	
	
	// Apofasizetai an exei do8ei h oxi shmaia complete 
	if(complete_flag_position==0)
		*complete=0;
	else
		*complete=1;
    return 0;
}


// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void CLUSTER_ARGS_print_cluster_args(char* input_file,char* configuration_file,char* output_file,int complete,int method){
	
	printf("input file:%s\n",input_file);	
	printf("configuration file:%s\n",configuration_file);	
	printf("output file:%s\n",output_file);	
	printf("complete flag: %d\n",complete);	
	printf("method: ");	
	if(method==CLASSIC)
		printf("Classic\n");
	else if(method==LSH)
		printf("LSH\n");
	else if(method==HYPERCUBE)
		printf("Hypercube\n");
		
}


// Synarthsh diavasmatos parametrwn apo to arxeio configuration 
int CLUSTER_ARGS_get_cluster_parameters(char* configuration_file,int default_K,int default_L,int default_LSH_k,int default_HYPERCUBE_M,int default_HYPERCUBE_k,int default_HYPERCUBE_probes,int* K,int *L,int *LSH_k,int *HYPERCUBE_M,int *HYPERCUBE_k,int *HYPERCUBE_probes,int* lsh_divider,unsigned int* lsh_m,double *lsh_w,unsigned int* cube_m, int* cube_divider,double* cube_w,int* cube_num_of_hi_functions,int* max_iterations){
	
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
			else if(!strcmp(phrase,"cube_num_of_hi_functions:"))
				*cube_num_of_hi_functions=value;
			else if(!strcmp(phrase,"max_iterations:"))
				*max_iterations=value;

		
		}
		
		fclose(fp);
		
		return 0;
	}
	
	return -1;
}

// Synarthsh emfanishs parametrwn (boh8htikh gia logoys elegxoy) 
void CLUSTER_ARGS_print_cluster_parameters(int K,int L,int LSH_k,int HYPERCUBE_M,int HYPERCUBE_k,int HYPERCUBE_probes,int max_iterations){
	
	printf("Cluster parameters:\n");
	printf("K = %d\n",K);	
	printf("L = %d\n",L);	
	printf("LSH_k = %d\n",LSH_k);	
	printf("HYPERCUBE_M = %d\n",HYPERCUBE_M);	
	printf("HYPERCUBE_k = %d\n",HYPERCUBE_k);	
	printf("HYPERCUBE_probes = %d\n",HYPERCUBE_probes);	
	printf("max_iterations = %d\n",max_iterations);	
			
}

