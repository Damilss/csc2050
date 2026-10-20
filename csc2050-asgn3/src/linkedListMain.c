/**
 * File: linkedListMain.c
 * Author: Emilio Scott
 * Date: 2026-10-12
 *
 * Test case files for linkedList.c and linkedList.h
 * This file should test every function in linkedList.h, both the runs that
 * should work and the runs that should fail, and includes its own main
 *
 * if a test fails, assert() stops the program right there, so the last
 * "asserting ..." line printed is the one that failed
 *
 * LinkedList and Link are only declared in linkedList.h, their fields live
 * in linkedList.c, so these tests can't build a list by hand or look at
 * head and tail. every list gets built with linkedListCreate() and
 * addListBack() (or addFrontDeque()), and gets checked through sizeList()
 * and containsList()
 *
 * containsList() returns the first idx a value is at, so the values put in
 * a list are always different from each other, that way containsList() on
 * each expected value gives back its exact idx, which checks the order of
 * the whole list
 *  - values seeded into a list are 0-999 (rand() % 100 + i * 100)
 *  - values a test adds to a list are 1000-1099
 *  - values that should never be found are -100 to -1
 *
 * nothing can be asserted after deleteList() (the list is freed), so run
 * this under leaks (macOS) or valgrind (linux) to check for memory leaks
 */

#include "linkedList.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void test_linkedListCreate_normal(int *assertion_count);
void test_deleteList_normal(int *assertion_count);
void test_deleteList_nullptr(int *assertion_count);
void test_sizeList_normal(int *assertion_count);
void test_sizeList_empty(int *assertion_count);
void test_sizeList_nullptr(int *assertion_count);
void test_containsList_normal(int *assertion_count);
void test_containsList_not_found(int *assertion_count);
void test_containsList_nullptr(int *assertion_count);
void test_addListFront_normal(int *assertion_count);
void test_addListFront_nullptr(int *assertion_count);
void test_addListBack_normal(int *assertion_count);
void test_addListBack_nullptr(int *assertion_count);
void test_addValueListFront_normal(int *assertion_count);
void test_addValueListFront_head(int *assertion_count);
void test_addValueListFront_not_found(int *assertion_count);
void test_addValueListFront_nullptr(int *assertion_count);
void test_addValueListBack_normal(int *assertion_count);
void test_addValueListBack_tail(int *assertion_count);
void test_addValueListBack_not_found(int *assertion_count);
void test_addValueListBack_nullptr(int *assertion_count);
void test_removeList_normal(int *assertion_count);
void test_removeList_head(int *assertion_count);
void test_removeList_tail(int *assertion_count);
void test_removeList_only_link(int *assertion_count);
void test_removeList_not_found(int *assertion_count);
void test_removeList_nullptr(int *assertion_count);
void test_printList_normal(int *assertion_count);
void test_printList_one_link(int *assertion_count);
void test_printList_empty(int *assertion_count);
void test_printList_nullptr(int *assertion_count);
void test_reverseList_normal(int *assertion_count);
void test_reverseList_twice(int *assertion_count);
void test_reverseList_one_link(int *assertion_count);
void test_reverseList_nullptr(int *assertion_count);
void test_dequeCreate_normal(int *assertion_count);
void test_deleteDeque_normal(int *assertion_count);
void test_deleteDeque_nullptr(int *assertion_count);
void test_sizeDeque_normal(int *assertion_count);
void test_sizeDeque_empty(int *assertion_count);
void test_sizeDeque_nullptr(int *assertion_count);
void test_addFrontDeque_normal(int *assertion_count);
void test_addFrontDeque_nullptr(int *assertion_count);
void test_backDeque_normal(int *assertion_count);
void test_backDeque_nullptr(int *assertion_count);
void test_removeBackDeque_normal(int *assertion_count);
void test_removeBackDeque_one_link(int *assertion_count);
void test_removeBackDeque_nullptr(int *assertion_count);
void test_printDeque_normal(int *assertion_count);
void test_printDeque_nullptr(int *assertion_count);

/**
 * main() is the test runner for this file, calls each test function
 * @param: int argc, argument counter
 * @param: char *argv[], the argument variables
 * @return: int, status code, 0 if success, non-zero if something went wrong
 */
