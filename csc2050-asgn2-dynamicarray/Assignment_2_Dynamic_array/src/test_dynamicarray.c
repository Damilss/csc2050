/*
 * Author: Emilio Scott
 * test_dynamicarray.c has unit test functions for the file dynamicarray.c
 * This file should have 100% coverage for dynamicarray.c
 * This file also includes it's own main
 *
 * Most of these functions don't need to be pointers since the the intArrs 
 * don't live beyond the functions that initialize them
 */

#include "dynamicarray.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void test_initArr_1(int *assertion_count);
void test_initArr_2(int *assertion_count); 
void test_destroyArr_1(int *assertion_count); 
void test_destroyArr_2(int *assertion_count);
void test_appendItem_normal(int *assertion_count);
void test_appendItem_size_eq_capacity(int *assertion_count);
void test_appendItem_nullptr(int *assertion_count);
void test_destroyArr_nullptr(int *assertion_count);
void test_setItem_normal(int *assertion_count);
void test_setItem_set_idx_gr(int *assertion_count);
void test_setItem_idx_neg(int *assertion_count);
void test_setItem_nullptr(int *assertion_count);
void test_removeItem_normal(int *assertion_count);
void test_removeItem_last_idx(int *assertion_count);
void test_removeItem_idx_gr(int *assertion_count);
void test_removeItem_idx_neg(int *assertion_count);
void test_removeItem_nullptr(int *assertion_count);
void test_length_1(int *assertion_count);
void test_length_2(int *assertion_count);
void test_contains_normal(int *assertion_count);
void test_contains_not_found(int *assertion_count);
void test_contains_nullptr(int *assertion_count);
void test_getItem_normal(int *assertion_count);
void test_getItem_idx_gr(int *assertion_count);
void test_getItem_idx_neg(int *assertion_count);
void test_getItem_nullptr(int *assertion_count);
void test_swapItem_normal(int *assertion_count);
void test_swapItem_idx_gr(int *assertion_count);
void test_swapItem_idx_neg(int *assertion_count);
void test_swapItem_nullptr(int *assertion_count);

/*
 * main.c is the test runner for this file, calls each function
 * @param: int argc, argument counter
 * @param: char *argv[] is the argument variables
 * @return: int, status code, 0 if succcess, non-zero if something went wrong
 */
int main (int argc, char *argv[]){
	int test_count = 0;
	int assertion_count = 0;

	//seed rand
	srand(time(NULL));

	printf("Now running test_dynamicarray.c\n");

	printf("test_initArr_1()\n");
	test_initArr_1(&assertion_count); 
	test_count++;

	printf("test_initArr_2()\n");
	test_initArr_2(&assertion_count);
	test_count++;

	printf("test_destroyArr_1()\n");
	test_destroyArr_1(&assertion_count);
	test_count++;

	printf("test_destroyArr_2()\n");
	test_destroyArr_2(&assertion_count);
	test_count++;

	printf("test_destroyArr_nullptr()\n");
	test_destroyArr_nullptr(&assertion_count);
	test_count++;

	printf("test_appendItem_normal()\n");
	test_appendItem_normal(&assertion_count);
	test_count++;

	printf("test_appendItem_size_eq_capacity()\n");
	test_appendItem_size_eq_capacity(&assertion_count);
	test_count++;

	printf("test_appendItem_nullptr()\n");
	test_appendItem_nullptr(&assertion_count);
	test_count++;

	printf("test_setItem_normal()\n");
	test_setItem_normal(&assertion_count);
	test_count++;

	printf("test_setItem_set_idx_gr()\n");
	test_setItem_set_idx_gr(&assertion_count);
	test_count++;

	printf("test_setItem_idx_neg()\n");
	test_setItem_idx_neg(&assertion_count);
	test_count++;

	printf("test_setItem_nullptr()\n");
	test_setItem_nullptr(&assertion_count);
	test_count++;

	printf("test_removeItem_normal()\n");
	test_removeItem_normal(&assertion_count);
	test_count++;

	printf("test_removeItem_last_idx()\n");
	test_removeItem_last_idx(&assertion_count);
	test_count++;

	printf("test_removeItem_idx_gr()\n");
	test_removeItem_idx_gr(&assertion_count);
	test_count++;

	printf("test_removeItem_idx_neg()\n");
	test_removeItem_idx_neg(&assertion_count);
	test_count++;

	printf("test_removeItem_nullptr()\n");
	test_removeItem_nullptr(&assertion_count);
	test_count++;

	printf("test_length_1()\n");
	test_length_1(&assertion_count);
	test_count++;

	printf("test_length_2()\n");
	test_length_2(&assertion_count);
	test_count++;

	printf("test_contains_normal()\n");
	test_contains_normal(&assertion_count);
	test_count++;

	printf("test_contains_not_found()\n");
	test_contains_not_found(&assertion_count);
	test_count++;

	printf("test_contains_nullptr()\n");
	test_contains_nullptr(&assertion_count);
	test_count++;

	printf("test_getItem_normal()\n");
	test_getItem_normal(&assertion_count);
	test_count++;

	printf("test_getItem_idx_gr()\n");
	test_getItem_idx_gr(&assertion_count);
	test_count++;

	printf("test_getItem_idx_neg()\n");
	test_getItem_idx_neg(&assertion_count);
	test_count++;

	printf("test_getItem_nullptr()\n");
	test_getItem_nullptr(&assertion_count);
	test_count++;

	printf("test_swapItem_normal()\n");
	test_swapItem_normal(&assertion_count);
	test_count++;

	printf("test_swapItem_idx_gr()\n");
	test_swapItem_idx_gr(&assertion_count);
	test_count++;

	printf("test_swapItem_idx_neg()\n");
	test_swapItem_idx_neg(&assertion_count);
	test_count++;

	printf("test_swapItem_nullptr()\n");
	test_swapItem_nullptr(&assertion_count);
	test_count++;

	printf("%d tests ran\n", test_count);
	printf("%d assertions ran \n", assertion_count);


	return 0;
}


