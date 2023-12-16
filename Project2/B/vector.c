#include "vector.h"

// Diavasma dianysmatwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn dianysmatwn kai h diastash
int VECTOR_read_vectors_from_file(char* file,double*** vectors,char*** vector_names,int* num_of_vectors,int* d){
	
	// Pinakas gia diavasma onomatos arxeioy (1024 xarakthres einai yper-arketoi)
	char vector_name[1024];
	
	// Deikths pros to arxeio 
	FILE* fp;
			
	// Boh8htikes metablhtes gia thn eyresh ths diastashs twn dianysmatwn
	char current_char;
	char previous_char;
	
	// Syntetagmenh dianysmatos
	double coordinate;
	
	// Ari8mmodeiktes
	int i,j;
	
	// Epistrefomeno ths fscanf (kyriws gia na mh bgazei warnings o metaglwttisths)
	int fscanf_result;
	
	// Anoigma toy arxeioy
	fp=fopen(file,"r");
	
	// Se periptwsh poy apotyxei to anoigma toy arxeioy epistrefetai apotyxia
	if(fp==NULL)
		return -1;
	
	// Ypologismos plh8ous syntetagmenwn dianysmatos
	// (O vroxos agnoei ta diadoxika kena)
	*d=0;
	previous_char=' ';
	while(!feof(fp)){
		
		fscanf_result=fscanf(fp,"%c",&current_char);

		if(current_char==' ' || current_char=='\t' || current_char=='\n'){
			if(previous_char!=' '&& previous_char!='\t'){			
				(*d)++;				
			}
		}
			
		if(current_char=='\n'){
			break;
		}
		previous_char=current_char;
			
	}
	
	// Apokatastash diastashs afoy diavasthke kai metrh8hke kai to onoma toy dianysmatos
	(*d)--;
	
	// Diavazoyme ka8e grammh kai ayksanoyme to plh8os twn dianysmatwn
	*num_of_vectors=1;
	while(!feof(fp)){
		
		fscanf_result=fscanf(fp,"%s",vector_name);
		
		for(i=0;i<*d;i++)
			fscanf_result=fscanf(fp,"%lf",&coordinate);
		
		(*num_of_vectors)++;
		
	}
	
	// Logw toy \n exei diabastei ena parapanw dianysma
	(*num_of_vectors)--;
	
	// Pleon exoyme kai diastash kai plh8os dianysmatwn. Desmeyetai mnhmh gia ton pinaka twn dianysmatwn kai 
	// diavazontai ta dianysmata
	*vectors=(double**)malloc(sizeof(double*)*(*num_of_vectors));
	for(i=0;i<*num_of_vectors;i++)
		(*vectors)[i]=(double*)malloc(sizeof(double)*(*d));
	
	// Desmeyetai kai xwros gia ton pinaka twn onomatwn twn dianysmatwn 
	*vector_names=(char**)malloc(sizeof(char*)*(*num_of_vectors));
	
	rewind(fp);
	
	for(i=0;i<*num_of_vectors;i++){
		
		// Diavasma onomatos dianysmatos
		fscanf_result=fscanf(fp,"%s",vector_name);
		
		// Desmeysh katallhlhs mnhmhs
		(*vector_names)[i]=(char*)malloc(sizeof(char)*(strlen(vector_name)+1));
				
		// Apo8hkeysh
		strcpy((*vector_names)[i],vector_name);
		
		// An to diavasma toy prwtoy ari8moy einai epityxes (an kai agnoeitai), diavazetai kai h
		// ypoloiph grammh (oi syntetagmenes toy dianysmatos)
		if(fscanf_result)
			for(j=0;j<*d;j++)
				fscanf_result=fscanf(fp,"%lf",&(*vectors)[i][j]);
	
	}

	fclose(fp);
	
	return 0;
	
}

// Synarthsh emfanishs dianysmatwn (boh8htikh gia logoys elegxoy)
void VECTOR_print_vectors(double** vectors,int num_of_vectors,int d){
	
	// Ari8modeiktes
	int i,j;
	
	for(i=0;i<num_of_vectors;i++){
		
		printf("%d ",i+1);
		for(j=0;j<d;j++)
			printf("%lf ",vectors[i][j]);
		printf("\n");
	
	}
	
}


// Synarthsh emfanishs dianysmatwn (boh8htikh gia logoys elegxoy)
void VECTOR_print_vectors_and_names(double** vectors,char** vector_names,int num_of_vectors,int d){
	
	// Ari8modeiktes
	int i,j;
	
	for(i=0;i<num_of_vectors;i++){
		
		printf("%s ",vector_names[i]);
		for(j=0;j<d;j++)
			printf("%lf ",vectors[i][j]);
		printf("\n");
	
	}
	
}

// Synarthsh emfanishs dianysmatos (boh8htikh gia logoys elegxoy)
void VECTOR_print_vector(int* vector,int d){
	
	// Ari8modeikths
	int j;

	for(j=0;j<d;j++)
		printf("%d ",vector[j]);
	printf("\n");
		
}


// Apeley8erwsh mnhmhs toy pinaka twn dianysmatwn
void VECTOR_free_vectors(int** vectors,int num_of_vectors,int d){
	
	// Ari8modeikths
	int i;
	
	for(i=0;i<num_of_vectors;i++)
		free(vectors[i]);
	
	free(vectors);	
	
}
