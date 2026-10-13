#ifndef LINKED_LIST_H
#define LINKED_LIST_H 1

#ifndef TYPE
#define TYPE int
#endif

#ifndef TYPE_SIZE
#define TYPE_SIZE sizeof(int)
#endif

#ifndef FORMAT_SPECIFIER
#define FORMAT_SPECIFIER "%d"
#endif

#ifndef LT
#define LT(A, B) ((A) < (B))
#endif

#ifndef GT
#define GT(A, B) ((A) > (B))
#endif

#ifndef EQ
#define EQ(A, B) ((A) == (B))
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct LinkedList LinkedList, Deque;

typedef struct Link Link;


/* ************************************************************************
	Linked List Interface
************************************************************************ */

// Allocates and initializes a doubly linked list with head and tail pointers
LinkedList* linkedListCreate();												

// Deallocates every link in the list including the sentinels and deletes the list itself
void deleteList( LinkedList* myList );							

// Returns the size of the list
int sizeList( LinkedList* myList );											

// Returns the index of the link if value is in the list otherwise -1
int containsList( LinkedList* list, TYPE value );							

// Adds a new link at the front of the list
void addListFront( LinkedList* list, TYPE value );							

// Adds a new link at the back of the list
void addListBack( LinkedList* list, TYPE value );							

// Adds a new link at the before an existing value in the list
void addValueListFront( LinkedList* list, TYPE value, TYPE valueToAdd );	

// Adds a new link at the after an existing value in the list
void addValueListBack( LinkedList* list, TYPE value, TYPE valueToAdd );		

// Removes the given value from the list
void removeList( LinkedList* list, TYPE value );

// Prints the values of the links in the list from front to back
void printList( LinkedList* list );											

// Reverse the list
void reverseList( LinkedList* list );										

/* ************************************************************************
	Deque Interface
************************************************************************ */

// Allocates and initializes the deque
Deque* dequeCreate();							

// Deallocates the data and deletes the deque
void deleteDeque(Deque* myDeque);				

// Returns the size of the deque
int sizeDeque(Deque* myDeque);					

// Adds a new link with the given value to the front of the deque
void addFrontDeque(Deque* myDeque, TYPE value);	

// Returns the value of the link at the back of the deque
TYPE backDeque(Deque* myDeque);					

// Removes the link at the back of the deque
void removeBackDeque(Deque* myDeque);			

// Prints the values of the links in the deque from front to back
void printDeque(Deque* myDeque);				

#endif
