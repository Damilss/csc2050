#include <stdlib.h>
#include <stdio.h>
#include "dynamicArray.h"

# ifndef TYPE
# define TYPE     double 
# define TYPE_SIZE sizeof(double)
# endif


typedef struct
{
	TYPE* data;		/* pointer to the data array */
	int size;		/* Number of elements in the array */
	int capacity;	/* capacity ofthe array */
}DynamicArray;


/* ************************************************************************
	Dynamic Array Functions
************************************************************************ */

/* Allocate and initialize dynamic array.

	param:	cap 	desired capacity for the dyn array
	pre:	none
	post:	none
	ret:	a non-null pointer to a dynArr of cap capacity
			and 0 elements in it.		
*/
DynamicArray* createDynArr(int cap) // 5 Points
{

}

/* Deallocate data array and the dynamic array ure. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	the memory used by v->data is freed
	post:	the memory used by d is freed
*/
void deleteDynArr(DynamicArray* v) // 5 Points
{

}

/* Resizes the underlying array to be the size cap 

	param: 	v		pointer to the dynamic array
	param:	cap		the new desired capacity
	pre:	v is not null
	post:	v has capacity newCap
*/
void _dynArrSetCapacity(DynamicArray* v, int newCap) // 5 Points
{	

}

/* Get the size of the dynamic array

	param: 	v		pointer to the dynamic array
	pre:	v is not null
	post:	none
	ret:	the size of the dynamic array
*/
int sizeDynArr(DynamicArray* v) // 5 Points
{

}

/* 	Adds an element to the end of the dynamic array

	param: 	v		pointer to the dynamic array
	param:	val		the value to add to the end of the dynamic array
	pre:	the dynArry is not null
	post:	size increases by 1
	post:	if reached capacity, capacity is doubled
	post:	val is in the last utilized position in the array
*/
void addDynArr(DynamicArray* v, TYPE val) // 5 Points
{

}

/* 	Adds an element at a specific position of the dynamic array

	param: 	v		pointer to the dynamic array
	param:	val		the value to add to the end of the dynamic array
	param:	pos		the index to add to val in the dynamic array
	pre:	the dynArry is not null
	pre:	pos >= 0 and pos < size of the array
	post:	size increases by 1
	post:	if reached capacity, capacity is doubled
	post:	val is in the array
*/
void addAtDynArr(DynamicArray* v, TYPE val, int pos) // 5 Points
{

}

/*	Get an element from the dynamic array from a specified position
	
	param: 	v		pointer to the dynamic array
	param:	pos		integer index to get the element from
	pre:	v is not null
	pre:	v is not empty
	pre:	pos < size of the dyn array and >= 0
	post:	no changes to the dyn Array
	ret:	value stored at index pos
*/
TYPE getDynArr(DynamicArray* v, int pos) // 5 Points
{
 
}

/*	Swap two specified elements in the dynamic array

	param: 	v		pointer to the dynamic array
	param:	i,j		the elements to be swapped
	pre:	v is not null
	pre:	v is not empty
	pre:	i, j >= 0 and i,j < size of the dynamic array
	post:	index i now holds the value at j and index j now holds the value at i
*/
void swapDynArr(DynamicArray* v, int i, int  j) // 5 Points
{

}

/*	Checks if a value is in the array and returns the first index the value is found in

	param: 	v		pointer to the dynamic array
	param:	val		the value to look for
	pre:	v is not NULL or empty
	pre:	i, j >= 0 and i,j < size of the dynamic array
	post:	returned index i holds the value
*/
int containsDynArr(DynamicArray* v, TYPE val) // 5 Points
{

}

/*	Remove the element at the specified location from the array

	param: 	v		pointer to the dynamic array
	param:	idx		location of element to remove
	pre:	v is not null
	pre:	v is not empty
	pre:	idx < size and idx >= 0
	post:	the element at idx is removed
*/
void removeAtDynArr(DynamicArray* v, int idx) // 5 Points
{

}


int main(int argc, char* argv[]) // 30 Points for complete test cases
{
	return 0;
}
