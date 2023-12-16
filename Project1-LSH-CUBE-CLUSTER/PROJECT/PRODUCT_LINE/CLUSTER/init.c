#include "init.h"

// Arxikopoihsh ths systadopoihshs me kmeans++
void INIT_k_means(int** vectors, int num_of_vectors, double (*metric)(int*,int*,int), int* cluster_centers, int K, int d){
	
	// To ekastote kentro pou epilegetai. 
	int current_center_num;
	
	//  O ari8mos twn epilegmnenwn kentrwn
	int num_of_chosen_centers;
	
	// Dynamikos pinakas pi8anothtwn me bash tis opoies 8a ylopoih8ei o algori8mos k-means++
	double* propability_array; //= new double[num_of_vectors];	
	
	// A8roisma sto opoio basizetai h ylopoihsh toy k-means++
	double S;
	
	// Metablhth epiloghs toy epomenoy kentroy me bash to krithrio toy k-means++
	double chooser;
	
	// Gia thn tyxaia eklogh poy apaitei o k-means++
	//default_random_engine generator;
	//uniform_real_distribution<double> distribution(0.0,1.0);
	
	// Boh8htikh metablhth shmaia
	int flag;
	
	// Boh8htikh metablhth
	int i,j;
	
	// H apostash ekastoy dianysmatos/kampylhs apo to prohgoymeno kentro
	double distance;
	
	// H trexousa timh toy xronoy dinetai ws sporos sthn rand()
	srand(time(NULL));
		
	// Epilegetai tyxaia to prwto kentro
	current_center_num = rand()%num_of_vectors+1;
	
	// Eisagetai sthn prwth 8esh toy sxetikoy pinaka
	cluster_centers[0]=current_center_num;
	
	// O ari8mos twn epilegmnenwn kentrwn ti8etai isos me 1
	num_of_chosen_centers = 1;
	
	// Desmeysh mnhmhs gia ton pinaka pi8anothtwn ekloghs kentroy
	propability_array=malloc(num_of_vectors*sizeof(double));
	
	// Epanalhptikh epilogh twn ypoloipwn kentrwn 
	for(num_of_chosen_centers=1;num_of_chosen_centers<K;num_of_chosen_centers++){		
		
		// printf("num_of_chosen_centers %d\n",num_of_chosen_centers);
		// Athroisma twn apostasewn olwn twn dianysmatwn apo to teleytaio epilegmeno kentro
		S=0;
		
		// Sarwnoume ola ta dianysmata
		for(i=0;i<num_of_vectors;i++){
			
			// Ean to trexon dianysma/kampylh exei epilegei hdh gia kentro, shkwnetai h shmaia flag
			flag=0;
			for(j=0;j<num_of_chosen_centers;j++){
				if(cluster_centers[j]==i){
					
					flag=1;
					break;
					
				}
			}
			
			// An exei epilegei gia kentro h sxetikh pithanothta na epilegei ek neou ws kentro einai mhden
			if(flag){
				
				flag=1;
				propability_array[i]=0;
								
			}
			else{
			// Diaforetika h sxetikh pithanothta dinetai apo thn apostash ths apo to prohgoymeno kentro
				
				
				distance = metric(vectors[current_center_num-1],vectors[i],d);
				
				// H apostash ayth prostithetai sto athroisma twn apostasewn poy antistoixei sthn oysi sto 1=100% 
				
				S+=distance;
				
				propability_array[i] = distance;
				
				
			}
			
		}
		
		// Edw epilegetai to neo kentro me vash ton k-means++
		chooser = ((double)rand() / (double)((unsigned)RAND_MAX + 1))*S;
				
		// To chooser einai enas arithmos apo 0 mexri S (afoy to distribution(generator) einai enas
		// tyxaios arithmos apo 0 mexri 1)
		// Sarwnoyme tpn pinaka propability_array kai to dianysma/kampylh panv sto opoio "peftei"
		// to chooser tha einai to neo kentro.
		// Profanws h pithnothta einai analogh ths apostashs apo to prohgoymeno kentro pragma poy 
		// einai kai h apaithsh toy k-means++
		S=0;
		for(i=0;i<num_of_vectors;i++){
			
			S+=propability_array[i];
			
			if(S>=chooser)
				break;
		}
		
		// To i+1 dinei th thesh toy neoy kentroy
		current_center_num = i+1;
		
		// An exei epilegei hdh den epilegetai deyterh fora
		flag=0;
		for(i=0;i<num_of_chosen_centers;i++){
			
			if(cluster_centers[i]==current_center_num){
				
				flag=1;
				break;
				
			}
			
		}
		
		if(flag)
			num_of_chosen_centers--;
		else
			cluster_centers[num_of_chosen_centers]=current_center_num;
			
	}
	
	free(propability_array);
		
}
