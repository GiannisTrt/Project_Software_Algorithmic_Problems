#ifndef _LIST_
#define _LIST_

#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdlib>

#include "bucket.h"

	// Lista akeraiwn
	
	// Orismos komvou listas
	typedef struct list_node{
		
		int value;
		struct list_node* next;
		
	}list_node;
	
	// Orismos listas (prokeitai sthn oysia gia lista me kefalh)
	typedef struct list{
		
		// Deikths pros ton prwto komvo 
		list_node* first;
		
		// Plh8os komvwn listas 
		int num_of_nodes;
		
	}list;
		
	// Synarthsh dhmiourgias listas
	list* LIST_create();
	
	// Eisagwgh stoixeiou
	void LIST_insert(list* list_ptr, int value);
	
	// Emfanish listas
	void LIST_print(list* list_ptr);
	
	// Adeiasma listas
	void LIST_empty(list* list_ptr);
	
	// Metatroph lista se bucket
	bucket* LIST_convert_to_bucket(list* list_ptr);

#endif