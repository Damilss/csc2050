/***********************************************************
* Author: Emilio Scott
* Email: escott15@calpoly.edu
* Date Created: 2026-10-12
* Filename: linkedList.c
*
* Overview:
* This program is an implementation of the linked list and the deque ADTs.
*
************************************************************/

#include <malloc/_malloc_type.h>
#include <stdlib.h>
#include <stdio.h>


#include "linkedList.h"

/**
 * typedef struct linkedList creates the aliases Linkedlist and deque for
 * the struct LinkedList
 *
 * please note this linkedList is a doubly linkedList
 *
 * link *head: holds a pointer to the head of the doubly linked list
 * link *tail: hols a pointer to the tail of the doubly linked list
 * int size: is the size of the doubly linked list
 *
 * aliases: LinkedList, Deque
 */
typedef struct LinkedList{
	Link *head;
	Link *tail;
	int size;
} LinkedList, Deque;

/**
 * Typedef struct creates the alias Link and node for the struct
 * Link that goes into the doubly linked list, please see
 * `typedef struct LinkedList`.
 *
 * TYPE value, this is the value that the link holds
 * Link *previous: holds a pointer to the previous link
 * Link *next: holds a pointer to the next link
 *
 * aliases: Link, node
 */
typedef struct Link{
	TYPE value;
	Link *previous;
	Link *next;
}Link, node;


/* ************************************************************************
	Linked List Functions
************************************************************************ */

LinkedList *linkedListCreate(void){
	LinkedList *newlist = malloc(sizeof(LinkedList));

	if (newlist == NULL)
		fprintf(stderr, "linkedListCreate(): malloc returned NULL\n");
	else {
    newlist->size = 0;
	newlist->head = NULL;
	newlist->tail = NULL;
	}
	return newlist;
}

void deleteList(LinkedList *list){

	// handling if list is already NULL
	if (list == NULL)
		return;


	Link *currentlink = list->head;

	while (currentlink != NULL){
		Link *nextlink = currentlink->next;

		free(currentlink);

		currentlink = nextlink;
	}
	free(list);
}

int sizeList(LinkedList *list){

	// checks for null list
	if (list == NULL)
		return -1;


	return list->size;
}

int containsList( LinkedList *list, TYPE value  ){

	// if list is NULL
	if (list == NULL || list->head == NULL)
		return -1;


	Link *currentlink = list->head;
	int currentidx = 0;

	// check 0th index
	if (currentlink->value == value)
		return currentidx;

	// loop until tail is reached
	while (currentlink->next != NULL){
		currentidx++;
		currentlink = currentlink->next;

		if (currentlink->value == value)
			return currentidx;
	}
	return -1;
}

void addListFront(LinkedList *list, TYPE value ){
	if (list == NULL)
		return;

	Link *newlink = malloc(sizeof(Link));

	if (newlink == NULL){
		fprintf(stderr, "addListFront(): malloc returned NULL\n");
		return;
	}

	newlink->value = value;
	newlink->previous = NULL;
	list->size++;

	if (list->head == NULL && list->tail == NULL){
		newlink->next = NULL;
		list->head = newlink;
		list->tail = newlink;

		return;
	}

	newlink->next = list->head;
	newlink->next->previous = newlink;
	list->head = newlink;
}


void addListBack(LinkedList *list, TYPE value){
	if (list == NULL)
		return;

	Link *newlink = malloc(sizeof(Link));

	if (newlink == NULL){
		fprintf(stderr, "addListBack(): malloc returned NULL\n");
		return;
	}

	newlink->value = value;
	newlink->next = NULL;
	list->size++;

	if (list->head == NULL && list->tail == NULL){
		newlink->previous = NULL;
		list->tail = newlink;
		list->head = newlink;

		return;
	}

	newlink->previous = list->tail;
	newlink->previous->next = newlink;
	list->tail = newlink;
}

void addValueListFront(LinkedList *list, TYPE value, TYPE valueToAdd){

	// if list is a NULL pointer
	if (list == NULL)
		return;

	if (list->head == NULL)
		return;

	Link *currentlink = list->head;

	// if list head has value we're adding before
	if (currentlink->value == value){
		addListFront(list, valueToAdd);

		return;
	}

	while (currentlink->next != NULL){
		currentlink = currentlink->next;

		if (currentlink->value == value){
			Link *newlink = malloc(sizeof(Link));

			if (newlink == NULL){
				fprintf(stderr, "addValueListFront(): malloc returned NULL\n");
				return;
			}

			newlink->value = valueToAdd;

			newlink->previous = currentlink->previous;
			newlink->previous->next = newlink;

			newlink->next = currentlink;
			currentlink->previous = newlink;

			list->size++;

			return;
		}
	}
}

