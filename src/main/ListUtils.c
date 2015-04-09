#include "ListUtils.h"
#include <stdlib.h>

struct List{
	void* data;
	struct List* next;
	int num_elements;
};

ListRef newList(void* headData){
	ListRef ref = malloc(sizeof(struct List));
	//malloc failed!
	if (ref == NULL) { 
		return NULL;
	}
	if (headData != NULL){
		ref->data = headData;
		ref->num_elements = 1;
	} else {
		ref->num_elements = 0;
	}
	ref->next = NULL;
	return ref;
}

void* headData(ListRef list){
	return list->data;
}

ListRef tail(ListRef list){
	//checks if the list is too short
	if (list->num_elements < 2) {
		return NULL;
	}
	return list->next;
}

ListRef append(ListRef list, void* data){
	//if the data is null
	if (data == NULL) {
		return NULL;
	}
	//if the list is empty, insert the element into the empty node
	if (list->num_elements == 0) {
		list->data = data;
		list->num_elements = 1;
		return list;
	}
	struct List* temp = list;
	while (temp->next != NULL) {
		temp->num_elements += 1;
		temp = temp->next;
	}
	ListRef ref = malloc(sizeof(struct List));
	//malloc failed!
	if (ref == NULL) { 
		return NULL;
	}
	ref->next = NULL;
	ref->data = data;
	ref->num_elements = 1;
	temp->num_elements += 1;
	temp->next = ref;
	return ref;
}

void destroyList(ListRef list, FreeFunc freeData){
	ListRef prev = list;
	ListRef nxt;
	if (list->num_elements == 0) {
		free(prev);
		return;
	}
	do {
		(*freeData)(prev->data);
		nxt = prev->next;
		free(prev);
		prev = nxt;
	} while (prev != NULL);
}

int isEmpty(ListRef list){
	return !(list->num_elements);
}
