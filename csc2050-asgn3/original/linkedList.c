/***********************************************************
* Author: 
* Email: 
* Date Created: 
* Filename: linkedList.c
*
* Overview:
*	This program is an implementation of the linked list and the deque ADTs. 
*
************************************************************/

#include "linkedList.h"

typedef struct LinkedList 		// Creates the alliases LinkedList and Deque for the struct LinkedList
{
	Link* head;
	Link* tail;
	int size;
}LinkedList, Deque;

typedef struct Link				// Creates the allias Link for the struct Link
{
	TYPE value;
	Link* previous;
	Link* next;
}Link;


/* ************************************************************************
	Linked List Functions
************************************************************************ */


/* ************************************************************************
	Deque Functions
************************************************************************ */
