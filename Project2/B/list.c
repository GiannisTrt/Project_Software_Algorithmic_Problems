#include "list.h"

// Synarthsh dhmiourgias listas
list* LIST_create(){

	// Desmeysh xwroy mnhmhs gia thn kefalh ths listas
    list* list_ptr = malloc(sizeof(list));

	// Arxikopoihsh pediwn
	list_ptr->first=NULL;
	list_ptr->num_of_nodes=0;
	
	return list_ptr;
	
}

// Eisagwgh stoixeiou
void LIST_insert(list* list_ptr, int value){
    
	// Desmeysh xwroy mnhmhs gia neo komvo
    list_node* node = malloc(sizeof(list_node));
	
	// Eisagvgh timhs ston komvo
	node->value=value;
	
	// Eisagwgh toy komvoy sth lista
	node->next=list_ptr->first;
	list_ptr->first=node;
	
	// Aykshsh toy plh8ous twn komvwn 	
	list_ptr->num_of_nodes++;
	
}

// Emfanish listas
void LIST_print(list* list_ptr){

    list_node* node = list_ptr->first;
    
    printf("<START OF LIST> ");
    while (node!= NULL){
    
        printf("%d ",node->value);		
        node = node->next;
    }
    printf("<END OF LIST>\n");
	
}

// Adeiasma listas
void LIST_empty(list* list_ptr){
    
	// Deikths ston ekastote trexonta komvo
	list_node *current_node = NULL;	
	
	// Deikths ston ekastote epomeno tou trexonta komvou
	list_node *next_node = NULL;
    
	// An h lista einai kenh den yparxei kati pou prepei na kanoume 
	if(list_ptr->num_of_nodes==0)
		return;
	
	// 8etoume ton current_node na deixnei ston prwto komvo
	current_node = list_ptr->first;
				
	// An o komvos einai monadikos, ton diagrafoume kai teleiwsame 
	if(list_ptr->num_of_nodes==1){
		
		free(current_node);		
		list_ptr->num_of_nodes=0;
		return;	
	}
	
	//  Diaforetika, proxwrane oso yparxei epomenos, diagrafontas panta ton trexonta 
	// (kai meta o next ginetai ka8e fora current)	
	next_node = current_node->next;
	while(current_node->next != NULL){
		
		// Free o trexwn
		free(current_node);
		
		// O epomenos ginetai trexwn
        current_node = next_node;
		
		// Enhmerwnetai o epomenos
        next_node = next_node->next; 
		
    }
	// Menei na diagrafei kai o trexwn (dioti apo thn anw diadikasia diagrafontai mono oi epomenoi)
	free(current_node);

	list_ptr->first=NULL;
	list_ptr->num_of_nodes=0;
    
}

// Metatroph lista se bucket
bucket* LIST_convert_to_bucket(list* list_ptr){
    
	// Deikths bucket sto bucket pou 8a epistrafei
    bucket* return_bucket;
	
	// Deikths sarwshs listas
	list_node* node;

	// Ari8modeikths
	register int i;
	
	// Deikths ston pinaka pou kataskeyazetai
	int* keys;
	
	// Mhkos pinaka pou kataskeyazetai
	int length;
	
	// Desmeysh mnhmhs gia to bucket pou 8a epistrafei
    return_bucket = malloc(sizeof(bucket));
	
	// To mhkos toy bucket einai iso me to plhos twn periexomenwn kombwn ths listas 
	return_bucket->length=list_ptr->num_of_nodes;
	
	// Desmeyetai xwros mnhmhs gia ton dynamiko pinaka tou bucket 
	return_bucket->keys = malloc(return_bucket->length*sizeof(int));
	
	// Arxikopoihsh deikth sarwshs listas
	node = list_ptr->first;
	
	// Xrhsh boh8htikoy deikth gia pio grhgorh prospelash
	keys=return_bucket->keys;
	
	// Xrhsh boh8htikhs metablhths gia pio grhgorh prospelash
	length=return_bucket->length;
	
	// Kataskeyh pinaka apo th lista
	for(i=0;i<length;i++){
		
		*keys=node->value;
		
		keys++;
		node=node->next;
		
	}
			
	return return_bucket;
	
}

