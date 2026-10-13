/***********************************************************
* Author: Emilio Scott
* Email: escott15@calpoly.edu
* Date Created: 2026-10-12
* Filename: linkedList.c
*
* Overview:
*	This program is an implementation of the linked list and the deque ADTs. 
*
************************************************************/

#include "linkedList.h"

// Creates the alliases LinkedList and Deque for the struct LinkedList
typedef struct LinkedList 		{
	Link *head;
	Link *tail;
	int size;
}LinkedList, Deque;

// Creates the allias Link for the struct Link
typedef struct Link				{
	TYPE value;
	Link *previous;
	Link *next;
}Link;


/* ************************************************************************
	Linked List Functions
************************************************************************ */


/* ************************************************************************
	Deque Functions
************************************************************************ */