int main(int argc, char *argv[]){
	int test_count = 0;
	int assertion_count = 0;

	// seed rand
	srand(time(NULL));

	printf("Now running linkedListMain.c\n");

	// linked list tests

	printf("test_linkedListCreate_normal()\n");
	test_linkedListCreate_normal(&assertion_count);
	test_count++;

	printf("test_deleteList_normal()\n");
	test_deleteList_normal(&assertion_count);
	test_count++;

	printf("test_deleteList_nullptr()\n");
	test_deleteList_nullptr(&assertion_count);
	test_count++;

	printf("test_sizeList_normal()\n");
	test_sizeList_normal(&assertion_count);
	test_count++;

	printf("test_sizeList_empty()\n");
	test_sizeList_empty(&assertion_count);
	test_count++;

	printf("test_sizeList_nullptr()\n");
	test_sizeList_nullptr(&assertion_count);
	test_count++;

	printf("test_containsList_normal()\n");
	test_containsList_normal(&assertion_count);
	test_count++;

	printf("test_containsList_not_found()\n");
	test_containsList_not_found(&assertion_count);
	test_count++;

	printf("test_containsList_nullptr()\n");
	test_containsList_nullptr(&assertion_count);
	test_count++;

	printf("test_addListFront_normal()\n");
	test_addListFront_normal(&assertion_count);
	test_count++;

	printf("test_addListFront_nullptr()\n");
	test_addListFront_nullptr(&assertion_count);
	test_count++;

	printf("test_addListBack_normal()\n");
	test_addListBack_normal(&assertion_count);
	test_count++;

	printf("test_addListBack_nullptr()\n");
	test_addListBack_nullptr(&assertion_count);
	test_count++;

	printf("test_addValueListFront_normal()\n");
	test_addValueListFront_normal(&assertion_count);
	test_count++;

	printf("test_addValueListFront_head()\n");
	test_addValueListFront_head(&assertion_count);
	test_count++;

	printf("test_addValueListFront_not_found()\n");
	test_addValueListFront_not_found(&assertion_count);
	test_count++;

	printf("test_addValueListFront_nullptr()\n");
	test_addValueListFront_nullptr(&assertion_count);
	test_count++;

	printf("test_addValueListBack_normal()\n");
	test_addValueListBack_normal(&assertion_count);
	test_count++;

	printf("test_addValueListBack_tail()\n");
	test_addValueListBack_tail(&assertion_count);
	test_count++;

	printf("test_addValueListBack_not_found()\n");
	test_addValueListBack_not_found(&assertion_count);
	test_count++;

	printf("test_addValueListBack_nullptr()\n");
	test_addValueListBack_nullptr(&assertion_count);
	test_count++;

	printf("test_removeList_normal()\n");
	test_removeList_normal(&assertion_count);
	test_count++;

	printf("test_removeList_head()\n");
	test_removeList_head(&assertion_count);
	test_count++;

	printf("test_removeList_tail()\n");
	test_removeList_tail(&assertion_count);
	test_count++;

	printf("test_removeList_only_link()\n");
	test_removeList_only_link(&assertion_count);
	test_count++;

	printf("test_removeList_not_found()\n");
	test_removeList_not_found(&assertion_count);
	test_count++;

	printf("test_removeList_nullptr()\n");
	test_removeList_nullptr(&assertion_count);
	test_count++;

	printf("test_printList_normal()\n");
	test_printList_normal(&assertion_count);
	test_count++;

	printf("test_printList_one_link()\n");
	test_printList_one_link(&assertion_count);
	test_count++;

	printf("test_printList_empty()\n");
	test_printList_empty(&assertion_count);
	test_count++;

	printf("test_printList_nullptr()\n");
	test_printList_nullptr(&assertion_count);
	test_count++;

	printf("test_reverseList_normal()\n");
	test_reverseList_normal(&assertion_count);
	test_count++;

	printf("test_reverseList_twice()\n");
	test_reverseList_twice(&assertion_count);
	test_count++;

	printf("test_reverseList_one_link()\n");
	test_reverseList_one_link(&assertion_count);
	test_count++;

	printf("test_reverseList_nullptr()\n");
	test_reverseList_nullptr(&assertion_count);
	test_count++;

	// deque tests

	printf("test_dequeCreate_normal()\n");
	test_dequeCreate_normal(&assertion_count);
	test_count++;

	printf("test_deleteDeque_normal()\n");
	test_deleteDeque_normal(&assertion_count);
	test_count++;

	printf("test_deleteDeque_nullptr()\n");
	test_deleteDeque_nullptr(&assertion_count);
	test_count++;

	printf("test_sizeDeque_normal()\n");
	test_sizeDeque_normal(&assertion_count);
	test_count++;

	printf("test_sizeDeque_empty()\n");
	test_sizeDeque_empty(&assertion_count);
	test_count++;

	printf("test_sizeDeque_nullptr()\n");
	test_sizeDeque_nullptr(&assertion_count);
	test_count++;

	printf("test_addFrontDeque_normal()\n");
	test_addFrontDeque_normal(&assertion_count);
	test_count++;

	printf("test_addFrontDeque_nullptr()\n");
	test_addFrontDeque_nullptr(&assertion_count);
	test_count++;

	printf("test_backDeque_normal()\n");
	test_backDeque_normal(&assertion_count);
	test_count++;

	printf("test_backDeque_nullptr()\n");
	test_backDeque_nullptr(&assertion_count);
	test_count++;

	printf("test_removeBackDeque_normal()\n");
	test_removeBackDeque_normal(&assertion_count);
	test_count++;

	printf("test_removeBackDeque_one_link()\n");
	test_removeBackDeque_one_link(&assertion_count);
	test_count++;

	printf("test_removeBackDeque_nullptr()\n");
	test_removeBackDeque_nullptr(&assertion_count);
	test_count++;

	printf("test_printDeque_normal()\n");
	test_printDeque_normal(&assertion_count);
	test_count++;

	printf("test_printDeque_nullptr()\n");
	test_printDeque_nullptr(&assertion_count);
	test_count++;

	printf("%d tests ran\n", test_count);
	printf("%d assertions ran\n", assertion_count);

	return 0;
}

/* ************************************************************************
	Linked List Tests
************************************************************************ */