/*
 * test_initArr_1() is the first unit test of initArr()
 * remember caller of initArr() has to check if (*intArr.arr) != NULL)
 * @param: int &assertion_count, takes the address of a counter to increment
 * @return: void
 */
void test_initArr_1 (int *assertion_count) {	
	intArr result = initArr();

	intArr expected;
	expected.arr = calloc(10, sizeof(*expected.arr));
	expected.capacity = 10;
	expected.size = 0;	

	if (expected.arr == NULL){
		printf("test_initArr_1(): test invalid, expected arr pointed is NULL\n");
		assert (expected.arr != NULL); 
		(*assertion_count)++;
		return;
	}
	
	printf("test_initArr_1(): asserting result.arr != NULL\n");
	assert (result.arr != NULL); 
	(*assertion_count)++;

	printf("test_initArr_1(): asserting result.capacity == expected.capacity\n");
	assert (result.capacity == expected.capacity);
	(*assertion_count)++;

	for (int i = 0; i < result.capacity; i++){
		printf("test_initArr_1(): asserting result.arr index %d intialized to 0\n", i);
		assert (result.arr[i] == 0); 
		(*assertion_count)++;
	}
	
	printf("test_initArr_1(): asserting result.size == 0\n");
	assert (result.size == expected.size); 
	(*assertion_count)++; 
	
	/*
	 * instead of using destoryArr(), keeping function testing independent
	 * If destroyArr(); doesn't work, doesn't cause memory leak
	 * structs get automatally forgotten after test ends
	 */	
	free(expected.arr); 
	free(result.arr);
}

/*
 * test_initArr_2() second unit tset of initArr()
 * remember caller of initArr() has to check if (*intArr.arr != NULL)
 * @param: int &assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_initArr_2 (int *assertion_count) {
	
	intArr result = initArr();
	
	intArr expected;
	expected.arr = calloc (10,sizeof(*expected.arr));
	expected.capacity = 10;
	expected.size = 0;

	if (expected.arr == NULL){
		printf("test_initArr_2(): test invalid, expected arr pointed is NULL\n");
	
		assert (expected.arr != NULL); 
		(*assertion_count)++;
		return;
	}
	printf ("test_initArr_2(): asserting result.arr != NULL\n");
	assert (result.arr != NULL); 
	(*assertion_count)++;

	printf ("test_initArr_2(): asserting result.capacity == expected.capacity\n");
	assert (result.capacity == expected.capacity); 
	(*assertion_count)++;

	printf ("test_initArr_2(): asserting result.size == expected.size\n");
	assert (result.size == expected.size);
	(*assertion_count)++;	

	//instead of using destroyArr(), keeping function testing independent
	//if destroyArr(); doesn't work, doesn't cause memory leak
	//structs get automatically forgotten after function
	free(expected.arr);
	free(result.arr);	
}

/*
 * test_destroyArr_1() first unit test of destroyArr()
 * @param: int &assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_destroyArr_1 (int *assertion_count) {

	// essentially initArr(); but don't use initArr() for test of 
	// other functions
	intArr result = (intArr){
		.arr = calloc(10, sizeof(*result.arr)),
		.capacity = 10,
		.size = 0,
	};


	intArr expected = (intArr){
		.arr = NULL,
		.capacity = -1,
		.size= -1,
	};
	if(result.arr == NULL){
		printf("test_destroyArr_1(): result.arr == NULL, Aborting Unit tests\n");

		// this way dooesn't follow the structure in the rest of the
		// file because i'm too lazy to refactor and make the
		// structures match
		free(result.arr);
		result.arr = NULL; 

		return;
	}	

	destroyArr(&result);

	printf("test_destroyArr_1(): asserting result.size == expected.size\n");
	assert(result.size == expected.size); 
	(*assertion_count)++;

	printf ("test_destroyArr_1(): asserting result.capacity == expected.capacity\n");
	assert (result.capacity == expected.capacity); 
	(*assertion_count)++;

	printf ("test_destroyArr_1(): asserting result.arr == expected.arr\n");
	assert (result.arr == expected.arr); 
	(*assertion_count)++;	
	
	free(result.arr);
	result.arr = NULL; 
	
}

/*
 * test_destroyarr_2() second unit test of destroyArr()
 * @param: int &assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_destroyArr_2 (int *assertion_count) {
	// essentially initArr(); but don't use initArr() for test of
	// other functions
	intArr result = (intArr){
		.arr = calloc(10, sizeof(*result.arr)),
		.capacity = 10,
		.size = 0,
	};
	
	intArr expected = (intArr){
		.arr = NULL,
		.capacity = -1,
		.size = -1,
	};

	destroyArr(&result);

	printf("test_destroyArr_2(): asserting result.size == expected.size\n");
	assert (result.size == expected.size); 
	(*assertion_count)++;

	printf ("test_destroyArr_2(): asserting result.capacity == expected.capacity\n");
	assert (result.capacity == expected.capacity);
	(*assertion_count)++;

	printf("test_destroyArr_2(): asserting result.arr == expected.arr\n");
	assert(result.arr == expected.arr);
	(*assertion_count)++;

	free(result.arr);
	result.arr = NULL;
	
}

	


/*
 * test_destroyArr_nullptr() unit test of destroyArr() with NULL input
 * destroyArr(NULL) has nothing to assert, it just can't crash, then an
 * already destroyed array (arr == NULL) gets destroyed again
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_destroyArr_nullptr (int *assertion_count) {
	// imagine an array that has already gone through destroyArr()
	// and is getting destroyed a second time
	intArr result = (intArr){
		.arr = NULL,
		.capacity = -1,
		.size = -1,
	};

	intArr expected = (intArr){
		.arr = NULL,
		.capacity = -1,
		.size = -1,
	};

	// if this returns at all, the NULL check worked
	destroyArr(NULL);

	destroyArr(&result);

	printf("test_destroyArr_nullptr(): asserting result.size == expected.size\n");
	assert(result.size == expected.size);
	(*assertion_count)++;

	printf("test_destroyArr_nullptr(): asserting result.capacity == expected.capacity\n");
	assert(result.capacity == expected.capacity);
	(*assertion_count)++;

	printf("test_destroyArr_nullptr(): asserting result.arr == expected.arr\n");
	assert(result.arr == expected.arr);
	(*assertion_count)++;
}

/*
 * test_appendItem_normal() is the intended runthrough of appendItem()
 * @param: int &assertion_count, takes address of a counter to increment 
 * @return: void
 */
