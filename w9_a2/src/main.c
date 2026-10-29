/**
 * File: w9_a2/main.c
 * Author: Emilio L <escott15@calpoly.edu>
 * date: 2026-10-22
 *
 * Write function to compare and print the struct. Then write the prototypes
 * that would accept the structs and functions as parameters.
 *
 * Getting into generic programming a little bit, practicing using generic
 * functions in C
 *
 */

#include <stdio.h>
#include <stddef.h>
#include <assert.h>

/**
 * struct boat is a ADT used to define a boat
 *
 * @attribute: int length, defines the length of the boat this unit is in
 *			   feet
 * @attribute: int engineSize, defines the the size of the engine this
 *			   number is in displacement. (CCs)
 */
typedef struct {
	char *name;
	int length;
	int engineSize;
} Boat;

/**
 * containsArr() is a generic function used to receive any type of funtion and arr,
 * and check if that array contains the specified value
 *
 * @param: const void *arr, this is the specified array to loop through
 * @param: size_t size, returns the number of elements in the array
 * @param: size_t elementSize, returns the bytes taken up by each element in array
 * @param: const void *value, the specified value that we'll be checking for in the list
 * @param: int (*equals)(const void*, const void*), this is the function pointer that
 *		   that points to the function that will handle comparing the values.
 * @return: 1 = true, 0 = false. if the array contains that specific value, this function
 *			will return true, otherwise return false
 */
int containsArr(
		const void *arr,
		size_t size,
		size_t elementSize,
		const void *value,
		const int (*equals)(const void*, const void*)
		);

/**
 * printArr() is an generic function that is used to take and type of
 * function and arr, and prints each value of the array from top to bottom
 *
 * @param: void *arr, takes in the array
 * @param: void (*operation)(void *array), this is the operation that will
 *		   that will print the array, it should compliment the other
 *		   parameter's data type
 */
void printArr(const void *arr, const void (*operation)(const void* arr));

/**
 * boat_eq() is a program-specific function used to compare to boat structs
 *
 * @param: const void *val1, boat 1
 * @param: cosnt void *val2, boat 2
 *
 * @return:short int  1 = true, 0 = false
 */
int boat_eq(const void *val1, const void *val2);

/**
 * boat_repr() is the function that prints a specified boat
 * @param: const void *boat, the specified boat to be printed to stdout
 *
 * @return: void
 */
void boat_repr(const void *boat);


/**
 * main(), entry point to the program, used to test the implementations of
 * specific functions and generic function
 *
 * @param: int argc, argument count
 * @param: char *argv, arguments in an array of string literals
 * @return: int, is the exit status of the main program
 */
int main(int argc, char *argv[]){



	return 0;
}

int containsArr(
		const void *arr,
		size_t size,
		size_t elementSize,
		const void *value,
		const int (*equals)(const void*, const void*)
        ){

	for (size_t i = 0; i < size; i++){

		// pointer arithmetic + size of value, const char * to not mess with offset
		const void *element =
			(const char *)arr + i * elementSize;


		if ((*equals)(value, element)){
			return 1;
		}
	}

	return 0;
}

int boat_eq(const void *val1, const void *val2){
	const Boat *boat1 = val1;
	const Boat *boat2 = val2;
	if (
			boat1->length == boat2->length &&
			boat1->engineSize == boat2->engineSize
	   ){
		return 1;
	}
	return 0;
}

void boat_repr(const void *value){
	const Boat *boat = value;
	printf(
		"boat: %s, length: %d, Engine size: %d\n",
		boat->name, boat->length, boat->length
		);

}

void printArr(const void *arr, const void (*operation)(const void *arr)){
	operation(arr);
}

