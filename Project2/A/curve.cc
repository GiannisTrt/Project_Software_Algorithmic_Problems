#include "curve.h"

// Diavasma kampylwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn kampylwn kai h diastash
int CURVE_read_discrete_frechet_curves_from_file(char* file,double*** curves,char*** curve_names,int* num_of_curves,int* d){
	
	// Pinakas gia diavasma onomatos arxeioy (1024 xarakthres einai yper-arketoi)
	char curve_name[1024];
	
	// Deikths pros to arxeio 
	FILE* fp;
			
	// Boh8htikes metablhtes gia thn eyresh ths diastashs twn kampylwn
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
	
	
	// Diavazoyme ka8e grammh kai ayksanoyme to plh8os twn kampylwn
	*num_of_curves=1;
	while(!feof(fp)){
		
		fscanf_result=fscanf(fp,"%s",curve_name);
		
		for(i=0;i<*d;i++)
			fscanf_result=fscanf(fp,"%lf",&coordinate);
		
		(*num_of_curves)++;
		
	}
	
	// Logw toy \n exei diabastei ena parapanw dianysma
	(*num_of_curves)--;
	
	// Pleon exoyme kai diastash kai plh8os kampylwn. Desmeyetai mnhmh gia ton pinaka twn kampylwn kai 
	// diavazontai ta dianysmata
	*curves=(double**)malloc(sizeof(double*)*(*num_of_curves));
	for(i=0;i<*num_of_curves;i++)
		(*curves)[i]=(double*)malloc(sizeof(double)*2*(*d));
	
	// Desmeyetai kai xwros gia ton pinaka twn onomatwn twn kampylwn 
	*curve_names=(char**)malloc(sizeof(char*)*(*num_of_curves));
	
	rewind(fp);

	for(i=0;i<*num_of_curves;i++){
		
		// Diavasma onomatos dianysmatos
		fscanf_result=fscanf(fp,"%s",curve_name);
		
		// Desmeysh katallhlhs mnhmhs
		(*curve_names)[i]=(char*)malloc(sizeof(char)*(strlen(curve_name)+1));
				
		// Apo8hkeysh
		strcpy((*curve_names)[i],curve_name);
		
		// An to diavasma toy prwtoy ari8moy einai epityxes (an kai agnoeitai), diavazetai kai h
		// ypoloiph grammh (oi syntetagmenes toy dianysmatos)
		// Sto diavasma symplyrwnontai oi orizonties syntetagmenes me toyhs diadokikoys 8etikoys akeraioys
		if(fscanf_result){
			
			for(j=0;j<*d;j++){
				
				(*curves)[i][2*j]=j+1;
				fscanf_result=fscanf(fp,"%lf",&((*curves)[i][2*j+1]));
				
			}
		}
	}

	fclose(fp);
	
	return 0;
	
}


// Diavasma kampylwn apo arxeio kai apo8hkeysh toys se sxetiko disdiastato pinaka
// O xwros toy pinaka desmeyetai entos ths synarthshs. Epistrefontai emmesa to plh8os twn kampylwn kai h diastash
int CURVE_read_continuous_frechet_curves_from_file(char* file,double*** curves,char*** curve_names,int* num_of_curves,int* d){
	
	// Pinakas gia diavasma onomatos arxeioy (1024 xarakthres einai yper-arketoi)
	char curve_name[1024];
	
	// Deikths pros to arxeio 
	FILE* fp;
			
	// Boh8htikes metablhtes gia thn eyresh ths diastashs twn kampylwn
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
	
	// 
	
	// Diavazoyme ka8e grammh kai ayksanoyme to plh8os twn kampylwn
	*num_of_curves=1;
	while(!feof(fp)){
		
		fscanf_result=fscanf(fp,"%s",curve_name);
		
		for(i=0;i<*d;i++)
			fscanf_result=fscanf(fp,"%lf",&coordinate);
		
		(*num_of_curves)++;
		
	}
	
	// Logw toy \n exei diabastei ena parapanw dianysma
	(*num_of_curves)--;
	
	// Pleon exoyme kai diastash kai plh8os kampylwn. Desmeyetai mnhmh gia ton pinaka twn kampylwn kai 
	// diavazontai ta dianysmata
	*curves=(double**)malloc(sizeof(double*)*(*num_of_curves));
	for(i=0;i<*num_of_curves;i++)
		(*curves)[i]=(double*)malloc(sizeof(double)*(*d));
	
	// Desmeyetai kai xwros gia ton pinaka twn onomatwn twn kampylwn 
	*curve_names=(char**)malloc(sizeof(char*)*(*num_of_curves));
	
	rewind(fp);
	
	for(i=0;i<*num_of_curves;i++){
		
		// Diavasma onomatos dianysmatos
		fscanf_result=fscanf(fp,"%s",curve_name);
		
		// Desmeysh katallhlhs mnhmhs
		(*curve_names)[i]=(char*)malloc(sizeof(char)*(strlen(curve_name)+1));
				
		// Apo8hkeysh
		strcpy((*curve_names)[i],curve_name);
		
		// An to diavasma toy prwtoy ari8moy einai epityxes (an kai agnoeitai), diavazetai kai h
		// ypoloiph grammh (oi syntetagmenes toy dianysmatos)
		if(fscanf_result)
			for(j=0;j<*d;j++)
				fscanf_result=fscanf(fp,"%lf",&(*curves)[i][j]);
	
	}

	fclose(fp);
	
	return 0;
	
}


// Synarthsh emfanishs kampylwn (boh8htikh gia logoys elegxoy)
void CURVE_print_discrete_frechet_curves(double** curves,int num_of_curves,int d){
	
	// Ari8modeiktes
	int i,j;
	
	for(i=0;i<num_of_curves;i++){
		
		printf("%d ",i+1);
		for(j=0;j<2*d;j++)
			printf("%lf ",curves[i][j]);
		printf("\n");
	
	}
	
}


// Synarthsh emfanishs kampylwn (boh8htikh gia logoys elegxoy)
void CURVE_print_curves_and_names(double** curves,char** curve_names,int num_of_curves,int d){
	
	// Ari8modeiktes
	int i,j;
	
	for(i=0;i<num_of_curves;i++){
		
		printf("%s ",curve_names[i]);
		for(j=0;j<d;j++)
			printf("%lf ",curves[i][j]);
		printf("\n");
	
	}
	
}

// Synarthsh emfanishs dianysmatos (boh8htikh gia logoys elegxoy)
void CURVE_print_vector(double* vector,int d){
	
	// Ari8modeikths
	int j;

	for(j=0;j<d;j++)
		printf("%lf ",vector[j]);
	printf("\n");
		
}


// Apeley8erwsh mnhmhs toy pinaka twn kampylwn
void CURVE_free_discrete_frechet_curves(double** curves,int num_of_curves,int d){
	
	// Ari8modeikths
	int i;
	
	for(i=0;i<num_of_curves;i++)
		free(curves[i]);
	
	free(curves);	
	
}

// Apeley8erwsh mnhmhs toy pinaka twn kampylwn
void CURVE_free_continuous_frechet_curves(double** curves,int num_of_curves,int d){
	
	// Ari8modeikths
	int i;
	
	for(i=0;i<num_of_curves;i++)
		free(curves[i]);
	
	free(curves);	
	
}