/**
 * test_linkedListCreate_normal() unit test of linkedListCreate(), a new list
 * should come back with nothing in it
 * remember caller of linkedListCreate() has to check if the list != NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_linkedListCreate_normal (int *assertion_count) {
	const int EXPECTED_SIZE = 0;
	const int EXPECTED_RESULT = -1;
	const TYPE TEST_VALUE = rand() % 100;

	LinkedList *result = linkedListCreate();

	printf("test_linkedListCreate_normal(): asserting result != NULL\n");
	assert(result != NULL);
	(*assertion_count)++;

	int result_size = sizeList(result);

	printf("test_linkedListCreate_normal(): asserting result_size == EXPECTED_SIZE\n");
	assert(result_size == EXPECTED_SIZE);
	(*assertion_count)++;

	// a brand new list has nothing in it to find
	int result_idx = containsList(result, TEST_VALUE);

	printf("test_linkedListCreate_normal(): asserting result_idx == EXPECTED_RESULT\n");
	assert(result_idx == EXPECTED_RESULT);
	(*assertion_count)++;

	deleteList(result);
	result = NULL;
}

/**
 * test_deleteList_normal() unit test of deleteList() on a list that has
 * links in it, the list is freed after deleteList() so there is nothing left
 * to assert on, run under leaks or valgrind to check every link got freed
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_deleteList_normal (int *assertion_count) {
	// set list size (0-10), 0 is an empty list where the while loop in
	// deleteList() never runs
	const int SET_LIST_SIZE = rand() % 11;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_deleteList_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			addListBack(result, rand() % 100 + i * 100);
		}

		// make sure there really are SET_LIST_SIZE links for deleteList() to free
		int result_size = sizeList(result);

		printf("test_deleteList_normal(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		// if this returns at all, every link got freed without walking off
		// the end of the list
		deleteList(result);
	}

	result = NULL;
}

/**
 * test_deleteList_nullptr() unit test of deleteList() with NULL input,
 * deleteList(NULL) has nothing to assert, it just can't crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_deleteList_nullptr (int *assertion_count) {
	// if this returns at all, the NULL check worked
	deleteList(NULL);
}

/**
 * test_sizeList_normal() unit test of intended run through of sizeList()
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_sizeList_normal (int *assertion_count) {
	// set list size (1-10)
	const int SET_LIST_SIZE = rand() % 10 + 1;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_sizeList_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			addListBack(result, rand() % 100 + i * 100);
		}

		int result_size = sizeList(result);

		printf("test_sizeList_normal(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_sizeList_empty() second unit test of sizeList(), an empty list should
 * have a size of 0
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_sizeList_empty (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_sizeList_empty(): result == NULL, Aborting unit tests\n");

	}else {
		int result_size = sizeList(result);

		printf("test_sizeList_empty(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_sizeList_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, sizeList() should return -1
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_sizeList_nullptr (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	int result = sizeList(NULL);

	printf("test_sizeList_nullptr(): asserting result == EXPECTED_RESULT when list == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;
}

/**
 * test_containsList_normal() unit test of intended run through of
 * containsList(), every value in the list should be found at its own idx
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_containsList_normal (int *assertion_count) {
	// set list size (1-10)
	const int SET_LIST_SIZE = rand() % 10 + 1;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_containsList_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			// + i * 100 so every value is different, two equal values
			// would both give back the idx of the first one
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		// every idx gets checked, idx 0 is checked before the while loop in
		// containsList(), and the tail is the last link the loop reaches
		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_containsList_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_containsList_not_found() unit test where the value is not in the
 * list, containsList() should return -1
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_containsList_not_found (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set list size (1-10), never empty so it's the search coming up empty
	// and not an empty list, the empty list is in test_containsList_nullptr()
	const int SET_LIST_SIZE = rand() % 10 + 1;

	// seeded values are 0-999, so -100 to -1 can never be in the list
	const TYPE TEST_VALUE = -(rand() % 100 + 1);

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_containsList_not_found(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			addListBack(result, rand() % 100 + i * 100);
		}

		// the whole list gets walked, all the way to the tail, before
		// containsList() gives up
		int result_idx = containsList(result, TEST_VALUE);

		printf("test_containsList_not_found(): asserting result_idx == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_containsList_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, or the list is empty (head == NULL)
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_containsList_nullptr (int *assertion_count) {
	const int EXPECTED_RESULT = -1;
	const TYPE TEST_VALUE = rand() % 100;

	int result = containsList(NULL, TEST_VALUE);

	printf("test_containsList_nullptr(): asserting result == EXPECTED_RESULT when list == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;

	// imagine a list with nothing in it, head and tail are both NULL, so
	// only the head == NULL check can stop it
	LinkedList *testlist = linkedListCreate();

	if(testlist == NULL){
		printf("test_containsList_nullptr(): testlist == NULL, Aborting unit tests\n");

	}else {
		result = containsList(testlist, TEST_VALUE);

		printf("test_containsList_nullptr(): asserting result == EXPECTED_RESULT when list is empty\n");
		assert(result == EXPECTED_RESULT);
		(*assertion_count)++;
	}

	deleteList(testlist);
	testlist = NULL;
}

/**
 * test_addListFront_normal() unit test of intended run through of
 * addListFront(), every value added goes in front of the one added before
 * it, so the list ends up in the opposite order the values were added in
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addListFront_normal (int *assertion_count) {
	// set list size (1-10), the first value always goes into an empty list
	const int SET_LIST_SIZE = rand() % 10 + 1;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addListFront_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListFront(result, value);

			// the first value added ends up last
			expected[SET_LIST_SIZE - 1 - i] = value;
		}

		int result_size = sizeList(result);

		printf("test_addListFront_normal(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addListFront_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addListFront_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, there is nothing to assert, it just can't
 * crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_addListFront_nullptr (int *assertion_count) {
	const TYPE TEST_VALUE = rand() % 100;

	// if this returns at all, the NULL check worked
	addListFront(NULL, TEST_VALUE);
}

/**
 * test_addListBack_normal() unit test of intended run through of
 * addListBack(), every value added goes behind the one added before it, so
 * the list ends up in the same order the values were added in
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addListBack_normal (int *assertion_count) {
	// set list size (1-10), the first value always goes into an empty list
	const int SET_LIST_SIZE = rand() % 10 + 1;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addListBack_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		int result_size = sizeList(result);

		printf("test_addListBack_normal(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addListBack_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addListBack_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, there is nothing to assert, it just can't
 * crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_addListBack_nullptr (int *assertion_count) {
	const TYPE TEST_VALUE = rand() % 100;

	// if this returns at all, the NULL check worked
	addListBack(NULL, TEST_VALUE);
}

/**
 * test_addValueListFront_normal() unit test of intended run through of
 * addValueListFront(), TEST_VALUE gets added in front of a value that is in
 * the list but isn't the head, everything from that value on moves back
 * one idx
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListFront_normal (int *assertion_count) {
	// set list size (2-10), need at least 2 links so there is a link that
	// isn't the head to add in front of
	const int SET_LIST_SIZE = rand() % 9 + 2;

	// any idx except the head (1 to SET_LIST_SIZE - 1), the head goes
	// through addListFront() and gets its own test
	const int SELECTED_IDX = rand() % (SET_LIST_SIZE - 1) + 1;

	// seeded values are 0-999, so 1000-1099 is never already in the list
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	// 10 links max, + 1 for TEST_VALUE
	TYPE expected[11];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addValueListFront_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			// expected gets TEST_VALUE right before the selected value
			if(i == SELECTED_IDX){
				selected_value = value;
				expected[expected_size] = TEST_VALUE;
				expected_size++;
			}

			expected[expected_size] = value;
			expected_size++;
		}

		addValueListFront(result, selected_value, TEST_VALUE);

		int result_size = sizeList(result);

		printf("test_addValueListFront_normal(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addValueListFront_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addValueListFront_head() unit test where the value to add in front
 * of is the head, TEST_VALUE should become the new head
 * the head is checked before the while loop in addValueListFront(), so it
 * always gets checked here, not just when rand() lands on it
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListFront_head (int *assertion_count) {
	// set list size (1-10), 1 is a list where the head is also the tail
	const int SET_LIST_SIZE = rand() % 10 + 1;
	const int SELECTED_IDX = 0;

	// seeded values are 0-999, so 1000-1099 is never already in the list
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	// 10 links max, + 1 for TEST_VALUE
	TYPE expected[11];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addValueListFront_head(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			// expected gets TEST_VALUE right before the head
			if(i == SELECTED_IDX){
				selected_value = value;
				expected[expected_size] = TEST_VALUE;
				expected_size++;
			}

			expected[expected_size] = value;
			expected_size++;
		}

		addValueListFront(result, selected_value, TEST_VALUE);

		int result_size = sizeList(result);

		printf("test_addValueListFront_head(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addValueListFront_head(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addValueListFront_not_found() unit test where the value to add in
 * front of is not in the list, nothing should get added
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListFront_not_found (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set list size (1-10), never empty, the empty list is in
	// test_addValueListFront_nullptr()
	const int SET_LIST_SIZE = rand() % 10 + 1;

	// seeded values are 0-999, so -100 to -1 is never in the list
	const TYPE MISSING_VALUE = -(rand() % 100 + 1);
	const TYPE TEST_VALUE = rand() % 100 + 1000;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addValueListFront_not_found(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		addValueListFront(result, MISSING_VALUE, TEST_VALUE);

		// there was nothing to add in front of, so TEST_VALUE should not
		// be anywhere in the list
		int result_idx = containsList(result, TEST_VALUE);

		printf("test_addValueListFront_not_found(): asserting containsList(result, TEST_VALUE) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		int result_size = sizeList(result);

		printf("test_addValueListFront_not_found(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			result_idx = containsList(result, expected[i]);

			printf("test_addValueListFront_not_found(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addValueListFront_nullptr(): this test case runs through the case if
 * the inputted list pointer is NULL, or the list is empty (head == NULL)
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListFront_nullptr (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	// any value, an empty list has nothing in it to add in front of
	const TYPE MISSING_VALUE = rand() % 100;
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	// if this returns at all, the NULL check worked
	addValueListFront(NULL, MISSING_VALUE, TEST_VALUE);

	// imagine a list with nothing in it, head and tail are both NULL, so
	// only the head == NULL check can stop it
	LinkedList *testlist = linkedListCreate();

	if(testlist == NULL){
		printf("test_addValueListFront_nullptr(): testlist == NULL, Aborting unit tests\n");

	}else {
		addValueListFront(testlist, MISSING_VALUE, TEST_VALUE);

		int result_size = sizeList(testlist);

		printf("test_addValueListFront_nullptr(): asserting result_size == EXPECTED_SIZE when list is empty\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteList(testlist);
	testlist = NULL;
}

/**
 * test_addValueListBack_normal() unit test of intended run through of
 * addValueListBack(), TEST_VALUE gets added behind a value that is in the
 * list but isn't the tail, everything after that value moves back one idx
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListBack_normal (int *assertion_count) {
	// set list size (2-10), need at least 2 links so there is a link that
	// isn't the tail to add behind
	const int SET_LIST_SIZE = rand() % 9 + 2;

	// any idx except the tail (0 to SET_LIST_SIZE - 2), the tail goes
	// through addListBack() and gets its own test
	const int SELECTED_IDX = rand() % (SET_LIST_SIZE - 1);

	// seeded values are 0-999, so 1000-1099 is never already in the list
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	// 10 links max, + 1 for TEST_VALUE
	TYPE expected[11];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addValueListBack_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			expected[expected_size] = value;
			expected_size++;

			// expected gets TEST_VALUE right after the selected value
			if(i == SELECTED_IDX){
				selected_value = value;
				expected[expected_size] = TEST_VALUE;
				expected_size++;
			}
		}

		addValueListBack(result, selected_value, TEST_VALUE);

		int result_size = sizeList(result);

		printf("test_addValueListBack_normal(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addValueListBack_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addValueListBack_tail() unit test where the value to add behind is
 * the tail, TEST_VALUE should become the new tail
 * the tail is checked before the while loop in addValueListBack(), so it
 * always gets checked here, not just when rand() lands on it
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListBack_tail (int *assertion_count) {
	// set list size (1-10), 1 is a list where the tail is also the head
	const int SET_LIST_SIZE = rand() % 10 + 1;
	const int SELECTED_IDX = SET_LIST_SIZE - 1;

	// seeded values are 0-999, so 1000-1099 is never already in the list
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	// 10 links max, + 1 for TEST_VALUE
	TYPE expected[11];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addValueListBack_tail(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			expected[expected_size] = value;
			expected_size++;

			// expected gets TEST_VALUE right after the tail
			if(i == SELECTED_IDX){
				selected_value = value;
				expected[expected_size] = TEST_VALUE;
				expected_size++;
			}
		}

		addValueListBack(result, selected_value, TEST_VALUE);

		int result_size = sizeList(result);

		printf("test_addValueListBack_tail(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addValueListBack_tail(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addValueListBack_not_found() unit test where the value to add behind
 * is not in the list, nothing should get added
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListBack_not_found (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set list size (1-10), never empty, the empty list is in
	// test_addValueListBack_nullptr()
	const int SET_LIST_SIZE = rand() % 10 + 1;

	// seeded values are 0-999, so -100 to -1 is never in the list
	const TYPE MISSING_VALUE = -(rand() % 100 + 1);
	const TYPE TEST_VALUE = rand() % 100 + 1000;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_addValueListBack_not_found(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		addValueListBack(result, MISSING_VALUE, TEST_VALUE);

		// there was nothing to add behind, so TEST_VALUE should not be
		// anywhere in the list
		int result_idx = containsList(result, TEST_VALUE);

		printf("test_addValueListBack_not_found(): asserting containsList(result, TEST_VALUE) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		int result_size = sizeList(result);

		printf("test_addValueListBack_not_found(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			result_idx = containsList(result, expected[i]);

			printf("test_addValueListBack_not_found(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_addValueListBack_nullptr(): this test case runs through the case if
 * the inputted list pointer is NULL, or the list is empty (head == NULL)
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addValueListBack_nullptr (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	// any value, an empty list has nothing in it to add behind
	const TYPE MISSING_VALUE = rand() % 100;
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	// if this returns at all, the NULL check worked
	addValueListBack(NULL, MISSING_VALUE, TEST_VALUE);

	// imagine a list with nothing in it, head and tail are both NULL, so
	// only the head == NULL check can stop it
	LinkedList *testlist = linkedListCreate();

	if(testlist == NULL){
		printf("test_addValueListBack_nullptr(): testlist == NULL, Aborting unit tests\n");

	}else {
		addValueListBack(testlist, MISSING_VALUE, TEST_VALUE);

		int result_size = sizeList(testlist);

		printf("test_addValueListBack_nullptr(): asserting result_size == EXPECTED_SIZE when list is empty\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteList(testlist);
	testlist = NULL;
}

/**
 * test_removeList_normal() unit test of intended run through of removeList(),
 * removes a value that isn't the head or the tail, every link after it
 * should move up one idx
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeList_normal (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set list size (3-10), need at least 3 links so there is a link that
	// isn't the head or the tail
	const int SET_LIST_SIZE = rand() % 8 + 3;

	// any idx except the head and the tail (1 to SET_LIST_SIZE - 2), those
	// get their own tests
	const int SELECTED_IDX = rand() % (SET_LIST_SIZE - 2) + 1;

	TYPE expected[10];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_removeList_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			// expected gets every value except the one being removed
			if(i == SELECTED_IDX){
				selected_value = value;
			}else {
				expected[expected_size] = value;
				expected_size++;
			}
		}

		removeList(result, selected_value);

		int result_size = sizeList(result);

		printf("test_removeList_normal(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		int result_idx = containsList(result, selected_value);

		printf("test_removeList_normal(): asserting containsList(result, selected_value) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			result_idx = containsList(result, expected[i]);

			printf("test_removeList_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_removeList_head() unit test where the value removed is the head, the
 * link after it should become the new head
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeList_head (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set list size (2-10), a list with only 1 link is
	// test_removeList_only_link()
	const int SET_LIST_SIZE = rand() % 9 + 2;
	const int SELECTED_IDX = 0;

	TYPE expected[10];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_removeList_head(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			// expected gets every value except the head
			if(i == SELECTED_IDX){
				selected_value = value;
			}else {
				expected[expected_size] = value;
				expected_size++;
			}
		}

		removeList(result, selected_value);

		int result_size = sizeList(result);

		printf("test_removeList_head(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		int result_idx = containsList(result, selected_value);

		printf("test_removeList_head(): asserting containsList(result, selected_value) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			result_idx = containsList(result, expected[i]);

			printf("test_removeList_head(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_removeList_tail() unit test where the value removed is the tail, the
 * link before it should become the new tail
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeList_tail (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set list size (2-10), a list with only 1 link is
	// test_removeList_only_link()
	const int SET_LIST_SIZE = rand() % 9 + 2;
	const int SELECTED_IDX = SET_LIST_SIZE - 1;

	TYPE expected[10];
	int expected_size = 0;
	TYPE selected_value = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_removeList_tail(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			// expected gets every value except the tail
			if(i == SELECTED_IDX){
				selected_value = value;
			}else {
				expected[expected_size] = value;
				expected_size++;
			}
		}

		removeList(result, selected_value);

		int result_size = sizeList(result);

		printf("test_removeList_tail(): asserting result_size == expected_size\n");
		assert(result_size == expected_size);
		(*assertion_count)++;

		// containsList() walks to the new tail and stops, if the new tail's
		// next was left pointing at the freed link it would keep going
		int result_idx = containsList(result, selected_value);

		printf("test_removeList_tail(): asserting containsList(result, selected_value) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		for(int i = 0; i < expected_size; i++){
			result_idx = containsList(result, expected[i]);

			printf("test_removeList_tail(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_removeList_only_link() unit test where the list only has one link and
 * it gets removed, the list should be empty again and still usable
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeList_only_link (int *assertion_count) {
	const int EXPECTED_SIZE = 0;
	const int EXPECTED_RESULT = -1;
	const TYPE SELECTED_VALUE = rand() % 100;
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_removeList_only_link(): result == NULL, Aborting unit tests\n");

	}else {
		addListBack(result, SELECTED_VALUE);

		removeList(result, SELECTED_VALUE);

		int result_size = sizeList(result);

		printf("test_removeList_only_link(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;

		int result_idx = containsList(result, SELECTED_VALUE);

		printf("test_removeList_only_link(): asserting containsList(result, SELECTED_VALUE) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		// head and tail can't be looked at from here, but if either one
		// was left pointing at the freed link, adding to the list again is
		// where it would break
		addListBack(result, TEST_VALUE);

		result_size = sizeList(result);

		printf("test_removeList_only_link(): asserting result_size == 1 after adding TEST_VALUE\n");
		assert(result_size == 1);
		(*assertion_count)++;

		result_idx = containsList(result, TEST_VALUE);

		printf("test_removeList_only_link(): asserting containsList(result, TEST_VALUE) == 0\n");
		assert(result_idx == 0);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_removeList_not_found() unit test where the value to remove is not in
 * the list, nothing should get removed
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeList_not_found (int *assertion_count) {
	// set list size (1-10), never empty, the empty list is in
	// test_removeList_nullptr()
	const int SET_LIST_SIZE = rand() % 10 + 1;

	// seeded values are 0-999, so -100 to -1 is never in the list
	const TYPE TEST_VALUE = -(rand() % 100 + 1);
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_removeList_not_found(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		removeList(result, TEST_VALUE);

		int result_size = sizeList(result);

		printf("test_removeList_not_found(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_removeList_not_found(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_removeList_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, or the list is empty (head == NULL)
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeList_nullptr (int *assertion_count) {
	const int EXPECTED_SIZE = 0;
	const TYPE TEST_VALUE = rand() % 100;

	// if this returns at all, the NULL check worked
	removeList(NULL, TEST_VALUE);

	// imagine a list with nothing in it, head and tail are both NULL, so
	// only the empty list check can stop it
	LinkedList *testlist = linkedListCreate();

	if(testlist == NULL){
		printf("test_removeList_nullptr(): testlist == NULL, Aborting unit tests\n");

	}else {
		removeList(testlist, TEST_VALUE);

		// size can't go below 0
		int result_size = sizeList(testlist);

		printf("test_removeList_nullptr(): asserting result_size == EXPECTED_SIZE when list is empty\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteList(testlist);
	testlist = NULL;
}

/**
 * test_printList_normal() unit test of intended run through of printList()
 * on a list with more than one link, what gets printed can only be checked
 * by eye, so the expected output gets printed right above it, then the list
 * gets checked to make sure printing it didn't change anything
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_printList_normal (int *assertion_count) {
	// set list size (2-10), 1 link and 0 links print differently and get
	// their own tests
	const int SET_LIST_SIZE = rand() % 9 + 2;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_printList_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		printf("test_printList_normal(): expected output is [ ");
		for(int i = 0; i < SET_LIST_SIZE; i++){
			if(i == 0)
				printf("%d", expected[i]);
			else
				printf(", %d", expected[i]);
		}
		printf(" ]\n");

		printList(result);

		// printList() only reads, so nothing should change
		int result_size = sizeList(result);

		printf("test_printList_normal(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_printList_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_printList_one_link() unit test of printList() on a list with only
 * one link, where the head is also the tail
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_printList_one_link (int *assertion_count) {
	const int EXPECTED_SIZE = 1;
	const TYPE TEST_VALUE = rand() % 100;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_printList_one_link(): result == NULL, Aborting unit tests\n");

	}else {
		addListBack(result, TEST_VALUE);

		printf("test_printList_one_link(): expected output is [ %d ]\n", TEST_VALUE);

		printList(result);

		// printList() only reads, so nothing should change
		int result_size = sizeList(result);

		printf("test_printList_one_link(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;

		int result_idx = containsList(result, TEST_VALUE);

		printf("test_printList_one_link(): asserting containsList(result, TEST_VALUE) == 0\n");
		assert(result_idx == 0);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_printList_empty() unit test of printList() on a list with nothing in
 * it, head and tail are both NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_printList_empty (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_printList_empty(): result == NULL, Aborting unit tests\n");

	}else {
		printf("test_printList_empty(): expected output is [  ]\n");

		printList(result);

		// printList() only reads, so nothing should change
		int result_size = sizeList(result);

		printf("test_printList_empty(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_printList_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, there is nothing to assert, it just can't
 * crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_printList_nullptr (int *assertion_count) {
	printf("test_printList_nullptr(): expected output is List is NULL\n");

	// if this returns at all, the NULL check worked
	printList(NULL);
}

/**
 * test_reverseList_normal() unit test of intended run through of
 * reverseList(), the list should end up in the opposite order
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_reverseList_normal (int *assertion_count) {
	// set list size (2-10), 1 link and 0 links have nothing to reverse and
	// get their own tests
	const int SET_LIST_SIZE = rand() % 9 + 2;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_reverseList_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addListBack(result, value);

			// the first value added should end up last
			expected[SET_LIST_SIZE - 1 - i] = value;
		}

		reverseList(result);

		int result_size = sizeList(result);

		printf("test_reverseList_normal(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_reverseList_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_reverseList_twice() unit test where the list gets reversed twice, it
 * should come back in its original order
 * the second reverse has to walk the links the first one rewired, so it
 * catches a next or previous pointer the first reverse left wrong
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_reverseList_twice (int *assertion_count) {
	// set list size (2-10)
	const int SET_LIST_SIZE = rand() % 9 + 2;
	TYPE expected[10];

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_reverseList_twice(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_LIST_SIZE; i++){
			expected[i] = rand() % 100 + i * 100;
			addListBack(result, expected[i]);
		}

		reverseList(result);
		reverseList(result);

		int result_size = sizeList(result);

		printf("test_reverseList_twice(): asserting result_size == SET_LIST_SIZE\n");
		assert(result_size == SET_LIST_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_LIST_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_reverseList_twice(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_reverseList_one_link() unit test of reverseList() on a list with only
 * one link, a list of one reversed is the same list
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_reverseList_one_link (int *assertion_count) {
	const int EXPECTED_SIZE = 1;
	const TYPE TEST_VALUE = rand() % 100;

	LinkedList *result = linkedListCreate();

	if(result == NULL){
		printf("test_reverseList_one_link(): result == NULL, Aborting unit tests\n");

	}else {
		addListBack(result, TEST_VALUE);

		reverseList(result);

		int result_size = sizeList(result);

		printf("test_reverseList_one_link(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;

		int result_idx = containsList(result, TEST_VALUE);

		printf("test_reverseList_one_link(): asserting containsList(result, TEST_VALUE) == 0\n");
		assert(result_idx == 0);
		(*assertion_count)++;
	}

	deleteList(result);
	result = NULL;
}

/**
 * test_reverseList_nullptr(): this test case runs through the case if the
 * inputted list pointer is NULL, or the list is empty (head == NULL)
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_reverseList_nullptr (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	// if this returns at all, the NULL check worked
	reverseList(NULL);

	// imagine a list with nothing in it, head and tail are both NULL, so
	// only the head == NULL check can stop it
	LinkedList *testlist = linkedListCreate();

	if(testlist == NULL){
		printf("test_reverseList_nullptr(): testlist == NULL, Aborting unit tests\n");

	}else {
		reverseList(testlist);

		int result_size = sizeList(testlist);

		printf("test_reverseList_nullptr(): asserting result_size == EXPECTED_SIZE when list is empty\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteList(testlist);
	testlist = NULL;
}

/* ************************************************************************
	Deque Tests
************************************************************************ */

