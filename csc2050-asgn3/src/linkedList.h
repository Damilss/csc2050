/**
 * Author: Emilio Scott
 * Date Created: 2026-10-12
 * File: linkedList.h
 *
 * Overview:
 * Header file of linked list and deque ADT.
 */

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

/**
 * typedef struct linkedList creates the aliases LinkedList and deque for
 * the struct linkedList
 *
 * please note this linkedList is a doubly linkedList
 *
 * link *head: holds a pointer to the head of the doubly linked list
 * link *tail: holds a pointer to the tail of the doubly linked list
 * int size:  hold the value for the size of the doubly linked list
 *
 * aliases: LinkedList, Deque
 */
typedef struct LinkedList LinkedList, Deque;

/**
 * Typedef struct creates the alias Link and node for the struct Link that
 * goes into the doubly linked list, please see `typedef struct LinkedList`
 *
 * TYPE value, this is the value that the link holds
 * Link *previous: holds a pointer to the previous link
 * Link *next: holds a pointer to the next link
 *
 * aliases: Link, node
 */
typedef struct Link Link, node;


/* ************************************************************************
	Linked List Interface
************************************************************************ */

/**
 * linkedListCreate() initializes a doubly linked list with head
 * and tail pointers
 * @param: void
 * @return: (LinkedList *) returns the doubly linked list
 */
LinkedList* linkedListCreate(void);

/**
 * deleteList() Deallocates every link in the list including sentinels and deletes the list
 * itself
 * @param: LinkedList *myList, the list to be deleted
 * @return: void
 */
void deleteList( LinkedList *list );

/**
 * sizeList returns the size of the list
 * @param: LinkedList *myList, the list to return the size pof
 * @return: int, the size of the list
 */
int sizeList( LinkedList *list );

/**
 * containsList() returns the index of the link if value in the list,
 * otherwise returns the false value listed as -1
 * @param: linkedList *list, the inputted list to run the function on
 * @param: TYPE value, this is the value to be check for
 * @return: int, the index of where the value is in the list
 */
int containsList( LinkedList *list, TYPE value );

/**
 * addListFront() adds a new link to the front of the list
 * @param: LinkedList *list, the list to add the new link to
 * @param: TYPE value, the value to add to the end of the list
 * @return: void
 */
void addListFront( LinkedList *list, TYPE value );

/**
 * addListBack() adds a new link at the back of the doubly linked list
 * @param: LinkedList *list, the doubly linked list to add to the back to
 * @param: TYPE value, value to add
 * @return: void
 */
void addListBack( LinkedList *list, TYPE value );


/**
 * addValueListFront() adds a new link at the before an existing value in the list
 * @param: LinkedList *list, list to mutate
 * @param: TYPE value, the existing value in the list to add the item before to
 * @param: TYPE valueToAdd, this is the value that will be added.
 * @return: void
 */
void addValueListFront( LinkedList *list, TYPE value, TYPE valueToAdd );

// Adds a new link at the after an existing value in the list
/**
 * addValueListBack() adds a new link at the after an existing value in the list
 * @param: LinkedList *list, the LinkedList to mutate
 * @param: TYPE value the value that will be used as reference to add to back off
 * @param: TYPE valueToAdd, the value to add to the list
 * @return: void
 */
void addValueListBack( LinkedList *list, TYPE value, TYPE valueToAdd );

/**
 * removeList() removes the given value from the list, if no occurance
 * of that value in list nothing happens
 * @param: LinkedList *list, the specified list to mutate
 * @param: TYPE value, the value to remove from the list
 */
void removeList( LinkedList *list, TYPE value );

// Prints the values of the links in the list from front to back
/**
 * printList() prints the values of the links in the list from front to back
 * @param: LinkedList *list, the linkedList to print
 * @return: void
 */
void printList( LinkedList *list );

// Reverse the list
/**
 * reverseList() reverses the values in the list
 * @param: LinkedList *list, the linkedList to reverse the values of
 * @return: void
 */
void reverseList( LinkedList *list );

/* ************************************************************************
	Deque Interface
************************************************************************ */

/**
 * dequeCreate() allocatess and initalizes the deque
 * @param: void
 * @return: Deque, Initialized deque
 */
Deque *dequeCreate(void);

/**
 * deleteDeque deallocates the data within and deletes the deque
 * @param: Deque, the specfied deque to be deleted
 * @return: void
 */
void deleteDeque(Deque *deque);

/**
 * sizeDeque(), returns the size of the Deque
 * @param: Deque *myDeque, the Deque to count the size of
 * @return: int, the size of the Deque specified
 */
int sizeDeque(Deque *deque);

/**
 * addFrontDeque() adds a new link with the given value to the front of deque
 * @param: Deque *myDeque, the Deque to be mutated
 * @param: TYPE value, the value to add to the front of the deque
 */
void addFrontDeque(Deque *deque, TYPE value);

/**
 * backDeque() returns the value of the link at the back of the deque
 * @param: Deque *myDeque, the deque to pull the value from
 * @return: TYPE, the value to return from the deque
 */
TYPE backDeque(Deque *deque);

/**
 * removeBackDeque() removes the link at the back of the deque.
 * @param: Deque *myDeque, the deque to mutate
 * @return: void
 */
void removeBackDeque(Deque *deque);

/** printDeque prints the values of the links in the deque from front to back
 * @param: Deque *myDeque, the deque to print values from
 * @return: void
 */
void printDeque(Deque *deque);

#endif