void test_appendItem_normal (int *assertion_count) {
	const int EXPECTED_EXIT_STATUS = 0;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*result.arr)),
		.capacity = 10,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*expected.arr)),
		.capacity = 10,
		.size = 0, 
	};

	 if (result.arr == NULL){
		printf("test_appendItem_normal(): result.arr == NULL, Aborting unit tests\n");

	}else if (expected.arr == NULL){
		printf("test_appendItem_normal(): expected.arr == NULL, Aborting unit tests\n");

	} else {
		expected.arr[0] = 4; 
		expected.size++; 

		int result_status = appendItem(&result, 4);

		printf("test_appendItem_normal(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		// important to differentiate the pointer->struct values opposed to the regular struct
		printf("test_appendItem_normal(): asserting result.arr[0] == expected.arr[0]\n");
		assert(result.arr[0] == expected.arr[0]);
		(*assertion_count)++;

		printf("test_appendItem_normal(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		printf("test_appendItem_normal(): asserting result.capacity == expected.capacity\n");
		assert(result.capacity == expected.capacity);
		(*assertion_count)++;
	}

	free(expected.arr);
	expected.arr = NULL;

	free(result.arr);
	result.arr = NULL; 

}


/*
 * test_appendItem_size_eq_capacity()
 * This test runs through the case where capcity is full and arrray needs to
 * be resized
 * @param: int &assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_appendItem_size_eq_capacity (int *assertion_count) {
	// initial expected of capacity from intArr()
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int TEST_VALUE = rand() % 10;
	printf("test_appendItem_size_eq_capacity(): TEST_VALUE = %d\n", TEST_VALUE);

	intArr result = (intArr){

		// -> higher precedence than * (no parantheses needed)
		.arr = calloc(10, sizeof(*result.arr)),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(20, sizeof(*expected.arr)),
		.capacity = 20,
		.size = 0,
	};
	
	// setting seed for size == capacity for both expected and size
	// using rand to generate numbers	
	if(result.capacity == INITIAL_EXPECTED_CAPACITY && result.arr != NULL && expected.arr != NULL){
		for(int i = 0; i < INITIAL_EXPECTED_CAPACITY; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			// same values as result, so we can check they survive the realloc()
			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		int result_status = appendItem(&result, TEST_VALUE);
		expected.arr[10] = TEST_VALUE;
		expected.size++;

		printf("test_appendItem_size_eq_capacity(): asserting result_status == 0\n");
		assert(result_status == 0);
		(*assertion_count)++;

		printf("test_appendItem_size_eq_capacity(): asserting result.size == expected.size\n");
		assert(result.size == expected.size); 
		(*assertion_count)++;
		
		// result->capacity should double and equal expected.capacity (20)
		printf("test_appendItem_size_eq_capacity(): asserting result.capacity == expected.capacity\n");
		assert(result.capacity == expected.capacity); 
		(*assertion_count)++;
		
		// both should equal TEST_VALUE
		printf("test_appendItem_size_eq_capacity(): asserting result.arr[10] == expected.arr[10]\n");
		assert(result.arr[10] == expected.arr[10]);
		(*assertion_count)++;

		printf("test_appendItem_size_eq_capacity(): asserting result.arr[10] == TEST_VALUE\n");
		assert(result.arr[10] == TEST_VALUE);
		(*assertion_count)++;

		// the original 10 items should have been moved over by realloc()
		for(int i = 0; i < INITIAL_EXPECTED_CAPACITY; i++){
			printf("test_appendItem_size_eq_capacity(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
		

	} else if (result.arr == NULL){
		printf("test_appendItem_size_eq_capacity(): result.arr == NULL, Aborting unit tests\n");

	} else if(expected.arr == NULL){
		printf("test_appendItem_size_eq_capacity(): expected.arr == NULL, Aborting Unit test\n");

	} else if (result.capacity != INITIAL_EXPECTED_CAPACITY){
		printf("test_appendItem_size_eq_capacity(): initalized capacity for result is not accurate, aborting unit tests\n");
		
	}	

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_appendItem_nullptr(): this test case runs through the case
 * if the inputted pointer is NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */ 
void test_appendItem_nullptr(int *assertion_count){
	const int EXPECTED_EXIT_STATUS = 1;
	const int RANDOM_SEED_NUM = rand() % 100;	

	// imagine an array that has somehow gotten out of
	// destoryArr() and is getting inputter into appendItem()
	// capacity = 0 so it gets pass the resizing
	intArr testarr = (intArr){
		.arr = NULL,
		.capacity = 0,
		.size = -1,
	};

	int result = appendItem(&testarr, RANDOM_SEED_NUM);
		

	printf("test_appendItem_nullptr(): asserting result == expected exit status when arr->arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;

	// now the intArr pointer itself is NULL
	result = appendItem(NULL, RANDOM_SEED_NUM);

	printf("test_appendItem_nullptr(): asserting result == expected exit status when arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;
}

/*
 * test_setItem_normal() unit test of intended run through of setItem()
 * @return: void
 * @param: int *assertion_count, takes address of a counter to increment
 */
void test_setItem_normal (int *assertion_count) { 
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 0;

	// set array size for both expected and result (1-10)
	// +1 so it's never 0, rand() % 0 below would divide by zero
	const int SET_ARR_SIZE = rand() % 10 + 1;
	const int SELECTED_IDX = rand() % SET_ARR_SIZE;
	const int TEST_VALUE = rand() % 100;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_setItem_normal(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_setItem_normal(): expected.arr == NULL, Aborting unit tests\n");

	}else {	
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100; 
			result.size++; 

			expected.arr[i] = result.arr[i];
			expected.size++;

		}

		int result_status = setItem(&result, TEST_VALUE, SELECTED_IDX);
		expected.arr[SELECTED_IDX] = TEST_VALUE;

		printf("test_setItem_normal(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_setItem_normal(): asserting "
			"expected.arr[SELECTED_IDX] == result.arr[SELECTED_IDX]\n");
		assert(result.arr[SELECTED_IDX] == expected.arr[SELECTED_IDX]);
		(*assertion_count)++;

		printf("test_setItem_normal(): asserting "
			"result.capacity == expected.capacity\n");
		assert(result.capacity == expected.capacity);
		(*assertion_count)++;

		printf("test_setItem_normal(): asserting "
			"result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_setItem_set_idx_gr()
 * unit test testing where the set idx is bigger than arr->size, but still
 * inside of arr->capacity, so the memory is there but it isn't an item yet.
 * setItem() should refuse it with exit status 2 and leave the array untouched
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_setItem_set_idx_gr (int *assertion_count) {
	// capacity of an intArr after doubling once from 10
	const int EXPECTED_CAPACITY = 20;
	const int EXPECTED_EXIT_STATUS = 2;

	// set array size for both expected and result (10-19)
	const int SET_ARR_SIZE = rand() % 10 + 10;

	// whatever SET_ARR_SIZE, the idx that is to be set must be in one
	// of the indicies from SET_ARR_SIZE-19, inside capacity but past size
	const int SELECTED_IDX = SET_ARR_SIZE + rand() % (EXPECTED_CAPACITY - SET_ARR_SIZE);

	// +1 so TEST_VALUE is never 0, otherwise a write into calloc()'s
	// zeroed out memory past size wouldn't show up
	const int TEST_VALUE = rand() % 100 + 1;

	intArr result = (intArr){
		.arr = calloc(EXPECTED_CAPACITY, sizeof(*(result.arr))),
		.capacity = EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(EXPECTED_CAPACITY, sizeof(*(expected.arr))),
		.capacity = EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_setItem_set_idx_gr(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_setItem_set_idx_gr(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		int result_status = setItem(&result, TEST_VALUE, SELECTED_IDX);

		printf("test_setItem_set_idx_gr(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		// idx == arr->size is the first idx that is out of range, so
		// it always gets checked, not just when rand() lands on it
		result_status = setItem(&result, TEST_VALUE, SET_ARR_SIZE);

		printf("test_setItem_set_idx_gr(): asserting result_status == EXPECTED_EXIT_STATUS when idx == result.size\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_setItem_set_idx_gr(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		// nothing should have been written, not even past result.size
		for(int i = 0; i < EXPECTED_CAPACITY; i++){
			printf("test_setItem_set_idx_gr(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_setItem_idx_neg()
 * unit test testing where the set idx is negative, setItem() should refuse
 * it with exit status 2 instead of writing before the start of arr
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_setItem_idx_neg (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 2;

	// set array size for result (1-10), never empty so it's the
	// negative idx getting refused and not an empty array
	const int SET_ARR_SIZE = rand() % 10 + 1;

	// any negative idx (-10 to -1)
	const int SELECTED_IDX = -(rand() % 10 + 1);
	const int TEST_VALUE = rand() % 100;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_setItem_idx_neg(): result.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;
		}

		int result_status = setItem(&result, TEST_VALUE, SELECTED_IDX);

		printf("test_setItem_idx_neg(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_setItem_idx_neg(): asserting result.size == SET_ARR_SIZE\n");
		assert(result.size == SET_ARR_SIZE);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;
}

/*
 * test_setItem_nullptr(): this test case runs through the case
 * if the inputted pointer, or the arr inside of it, is NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_setItem_nullptr (int *assertion_count) {
	const int EXPECTED_EXIT_STATUS = 1;
	const int TEST_VALUE = rand() % 100;

	// imagine an intArr whose arr was freed and set to NULL by hand
	// (like at the end of these tests) instead of going through
	// destroyArr(), so size and capacity still look normal
	intArr testarr = (intArr){
		.arr = NULL,
		.capacity = 10,
		.size = 10,
	};

	// idx 0 is in range, so only the NULL check can stop it
	int result = setItem(&testarr, TEST_VALUE, 0);

	printf("test_setItem_nullptr(): asserting result == EXPECTED_EXIT_STATUS when arr->arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;

	result = setItem(NULL, TEST_VALUE, 0);

	printf("test_setItem_nullptr(): asserting result == EXPECTED_EXIT_STATUS when arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;
}

/*
 * test_removeItem_normal()
 * unit test of intended runthrough, removes an item that isn't the last
 * one, every item after it should slide over one idx to the left
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeItem_normal (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 0;

	// set array size for both expected and result (2-10)
	const int SET_ARR_SIZE = rand() % 9 + 2;

	// any idx except the last one (0 to SET_ARR_SIZE - 2), so there is
	// always at least one item that has to slide over
	const int SELECTED_IDX = rand() % (SET_ARR_SIZE - 1);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_removeItem_normal(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_removeItem_normal(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			// expected gets every item except the one being removed
			if(i != SELECTED_IDX){
				expected.arr[expected.size] = result.arr[i];
				expected.size++;
			}
		}

		int result_status = removeItem(&result, SELECTED_IDX);

		printf("test_removeItem_normal(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_removeItem_normal(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		printf("test_removeItem_normal(): asserting result.capacity == expected.capacity\n");
		assert(result.capacity == expected.capacity);
		(*assertion_count)++;

		for(int i = 0; i < expected.size; i++){
			printf("test_removeItem_normal(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_removeItem_last_idx()
 * unit test where the removed item is the last one, nothing has to
 * slide over, size just goes down by one
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeItem_last_idx (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 0;

	// set array size for both expected and result (1-10)
	const int SET_ARR_SIZE = rand() % 10 + 1;
	const int SELECTED_IDX = SET_ARR_SIZE - 1;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_removeItem_last_idx(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_removeItem_last_idx(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			// expected gets every item except the last one
			if(i != SELECTED_IDX){
				expected.arr[i] = result.arr[i];
				expected.size++;
			}
		}

		int result_status = removeItem(&result, SELECTED_IDX);

		printf("test_removeItem_last_idx(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_removeItem_last_idx(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		for(int i = 0; i < expected.size; i++){
			printf("test_removeItem_last_idx(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_removeItem_idx_gr()
 * unit test where the idx to remove is >= arr->size, removeItem() should
 * refuse it with exit status 2 and not remove anything
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeItem_idx_gr (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 2;

	// set array size for both expected and result (0-9), 0 is an
	// empty array where every idx is out of range
	const int SET_ARR_SIZE = rand() % 10;

	// whatever SET_ARR_SIZE, the idx is from SET_ARR_SIZE-9, inside
	// capacity but past size
	const int SELECTED_IDX = SET_ARR_SIZE + rand() % (INITIAL_EXPECTED_CAPACITY - SET_ARR_SIZE);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_removeItem_idx_gr(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_removeItem_idx_gr(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		int result_status = removeItem(&result, SELECTED_IDX);

		printf("test_removeItem_idx_gr(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		// idx == arr->size is the first idx that is out of range, so
		// it always gets checked, not just when rand() lands on it
		result_status = removeItem(&result, SET_ARR_SIZE);

		printf("test_removeItem_idx_gr(): asserting result_status == EXPECTED_EXIT_STATUS when idx == result.size\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_removeItem_idx_gr(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		for(int i = 0; i < expected.size; i++){
			printf("test_removeItem_idx_gr(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_removeItem_idx_neg()
 * unit test where the idx to remove is negative, removeItem() should
 * refuse it with exit status 2 and not remove anything
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeItem_idx_neg (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 2;

	// set array size for both expected and result (1-10), never empty so
	// it's the negative idx getting refused and not an empty array
	const int SET_ARR_SIZE = rand() % 10 + 1;

	// any negative idx (-10 to -1)
	const int SELECTED_IDX = -(rand() % 10 + 1);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_removeItem_idx_neg(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_removeItem_idx_neg(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		int result_status = removeItem(&result, SELECTED_IDX);

		printf("test_removeItem_idx_neg(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_removeItem_idx_neg(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		for(int i = 0; i < expected.size; i++){
			printf("test_removeItem_idx_neg(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_removeItem_nullptr(): this test case runs through the case
 * if the inputted pointer, or the arr inside of it, is NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeItem_nullptr (int *assertion_count) {
	const int EXPECTED_EXIT_STATUS = 1;

	// imagine an intArr whose arr was freed and set to NULL by hand
	// (like at the end of these tests) instead of going through
	// destroyArr(), so size and capacity still look normal
	intArr testarr = (intArr){
		.arr = NULL,
		.capacity = 10,
		.size = 10,
	};

	// idx 0 is in range, so only the NULL check can stop it
	int result = removeItem(&testarr, 0);

	printf("test_removeItem_nullptr(): asserting result == EXPECTED_EXIT_STATUS when arr->arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;

	result = removeItem(NULL, 0);

	printf("test_removeItem_nullptr(): asserting result == EXPECTED_EXIT_STATUS when arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;
}

/*
 * test_length_1(), tests length function
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_length_1(int *assertion_count){
	const int ARR_LENGTH = rand() % 10;
	const int INITIAL_CAPACITY = 10;
	intArr result = (intArr){
		.arr = calloc(10, sizeof(*result.arr)),
		.capacity = INITIAL_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_length_1(): result->arr == NULL, Aborting unit tests\n");

	} else {
		// seeding goes after the NULL check, so it never writes through NULL
		for (int i = 0; i <= ARR_LENGTH; i++){
			result.arr[i] = rand() % 100;
			result.size++;
		}

		int result_len = length(&result);

		printf("test_length_1(): asserting result.size == result_len\n");

		assert(result.size == result_len);
		(*assertion_count)++;

		// i <= ARR_LENGTH puts in ARR_LENGTH + 1 items (1-10)
		printf("test_length_1(): asserting result_len == ARR_LENGTH + 1\n");
		assert(result_len == ARR_LENGTH + 1);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;	
}

/*
 * test_length_2(), second unit test of length function, an empty intArr
 * should have a length of 0
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_length_2(int *assertion_count){
	const int INITIAL_CAPACITY = 10;
	intArr result = (intArr){
		.arr = calloc(10, sizeof(*result.arr)),
		.capacity = INITIAL_CAPACITY,
		.size = 0,
	};

	if (result.arr == NULL){
		printf("test_length_2(): result.arr == NULL, Aborting unit tests\n");
	} else {
		int result_len = length(&result);

		printf("test_length_2(): asserting result.size == result_len\n");
		assert(result.size == result_len);
		(*assertion_count)++;

		printf("test_length_2(): asserting result_len == 0\n");
		assert(result_len == 0);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;
}

/*
 * test_contains_normal() unit test of intended run through of contains(),
 * picks a value that is in the array and checks that contains() finds it
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_contains_normal (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_RESULT = 1;

	// set array size for result (1-10)
	const int SET_ARR_SIZE = rand() % 10 + 1;
	const int SELECTED_IDX = rand() % SET_ARR_SIZE;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_contains_normal(): result.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;
		}

		// the value being searched for is whatever landed at SELECTED_IDX
		const int TEST_VALUE = result.arr[SELECTED_IDX];

		int result_found = contains(&result, TEST_VALUE);

		printf("test_contains_normal(): asserting result_found == EXPECTED_RESULT\n");
		assert(result_found == EXPECTED_RESULT);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;
}

/*
 * test_contains_not_found() unit test where the value is not in the
 * array, contains() should return 0
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_contains_not_found (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_RESULT = 0;

	// set array size for result (0-10), 0 is an empty array which
	// can't contain anything
	const int SET_ARR_SIZE = rand() % 11;

	// items are seeded from 0-99, so 100-199 can never be in the array
	const int TEST_VALUE = rand() % 100 + 100;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_contains_not_found(): result.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;
		}

		int result_found = contains(&result, TEST_VALUE);

		printf("test_contains_not_found(): asserting result_found == EXPECTED_RESULT\n");
		assert(result_found == EXPECTED_RESULT);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;
}

/*
 * test_contains_nullptr(): this test case runs through the case
 * if the inputted pointer, or the arr inside of it, is NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_contains_nullptr (int *assertion_count) {
	const int EXPECTED_RESULT = -1;
	const int TEST_VALUE = rand() % 100;

	// imagine an intArr whose arr was freed and set to NULL by hand
	// (like at the end of these tests) instead of going through
	// destroyArr(), so size and capacity still look normal
	intArr testarr = (intArr){
		.arr = NULL,
		.capacity = 10,
		.size = 10,
	};

	int result = contains(&testarr, TEST_VALUE);

	printf("test_contains_nullptr(): asserting result == EXPECTED_RESULT when arr->arr == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;

	result = contains(NULL, TEST_VALUE);

	printf("test_contains_nullptr(): asserting result == EXPECTED_RESULT when arr == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;
}

/*
 * test_getItem_normal() unit test of intended run through of getItem()
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_getItem_normal (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;

	// set array size for both expected and result (1-10)
	const int SET_ARR_SIZE = rand() % 10 + 1;
	const int SELECTED_IDX = rand() % SET_ARR_SIZE;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_getItem_normal(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_getItem_normal(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		int result_item = getItem(&result, SELECTED_IDX);

		printf("test_getItem_normal(): asserting result_item == expected.arr[SELECTED_IDX]\n");
		assert(result_item == expected.arr[SELECTED_IDX]);
		(*assertion_count)++;

		// getItem() only reads, so nothing else should change
		printf("test_getItem_normal(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_getItem_idx_gr() unit test where the idx is >= arr->size, getItem()
 * should return -1 instead of reading past the last item
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_getItem_idx_gr (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_RESULT = -1;

	// set array size for result (0-9)
	const int SET_ARR_SIZE = rand() % 10;

	// whatever SET_ARR_SIZE, the idx is from SET_ARR_SIZE-9, inside
	// capacity but past size
	const int SELECTED_IDX = SET_ARR_SIZE + rand() % (INITIAL_EXPECTED_CAPACITY - SET_ARR_SIZE);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_getItem_idx_gr(): result.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;
		}

		int result_item = getItem(&result, SELECTED_IDX);

		printf("test_getItem_idx_gr(): asserting result_item == EXPECTED_RESULT\n");
		assert(result_item == EXPECTED_RESULT);
		(*assertion_count)++;

		// idx == arr->size is the first idx that is out of range, so
		// it always gets checked, not just when rand() lands on it
		result_item = getItem(&result, SET_ARR_SIZE);

		printf("test_getItem_idx_gr(): asserting result_item == EXPECTED_RESULT when idx == result.size\n");
		assert(result_item == EXPECTED_RESULT);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;
}

/*
 * test_getItem_idx_neg() unit test where the idx is negative, getItem()
 * should return -1 instead of reading before the start of arr
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_getItem_idx_neg (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_RESULT = -1;

	// set array size for result (1-10), never empty so it's the
	// negative idx getting refused and not an empty array
	const int SET_ARR_SIZE = rand() % 10 + 1;

	// any negative idx (-10 to -1)
	const int SELECTED_IDX = -(rand() % 10 + 1);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_getItem_idx_neg(): result.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;
		}

		int result_item = getItem(&result, SELECTED_IDX);

		printf("test_getItem_idx_neg(): asserting result_item == EXPECTED_RESULT\n");
		assert(result_item == EXPECTED_RESULT);
		(*assertion_count)++;
	}

	free(result.arr);
	result.arr = NULL;
}

/*
 * test_getItem_nullptr(): this test case runs through the case
 * if the inputted pointer, or the arr inside of it, is NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_getItem_nullptr (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// imagine an intArr whose arr was freed and set to NULL by hand
	// (like at the end of these tests) instead of going through
	// destroyArr(), so size and capacity still look normal
	intArr testarr = (intArr){
		.arr = NULL,
		.capacity = 10,
		.size = 10,
	};

	// idx 0 is in range, so only the NULL check can stop it
	int result = getItem(&testarr, 0);

	printf("test_getItem_nullptr(): asserting result == EXPECTED_RESULT when arr->arr == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;

	result = getItem(NULL, 0);

	printf("test_getItem_nullptr(): asserting result == EXPECTED_RESULT when arr == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;
}

/*
 * test_swapItem_normal() unit test of intended run through of swapItem()
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_swapItem_normal (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = 0;

	// set array size for both expected and result (2-10), need at least
	// 2 items to have something to swap
	const int SET_ARR_SIZE = rand() % 9 + 2;
	const int SELECTED_IDX1 = rand() % SET_ARR_SIZE;

	// any idx except SELECTED_IDX1, wraps around past the end, so a
	// swapItem() that does nothing can't pass by swapping an idx with itself
	const int SELECTED_IDX2 = (SELECTED_IDX1 + 1 + rand() % (SET_ARR_SIZE - 1)) % SET_ARR_SIZE;

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_swapItem_normal(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_swapItem_normal(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			// + i * 100 so every item is different, two equal items
			// would look the same swapped or not
			result.arr[i] = rand() % 100 + i * 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		// swap by hand on expected
		int temp = expected.arr[SELECTED_IDX1];
		expected.arr[SELECTED_IDX1] = expected.arr[SELECTED_IDX2];
		expected.arr[SELECTED_IDX2] = temp;

		int result_status = swapItem(&result, SELECTED_IDX1, SELECTED_IDX2);

		printf("test_swapItem_normal(): asserting result_status == EXPECTED_EXIT_STATUS\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		printf("test_swapItem_normal(): asserting result.size == expected.size\n");
		assert(result.size == expected.size);
		(*assertion_count)++;

		for(int i = 0; i < expected.size; i++){
			printf("test_swapItem_normal(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_swapItem_idx_gr() unit test where one of the idx is >= arr->size,
 * swapItem() should return -1 and not swap anything
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_swapItem_idx_gr (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = -1;

	// set array size for both expected and result (1-9), need at least
	// 1 item for the good idx, and room in capacity for the bad idx
	const int SET_ARR_SIZE = rand() % 9 + 1;
	const int GOOD_IDX = rand() % SET_ARR_SIZE;

	// whatever SET_ARR_SIZE, the bad idx is from SET_ARR_SIZE-9, inside
	// capacity but past size
	const int BAD_IDX = SET_ARR_SIZE + rand() % (INITIAL_EXPECTED_CAPACITY - SET_ARR_SIZE);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_swapItem_idx_gr(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_swapItem_idx_gr(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		// bad idx as idx2
		int result_status = swapItem(&result, GOOD_IDX, BAD_IDX);

		printf("test_swapItem_idx_gr(): asserting result_status == EXPECTED_EXIT_STATUS when idx2 is out of range\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		// bad idx as idx1
		result_status = swapItem(&result, BAD_IDX, GOOD_IDX);

		printf("test_swapItem_idx_gr(): asserting result_status == EXPECTED_EXIT_STATUS when idx1 is out of range\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		// nothing should have moved, not even past result.size
		for(int i = 0; i < INITIAL_EXPECTED_CAPACITY; i++){
			printf("test_swapItem_idx_gr(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_swapItem_idx_neg() unit test where one of the idx is negative,
 * swapItem() should return -1 and not swap anything
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_swapItem_idx_neg (int *assertion_count) {
	const int INITIAL_EXPECTED_CAPACITY = 10;
	const int EXPECTED_EXIT_STATUS = -1;

	// set array size for both expected and result (1-10)
	const int SET_ARR_SIZE = rand() % 10 + 1;
	const int GOOD_IDX = rand() % SET_ARR_SIZE;

	// any negative idx (-10 to -1)
	const int BAD_IDX = -(rand() % 10 + 1);

	intArr result = (intArr){
		.arr = calloc(10, sizeof(*(result.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	intArr expected = (intArr){
		.arr = calloc(10, sizeof(*(expected.arr))),
		.capacity = INITIAL_EXPECTED_CAPACITY,
		.size = 0,
	};

	if(result.arr == NULL){
		printf("test_swapItem_idx_neg(): result.arr == NULL, Aborting unit tests\n");

	}else if(expected.arr == NULL){
		printf("test_swapItem_idx_neg(): expected.arr == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_ARR_SIZE; i++){
			result.arr[i] = rand() % 100;
			result.size++;

			expected.arr[i] = result.arr[i];
			expected.size++;
		}

		// bad idx as idx2
		int result_status = swapItem(&result, GOOD_IDX, BAD_IDX);

		printf("test_swapItem_idx_neg(): asserting result_status == EXPECTED_EXIT_STATUS when idx2 is negative\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		// bad idx as idx1
		result_status = swapItem(&result, BAD_IDX, GOOD_IDX);

		printf("test_swapItem_idx_neg(): asserting result_status == EXPECTED_EXIT_STATUS when idx1 is negative\n");
		assert(result_status == EXPECTED_EXIT_STATUS);
		(*assertion_count)++;

		for(int i = 0; i < expected.size; i++){
			printf("test_swapItem_idx_neg(): asserting result.arr index %d == expected.arr index %d\n", i, i);
			assert(result.arr[i] == expected.arr[i]);
			(*assertion_count)++;
		}
	}

	free(result.arr);
	result.arr = NULL;

	free(expected.arr);
	expected.arr = NULL;
}

/*
 * test_swapItem_nullptr(): this test case runs through the case
 * if the inputted pointer, or the arr inside of it, is NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_swapItem_nullptr (int *assertion_count) {
	const int EXPECTED_EXIT_STATUS = -1;

	// imagine an intArr whose arr was freed and set to NULL by hand
	// (like at the end of these tests) instead of going through
	// destroyArr(), so size and capacity still look normal
	intArr testarr = (intArr){
		.arr = NULL,
		.capacity = 10,
		.size = 10,
	};

	// idx 0 and 1 are in range, so only the NULL check can stop it
	int result = swapItem(&testarr, 0, 1);

	printf("test_swapItem_nullptr(): asserting result == EXPECTED_EXIT_STATUS when arr->arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;

	result = swapItem(NULL, 0, 1);

	printf("test_swapItem_nullptr(): asserting result == EXPECTED_EXIT_STATUS when arr == NULL\n");
	assert(result == EXPECTED_EXIT_STATUS);
	(*assertion_count)++;
}