/**
 * test_dequeCreate_normal() unit test of dequeCreate(), a new deque should
 * come back with nothing in it
 * remember caller of dequeCreate() has to check if the deque != NULL
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_dequeCreate_normal (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	Deque *result = dequeCreate();

	printf("test_dequeCreate_normal(): asserting result != NULL\n");
	assert(result != NULL);
	(*assertion_count)++;

	int result_size = sizeDeque(result);

	printf("test_dequeCreate_normal(): asserting result_size == EXPECTED_SIZE\n");
	assert(result_size == EXPECTED_SIZE);
	(*assertion_count)++;

	deleteDeque(result);
	result = NULL;
}

/**
 * test_deleteDeque_normal() unit test of deleteDeque() on a deque that has
 * links in it, the deque is freed after deleteDeque() so there is nothing
 * left to assert on, run under leaks or valgrind to check every link got
 * freed
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_deleteDeque_normal (int *assertion_count) {
	// set deque size (0-10), 0 is an empty deque
	const int SET_DEQUE_SIZE = rand() % 11;

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_deleteDeque_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			addFrontDeque(result, rand() % 100 + i * 100);
		}

		// make sure there really are SET_DEQUE_SIZE links for deleteDeque() to free
		int result_size = sizeDeque(result);

		printf("test_deleteDeque_normal(): asserting result_size == SET_DEQUE_SIZE\n");
		assert(result_size == SET_DEQUE_SIZE);
		(*assertion_count)++;

		// if this returns at all, every link got freed without walking off
		// the end of the deque
		deleteDeque(result);
	}

	result = NULL;
}

/**
 * test_deleteDeque_nullptr() unit test of deleteDeque() with NULL input,
 * deleteDeque(NULL) has nothing to assert, it just can't crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_deleteDeque_nullptr (int *assertion_count) {
	// if this returns at all, the NULL check worked
	deleteDeque(NULL);
}

/**
 * test_sizeDeque_normal() unit test of intended run through of sizeDeque()
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_sizeDeque_normal (int *assertion_count) {
	// set deque size (1-10)
	const int SET_DEQUE_SIZE = rand() % 10 + 1;

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_sizeDeque_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			addFrontDeque(result, rand() % 100 + i * 100);
		}

		int result_size = sizeDeque(result);

		printf("test_sizeDeque_normal(): asserting result_size == SET_DEQUE_SIZE\n");
		assert(result_size == SET_DEQUE_SIZE);
		(*assertion_count)++;
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_sizeDeque_empty() second unit test of sizeDeque(), an empty deque
 * should have a size of 0
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_sizeDeque_empty (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_sizeDeque_empty(): result == NULL, Aborting unit tests\n");

	}else {
		int result_size = sizeDeque(result);

		printf("test_sizeDeque_empty(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_sizeDeque_nullptr(): this test case runs through the case if the
 * inputted deque pointer is NULL, sizeDeque() should return -1
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_sizeDeque_nullptr (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	int result = sizeDeque(NULL);

	printf("test_sizeDeque_nullptr(): asserting result == EXPECTED_RESULT when deque == NULL\n");
	assert(result == EXPECTED_RESULT);
	(*assertion_count)++;
}

/**
 * test_addFrontDeque_normal() unit test of intended run through of
 * addFrontDeque(), every value added goes in front of the one added before
 * it, so the deque ends up in the opposite order the values were added in
 * Deque is the same struct as LinkedList (see linkedList.h), so
 * containsList() works on it to check the order
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_addFrontDeque_normal (int *assertion_count) {
	// set deque size (1-10), the first value always goes into an empty deque
	const int SET_DEQUE_SIZE = rand() % 10 + 1;
	TYPE expected[10];

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_addFrontDeque_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addFrontDeque(result, value);

			// the first value added ends up at the back
			expected[SET_DEQUE_SIZE - 1 - i] = value;
		}

		int result_size = sizeDeque(result);

		printf("test_addFrontDeque_normal(): asserting result_size == SET_DEQUE_SIZE\n");
		assert(result_size == SET_DEQUE_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_addFrontDeque_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_addFrontDeque_nullptr(): this test case runs through the case if the
 * inputted deque pointer is NULL, there is nothing to assert, it just can't
 * crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_addFrontDeque_nullptr (int *assertion_count) {
	const TYPE TEST_VALUE = rand() % 100;

	// if this returns at all, the NULL check worked
	addFrontDeque(NULL, TEST_VALUE);
}

/**
 * test_backDeque_normal() unit test of intended run through of backDeque(),
 * the value at the back should be the first value that was added to the
 * front
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_backDeque_normal (int *assertion_count) {
	// set deque size (1-10), 1 is a deque where the back is also the front
	const int SET_DEQUE_SIZE = rand() % 10 + 1;
	TYPE expected_back = 0;

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_backDeque_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addFrontDeque(result, value);

			// every value after the first goes in front of it, so the
			// first value added stays at the back
			if(i == 0)
				expected_back = value;
		}

		TYPE result_back = backDeque(result);

		printf("test_backDeque_normal(): asserting result_back == expected_back\n");
		assert(result_back == expected_back);
		(*assertion_count)++;

		// backDeque() only reads, so nothing should be removed
		int result_size = sizeDeque(result);

		printf("test_backDeque_normal(): asserting result_size == SET_DEQUE_SIZE\n");
		assert(result_size == SET_DEQUE_SIZE);
		(*assertion_count)++;
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_backDeque_nullptr(): this test case runs through the case if the
 * inputted deque pointer is NULL, or the deque is empty (tail == NULL),
 * either way there is no back link to read
 * backDeque() returns a TYPE and no TYPE value is set aside to mean
 * "failed", so all this checks is that backDeque() comes back instead of
 * crashing
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_backDeque_nullptr (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	// if this returns at all, the NULL check worked
	backDeque(NULL);

	// imagine a deque with nothing in it, head and tail are both NULL, so
	// only the empty deque check can stop it
	Deque *testdeque = dequeCreate();

	if(testdeque == NULL){
		printf("test_backDeque_nullptr(): testdeque == NULL, Aborting unit tests\n");

	}else {
		// if this returns at all, the empty deque check worked
		backDeque(testdeque);

		// backDeque() only reads, so the deque should still be empty
		int result_size = sizeDeque(testdeque);

		printf("test_backDeque_nullptr(): asserting result_size == EXPECTED_SIZE when deque is empty\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteDeque(testdeque);
	testdeque = NULL;
}

/**
 * test_removeBackDeque_normal() unit test of intended run through of
 * removeBackDeque(), the back link gets removed and the link that was in
 * front of it becomes the new back
 * Deque is the same struct as LinkedList (see linkedList.h), so
 * containsList() works on it to check the order
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeBackDeque_normal (int *assertion_count) {
	const int EXPECTED_RESULT = -1;

	// set deque size (2-10), a deque with only 1 link is
	// test_removeBackDeque_one_link()
	const int SET_DEQUE_SIZE = rand() % 9 + 2;

	// the back is the last idx in expected, so removing it is just one
	// less expected link
	const int EXPECTED_SIZE = SET_DEQUE_SIZE - 1;
	TYPE expected[10];

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_removeBackDeque_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addFrontDeque(result, value);

			// the first value added ends up at the back
			expected[SET_DEQUE_SIZE - 1 - i] = value;
		}

		const TYPE REMOVED_VALUE = expected[SET_DEQUE_SIZE - 1];

		removeBackDeque(result);

		int result_size = sizeDeque(result);

		printf("test_removeBackDeque_normal(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;

		int result_idx = containsList(result, REMOVED_VALUE);

		printf("test_removeBackDeque_normal(): asserting containsList(result, REMOVED_VALUE) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		// the link that was second to last is the new back
		TYPE result_back = backDeque(result);

		printf("test_removeBackDeque_normal(): asserting result_back == expected[EXPECTED_SIZE - 1]\n");
		assert(result_back == expected[EXPECTED_SIZE - 1]);
		(*assertion_count)++;

		for(int i = 0; i < EXPECTED_SIZE; i++){
			result_idx = containsList(result, expected[i]);

			printf("test_removeBackDeque_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_removeBackDeque_one_link() unit test where the deque only has one link
 * and it gets removed, the deque should be empty again and still usable
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeBackDeque_one_link (int *assertion_count) {
	const int EXPECTED_SIZE = 0;
	const int EXPECTED_RESULT = -1;
	const TYPE SELECTED_VALUE = rand() % 100;
	const TYPE TEST_VALUE = rand() % 100 + 1000;

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_removeBackDeque_one_link(): result == NULL, Aborting unit tests\n");

	}else {
		addFrontDeque(result, SELECTED_VALUE);

		removeBackDeque(result);

		int result_size = sizeDeque(result);

		printf("test_removeBackDeque_one_link(): asserting result_size == EXPECTED_SIZE\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;

		int result_idx = containsList(result, SELECTED_VALUE);

		printf("test_removeBackDeque_one_link(): asserting containsList(result, SELECTED_VALUE) == EXPECTED_RESULT\n");
		assert(result_idx == EXPECTED_RESULT);
		(*assertion_count)++;

		// head and tail can't be looked at from here, but if either one
		// was left pointing at the freed link, adding to the deque again
		// is where it would break
		addFrontDeque(result, TEST_VALUE);

		result_size = sizeDeque(result);

		printf("test_removeBackDeque_one_link(): asserting result_size == 1 after adding TEST_VALUE\n");
		assert(result_size == 1);
		(*assertion_count)++;

		TYPE result_back = backDeque(result);

		printf("test_removeBackDeque_one_link(): asserting result_back == TEST_VALUE\n");
		assert(result_back == TEST_VALUE);
		(*assertion_count)++;
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_removeBackDeque_nullptr(): this test case runs through the case if
 * the inputted deque pointer is NULL, or the deque is empty (tail == NULL),
 * either way there is no back link to remove
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_removeBackDeque_nullptr (int *assertion_count) {
	const int EXPECTED_SIZE = 0;

	// if this returns at all, the NULL check worked
	removeBackDeque(NULL);

	// imagine a deque with nothing in it, head and tail are both NULL, so
	// only the empty deque check can stop it
	Deque *testdeque = dequeCreate();

	if(testdeque == NULL){
		printf("test_removeBackDeque_nullptr(): testdeque == NULL, Aborting unit tests\n");

	}else {
		removeBackDeque(testdeque);

		// size can't go below 0
		int result_size = sizeDeque(testdeque);

		printf("test_removeBackDeque_nullptr(): asserting result_size == EXPECTED_SIZE when deque is empty\n");
		assert(result_size == EXPECTED_SIZE);
		(*assertion_count)++;
	}

	deleteDeque(testdeque);
	testdeque = NULL;
}

/**
 * test_printDeque_normal() unit test of intended run through of
 * printDeque(), what gets printed can only be checked by eye, so the
 * expected output gets printed right above it, then the deque gets checked
 * to make sure printing it didn't change anything
 * @param: int *assertion_count, takes address of a counter to increment
 * @return: void
 */
