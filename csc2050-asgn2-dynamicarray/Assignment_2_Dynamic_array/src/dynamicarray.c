/**
 * file: dynamicarray.c
 * Author: Emilio S
 * date: 2026-10-05
 *
 * dynamic arraylist data structure practice in C
 */

#include "dynamicarray.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


// Remember that caller has to make sure (*(intArr.arr) != NULL)
intArr initArr(void){
	int *new_arr = calloc(10, sizeof(*new_arr));	

	return (intArr){
		.arr = new_arr,
		.capacity = 10,
		.size = 0,
	};
}

void destroyArr(intArr *arr){
	if (arr == NULL){
		fprintf(stderr, "destroyArr(): arr == NULL\n");
		return;
	}

	free (arr->arr);

	arr->arr= NULL;
	arr->capacity = -1;
	arr->size = -1;
}


int appendItem(intArr *arr, int item){

	if (arr == NULL){
		fprintf(stderr, "appendItem(): arr == NULL\n");
		return 1;
	}

	if (arr->arr == NULL){
		fprintf(stderr, "appendItem(): arr->arr == NULL\n");
		return 1;
	}

	// checks if arr is overloaded
	if ( arr->capacity <= arr->size ){

		//resizes and moves items
		int new_capacity = arr->capacity * 2;
		int *new_arr = realloc(arr->arr,  new_capacity * sizeof (*arr->arr));
	
		if (new_arr == NULL)
		{
			fprintf(stderr, 
				"appendItem(): new_arr: when setting reallocating new arr, "
			       "new_arr pointer = NULL\n"
				);
			return 1;
		}

		arr->arr = new_arr;
		arr->capacity = new_capacity;
	}

	// increment and appendItem at (arr->size)-1, after incrementing, size
	// is one past the new last item because of the 0th indexing in the array.
	arr->size++;
	(arr->arr)[(arr->size) - 1] = item;

	return 0;
}

int setItem(intArr *arr, int item, int idx){
	
	if (arr == NULL){
		fprintf(stderr, "setItem(): arr == NULL\n");
		return 1;
	} 

	if (arr->arr == NULL){
		fprintf(stderr, "setItem(): arr->arr == NULL\n");
		return 1;
	}

	// valid idx is 0 to arr->size - 1, idx == arr->size is already one past
	// the last item
	if (idx < 0 || idx >= arr->size){
		fprintf(stderr, "setItem(): index is out of range of intArr\n");
		return 2;
	}
	
	arr->arr[idx] = item;	

	return 0; 
}

int removeItem(intArr *arr, int idx){
	if(arr == NULL){
		fprintf(stderr, "removeItem(): arr == NULL\n");
		return 1;
	}

	if(arr->arr == NULL){
		fprintf(stderr, "removeItem(): arr->arr == NULL\n");
		return 1;
	}

	// valid idx is 0 to arr->size - 1, idx == arr->size is already one past
	// the last item
	if (idx < 0 || idx >= arr->size){
		fprintf(stderr, "removeItem(): index is out of range of intArr\n");
		return 2;
	} 
	
	//if specified index isn't the last item in arraylist
	if ( !(idx == arr->size - 1) ){

		// slide every item in front of removed item over	
		for (int i = idx; i < arr->size - 1;i++){
			arr->arr[i] = arr->arr[i+1];
			}
	}

	arr->size--;

	return 0;
}

int length(const intArr *arr){
	assert(arr != NULL && arr->arr !=NULL);
	return arr->size;
}

int contains(const intArr *arr, int val){
	if( arr == NULL ){
		fprintf(stderr, "contains(): arr == NULL\n");
		return -1;
	}

	if(arr->arr == NULL){
		fprintf(stderr, "contains(): arr->arr == NULL\n");
		return -1;
	}

	for(int i = 0; i < arr->size; i++){
		if( val == arr->arr[i] ){
			return 1;
		}
	}

	return 0; 
}

int getItem(const intArr *arr, int idx){
	if(arr == NULL){
		fprintf(stderr, "getItem(): arr == NULL\n");
		return -1;
	}

	if (arr->arr == NULL){
		fprintf(stderr, "getItem(): arr->arr == NULL\n");
		return -1;
	}

	if(idx < 0 || idx >= arr->size){
		fprintf(stderr, "getItem(): idx < 0 or idx >= arr->size\n");
		return -1;
	}

	return (arr->arr[idx]);
}

int swapItem(intArr *arr, int idx1, int idx2){
	if (arr == NULL){
		fprintf(stderr, "swapItem(): arr == NULL\n");
		return -1;
	}

	if (arr->arr == NULL){
		fprintf(stderr, "swapItem(): arr->arr == NULL\n");
		return -1;
	}

	if(idx1 < 0 || idx2 < 0 || idx1 >=arr->size || idx2 >=arr->size){
		fprintf(stderr, "swapItem(): idx1 or idx2 is < 0 or >= arr->size\n");
		return -1;
	}

	// hold idx1's item so it doesn't get overwritten
	int temp = arr->arr[idx1];
	arr->arr[idx1] = arr->arr[idx2];
	arr->arr[idx2] = temp;

	return 0;
}
