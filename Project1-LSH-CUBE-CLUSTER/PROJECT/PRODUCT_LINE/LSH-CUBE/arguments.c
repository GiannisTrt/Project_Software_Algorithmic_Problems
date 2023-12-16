#include "arguments.h"

// Synarthsh diavasmatos orismatwn 
int ARGS_get_lsh_args(int argc, char* argv[],char** input_file,char** query_file,int* k,int* L,char** output_file,int* N,double* R,int default_k,int default_L,int default_N,double default_R){
    
	// Arxikopoihsh 8eshs twn shmaiwn ths grammhs entolwn
    int i_flag_position=0;
    int q_flag_position=0;
    int k_flag_position=0;
    int L_flag_position=0;
    int o_flag_position=0;
    int N_flag_position=0;
	int R_flag_position=0;
    
	// Boh8htikh metavlhth
    int i;
    	
	// Mporei na mh dinontai ta k, L, N, R. Synepws to plh8os twn orismatwn mporei na einai
	// 7,9,11,13 'h 15
    if(argc!=7 && argc!=9 && argc!=11 && argc!=13 && argc!=15){
        
        printf("Invalid numper of arguments\n");
        return -1;
        
    }
   	
	// Ypologizoume tis 8eseis twn shmaiwn
    for(i=1;i<argc;i+=2){
        
        if(!strcmp(argv[i],"-i"))
            i_flag_position=i;
        else if(!strcmp(argv[i],"-q"))
            q_flag_position=i;
        else if(!strcmp(argv[i],"-k"))
            k_flag_position=i;
        else if(!strcmp(argv[i],"-L"))
            L_flag_position=i;        
        else if(!strcmp(argv[i],"-o"))
            o_flag_position=i;
		else if(!strcmp(argv[i],"-N"))
            N_flag_position=i;        
        else if(!strcmp(argv[i],"-R"))
            R_flag_position=i;
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
	

	// An den exei do8ei shmaia -k ekxwreitai sto k h proepilegmenh timh sfalma
	if(k_flag_position==0)
		*k=default_k;	
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
	
	// An den exei do8ei shmaia -N ekxwreitai sto N h proepilegmenh timh sfalma
	if(N_flag_position==0)
		*N=default_N;	
	// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
	else
		*N=atoi(argv[N_flag_position+1]);
		
	// An to N einai mh 8etikos ari8mos epistrefetai sfalma
	if(*N<=0){
        
        printf("Invalid N value.\n");
        return -1;
        
    }
	
	// An den exei do8ei shmaia -R ekxwreitai sto R h proepilegmenh timh sfalma
	if(R_flag_position==0)
		*R=default_R;	
	// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
	else
		*R=atof(argv[R_flag_position+1]);
		
	// An to R einai mh 8etikos ari8mos epistrefetai sfalma
	if(*R<=0){
        
        printf("Invalid R value.\n");
        return -1;
        
    }
	 
    return 0;
}

// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void ARGS_print_lsh_args(char* input_file,char* query_file,int k,int L,char* output_file,int N,double R){
	
	printf("input file:%s\n",input_file);	
	printf("query file:%s\n",query_file);	
	printf("output file:%s\n",output_file);	
	printf("k=%d\n",k);	
	printf("L=%d\n",L);	
	printf("N=%d\n",N);	
	printf("R=%lf\n",R);	
	
}

// Synarthsh diavasmatos orismatwn 
int ARGS_get_cube_args(int argc, char* argv[],char** input_file,char** query_file,int* k,int* M,char** output_file,int* N,double* R,int* probes,int default_k,int default_M,int default_N,double default_R,int default_probes){
    
	// Arxikopoihsh 8eshs twn shmaiwn ths grammhs entolwn
    int i_flag_position=0;
    int q_flag_position=0;
    int k_flag_position=0;
    int M_flag_position=0;
    int o_flag_position=0;
    int N_flag_position=0;
	int R_flag_position=0;
	int probes_flag_position=0;
    
	// Boh8htikh metavlhth
    int i;
    	
	// Mporei na mh dinontai ta k, M, N, R, probes. Synepws to plh8os twn orismatwn mporei na einai
	// 7,9,11,13,15 'h 17
    if(argc!=7 && argc!=9 && argc!=11 && argc!=13 && argc!=15 && argc!=17){
        
        printf("Invalid numper of arguments\n");
        return -1;
        
    }

	// Ypologizoume tis 8eseis twn shmaiwn
    for(i=1;i<argc;i+=2){
        
        if(!strcmp(argv[i],"-i"))
            i_flag_position=i;
        else if(!strcmp(argv[i],"-q"))
            q_flag_position=i;
        else if(!strcmp(argv[i],"-k"))
            k_flag_position=i;
        else if(!strcmp(argv[i],"-M"))
            M_flag_position=i;        
        else if(!strcmp(argv[i],"-o"))
            o_flag_position=i;
		else if(!strcmp(argv[i],"-N"))
            N_flag_position=i;        
        else if(!strcmp(argv[i],"-R"))
            R_flag_position=i;
		else if(!strcmp(argv[i],"-probes"))
            probes_flag_position=i;
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
	

	// An den exei do8ei shmaia -k ekxwreitai sto k h proepilegmenh timh sfalma
	if(k_flag_position==0)
		*k=default_k;	
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
	
	// An den exei do8ei shmaia -N ekxwreitai sto N h proepilegmenh timh sfalma
	if(N_flag_position==0)
		*N=default_N;	
	// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
	else
		*N=atoi(argv[N_flag_position+1]);
		
	// An to N einai mh 8etikos ari8mos epistrefetai sfalma
	if(*N<=0){
        
        printf("Invalid N value.\n");
        return -1;
        
    }
	
	// An den exei do8ei shmaia -R ekxwreitai sto R h proepilegmenh timh sfalma
	if(R_flag_position==0)
		*R=default_R;	
	// Diaforetika ekxwreitai h timh poy akoloy8ei thn antistoixh shmaia
	else
		*R=atof(argv[R_flag_position+1]);
		
	// An to R einai mh 8etikos ari8mos epistrefetai sfalma
	if(*R<=0){
        
        printf("Invalid R value.\n");
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

    return 0;
}

// Synarthsh emfanishs orismatwn (boh8htikh gia logoys elegxoy) 
void ARGS_print_cube_args(char* input_file,char* query_file,int k,int M,char* output_file,int N,double R,int probes){
	
	printf("input file:%s\n",input_file);	
	printf("query file:%s\n",query_file);	
	printf("output file:%s\n",output_file);	
	printf("k=%d\n",k);	
	printf("M=%d\n",M);	
	printf("probes=%d\n",probes);	
	printf("N=%d\n",N);	
	printf("R=%lf\n",R);	
	
}