void test_printDeque_normal (int *assertion_count) {
	// set deque size (1-10)
	const int SET_DEQUE_SIZE = rand() % 10 + 1;
	TYPE expected[10];

	Deque *result = dequeCreate();

	if(result == NULL){
		printf("test_printDeque_normal(): result == NULL, Aborting unit tests\n");

	}else {
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			TYPE value = rand() % 100 + i * 100;
			addFrontDeque(result, value);

			// the first value added ends up at the back
			expected[SET_DEQUE_SIZE - 1 - i] = value;
		}

		printf("test_printDeque_normal(): expected output is [ ");
		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			if(i == 0)
				printf("%d", expected[i]);
			else
				printf(", %d", expected[i]);
		}
		printf(" ]\n");

		printDeque(result);

		// printDeque() only reads, so nothing should change
		int result_size = sizeDeque(result);

		printf("test_printDeque_normal(): asserting result_size == SET_DEQUE_SIZE\n");
		assert(result_size == SET_DEQUE_SIZE);
		(*assertion_count)++;

		for(int i = 0; i < SET_DEQUE_SIZE; i++){
			int result_idx = containsList(result, expected[i]);

			printf("test_printDeque_normal(): asserting containsList(result, expected[%d]) == %d\n", i, i);
			assert(result_idx == i);
			(*assertion_count)++;
		}
	}

	deleteDeque(result);
	result = NULL;
}

/**
 * test_printDeque_nullptr(): this test case runs through the case if the
 * inputted deque pointer is NULL, there is nothing to assert, it just can't
 * crash
 * @param: int *assertion_count, not used since there is nothing to assert,
 * every test takes it so main() can call them all the same way
 * @return: void
 */
void test_printDeque_nullptr (int *assertion_count) {
	printf("test_printDeque_nullptr(): expected output is List is NULL\n");

	// if this returns at all, the NULL check worked
	printDeque(NULL);
}