void addValueListBack(LinkedList *list, TYPE value, TYPE valueToAdd){

	// if list is a null pointer
	if (list == NULL)
		return;

	// if list is empty
	if (list->head == NULL)
		return;

	Link *currentlink = list->tail;

	// if list tail has value were adding after
	if(currentlink->value == value){
		addListBack(list, valueToAdd);
		return;
	}

	while(currentlink->previous != NULL){
		currentlink = currentlink->previous;

		if (currentlink->value == value){
			Link *newlink = malloc(sizeof(Link));

			if (newlink == NULL){
				fprintf(stderr, "addValueListBack(): malloc returned NULL\n");
				return;
			}

			newlink->value = valueToAdd;

			newlink->previous = currentlink;
			newlink->next = currentlink->next;

			currentlink->next = newlink;
			newlink->next->previous = newlink;

			list->size++;

			return;
		}
	}
}

void removeList(LinkedList *list, TYPE value){

	// if list is null
	if (list == NULL)
		return;

	// if there are no values in the list
	if (list->tail == NULL || list->head == NULL)
		return;

	// if only one value and that value matches, else if just return
	if (list->tail->previous == NULL && list->tail->value == value){
		free(list->tail);
		list->tail = NULL;
		list->head = NULL;

		// list size should be 0
		list->size--;

		return;

	} else if (list->tail->previous == NULL && list->tail->value != value)
		return;

	// if the value is at the tail
	if (list->tail->value == value){
		Link *tmp = list->tail;

		list->tail = list->tail->previous;
		list->tail->next = NULL;

		free(tmp);

		list->size--;

		return;
	}

	Link *currentlink = list->tail;

	// check for values in the middle
	while (currentlink->previous != NULL){
		currentlink = currentlink->previous;
		
		// if value is at head	
		if (currentlink == list->head && currentlink->value == value){
			list->head = list->head->next;
			list->head->previous = NULL;

			free(currentlink);
			list->size--;

			return;
		}

		if (currentlink->value == value){
			currentlink->next->previous = currentlink->previous;
			currentlink->previous->next = currentlink->next;

			free(currentlink);

			list->size--;

			return;
		}	
	}
}

void printList(LinkedList *list){
	if (list == NULL){
		printf("List is NULL\n");
		return;
	}

	if (list->head == list->tail && (list->head == NULL || list->tail == NULL)){
		printf("current list: \n[  ]\n");
		return;
	}
	
	if (list->head == list->tail){
		printf("current list: \n[ %d ]\n", list->head->value);
		return;
	}

   	Link *currentlink = list->head;
	int currentidx = 0;
	printf("current list: \n");

	while (currentlink != NULL){
		if (currentidx == 0)
			printf("[ %d", currentlink->value);
		else
			printf(", %d", currentlink->value);

		currentlink = currentlink->next;
		currentidx++;
	}
	printf(" ]\n");
}

void reverseList(LinkedList *list){

	// If list is NULL
	if (list == NULL)
		return;

	// list is empty
	if(list->head == NULL)
		return;

	// if there is only one value
	if (list->head == list->tail)
		return;

	Link *currentlink = list->head;

	// swap next and previous on every link, walking from the old head
	// to the old tail
	while (currentlink != NULL){
		Link *nextlink = currentlink->next;

		currentlink->next = currentlink->previous;
		currentlink->previous = nextlink;

		currentlink = nextlink;
	}

	// the old tail is now the front, and the old head is now the back
	Link *tmp = list->head;
	list->head = list->tail;
	list->tail = tmp;
}

/* ************************************************************************
	Deque Functions
************************************************************************ */
Deque *dequeCreate(void){
	return linkedListCreate();
}

void deleteDeque(Deque *deque){
	deleteList(deque);
}

int sizeDeque( Deque *deque ){
	return sizeList(deque);
}

void addFrontDeque(Deque *deque, TYPE value){
	addListFront(deque, value);
}

TYPE backDeque(Deque *deque){
	return deque->tail->value;
}

void removeBackDeque(Deque *deque){
	Link *back = deque->tail;
	back->previous = deque->tail;
	deque->tail->next = NULL;

	free(back);
}

void printDeque(Deque *deque){
	printList(deque);
}
