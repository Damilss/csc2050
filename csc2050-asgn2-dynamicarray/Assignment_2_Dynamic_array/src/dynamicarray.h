#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

/*
 * Author: Emilio S
 * dynamic arraylist data structure practice in C
 */

/*
 * struct intArr is an array list data structure, starting off with 10 spaces
 * that dynamically adjust themselves.
 */
typedef struct {
	int *arr;
	int capacity;
	int size;
} intArr;

/*
 * initArray() initializes an array instance
 * Remember that the caller of this function has to check (*(intArr.arr) != NUL)
 * @return: intArr, the given array that was intialized
 */ 
intArr initArr(void);

/*
 * destroyArray() deletes the given array
 * safe to call with a NULL arr, or on an array that was already destroyed
 * @param: intArr *arr, the specified array to be deleted
 * @return: void
 */
void destroyArr(intArr *arr);

/*
 * appendItem() appends a number to the end of an intArr
 * if the array is at full capacity, recreate the array adn double the size of the
 * array to fit more items inside, moving all of the items from the origianl into
 * the new array
 * @param: intArr *arr, is the specified array
 * @param: int item, the item you want to append to the array
 * @return: int, exit status if return is non-zero, then something went wrong
 * 		1 = null input (arr or arr->arr), or realloc() returned NULL
 */
int appendItem(intArr *arr, int item);

/*
 * setItem(), recieves item and index and sets the item in specified array
 *
 * I chose to have the set index range be from 0 to arr->size - 1 than capacity.
 *
 * @param: intArr *arr, is the specified intArr
 * @param: int item, specified item to put in array
 * @param: int idx specified index for item to be set
 * @return: int exit status, if return is non-zero, then something went wrong
 * 		1 = null input (arr or arr->arr)
 * 		2 = bad input, idx < 0 or idx >= arr->size
 */
int setItem(intArr *arr, int item, int idx);

/*
 * removeItem() takes specified item from intArr, removes it, slides rest of
 * values from remaining index highest then specified idx, depending where
 * specified index of removed item was
 * @param: intArr *arr, specified arr
 * @param: int idx, specified idx for item remove
 * @return: int, exit status, if return is non-zero, then something went wrong
 * 		1 = null input (arr or arr->arr)
 * 		2 = bad input, idx < 0 or idx >= arr->size
 */
int removeItem(intArr *arr, int idx);

/*
 * length(), retreives arr->size
 * @param: intArr *arr, specified arr
 * @return: int returns the length of specified arr
 */
int length(const intArr *arr);


/**
 * contains(): returns 1 if a specified integer is in the specified intArr, and 0
 * if not found, -1 if there is an err
 * @param: const intArr *arr, the array to pull the specified value from
 * @param: int val, specified value to check for
 * @return: int, 1 if found, 0 if not found, -1 if arr or arr->arr is NULL
 */
int contains(const intArr *arr, int val);

/**
 * getItem(): retrieves the integer at the specified idx
 * @param: const intArr *arr, specified array to pull specified number from
 * @param: int idx, specified idx to pull from
 * @return: int, specified number in that index, -1 if something went wrong
 * 		(null input, or idx < 0 or idx >= arr->size)
 * 		careful, -1 can also be a real item stored in the array
 */
int getItem(const intArr *arr, int idx);

/**
 * swapItem(): swaps the iteams at two specified spots in any specified array
 * @param: intArr *arr, the specified to perform the swap on
 * @param: int idx1, the first index of the item to be swapped
 * @param: int idx2, the second index of the item to be swapped
 * @return: int, exit status, if return is -1 then something went wrong
 * 		(null input, or idx1/idx2 < 0 or >= arr->size), 0 if swapped
 */
int swapItem(intArr *arr, int idx1, int idx2);

#endif
