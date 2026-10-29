/**
 * File: w9_a2/test_main.c
 * date: 2026-10-27
 *
 * assert() based tests for the Boat functions and the generic functions in
 * main.c. main.c is included directly (with its main() renamed) so these
 * tests can call its functions without changing that file.
 *
 * build: cc -Wall -Wextra -o test_main test_main.c
 * run:   ./test_main
 *
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// rename main.c's main() so this file can provide its own
#define main boat_main
#include "main.c"
#undef main

#define ARR_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define OUT_BUF_SIZE 256

#define RUN_TEST(test) \
	do { \
		printf("running %s\n", #test); \
		fflush(stdout); \
		test(); \
	} while (0)


/* ---------------------------- seed data ---------------------------- */

static const Boat SEED_BOATS[] = {
	{"Sea Ray",       24, 350},
	{"Boston Whaler", 17, 115},
	{"Bayliner",      19, 250},
	{"Grady-White",   28, 600},
	{"Kayak",         12,   0},
};

// boats that are not in SEED_BOATS, but share something with Sea Ray
static const Boat NEAR_MISSES[] = {
	{"Same length",  24, 999},
	{"Same engine",  99, 350},
	{"Swapped",     350,  24},
	{"Zeroed",        0,   0},
};

static const int SEED_INTS[] = {4, 8, 15, 16, 23, 42};


/* ----------------------------- helpers ----------------------------- */

/**
 * int_eq() compares two ints, used to run containsArr() on a non-Boat array
 *
 * @param: const void *val1, int 1
 * @param: const void *val2, int 2
 * @return: 1 = true, 0 = false
 */
static int int_eq(const void *val1, const void *val2){
	return *(const int *)val1 == *(const int *)val2;
}

/**
 * print_boats_with_printArr() calls printArr() with boat_repr() so it can be
 * passed to capture_output()
 *
 * @param: const void *arr, the boats to print
 */
static void print_boats_with_printArr(const void *arr){
	printArr(arr, boat_repr);
}

/**
 * capture_output() runs a print function and collects what it wrote to stdout
 *
 * @param: void (*print)(const void *), the function that prints
 * @param: const void *arg, passed through to print
 * @param: char *buf, where the captured text is stored, null terminated
 * @param: size_t bufSize, size of buf in bytes
 */
static void capture_output(
		void (*print)(const void *),
		const void *arg,
		char *buf,
		size_t bufSize
		){

	fflush(stdout);

	FILE *tmp = tmpfile();
	assert(tmp != NULL);

	int savedStdout = dup(STDOUT_FILENO);
	assert(savedStdout != -1);

	int redirected = dup2(fileno(tmp), STDOUT_FILENO);
	assert(redirected != -1);

	print(arg);
	fflush(stdout);

	int restored = dup2(savedStdout, STDOUT_FILENO);
	assert(restored != -1);
	close(savedStdout);

	rewind(tmp);
	size_t len = fread(buf, 1, bufSize - 1, tmp);
	buf[len] = '\0';
	fclose(tmp);
}


/* ----------------------------- boat_eq ----------------------------- */

/** every seed boat equals itself */
static void test_boat_eq_same_boat(void){
	for (size_t i = 0; i < ARR_LEN(SEED_BOATS); i++){
		assert(boat_eq(&SEED_BOATS[i], &SEED_BOATS[i]) == 1);
	}
}

/** a separate Boat with the same values is equal, in both orders */
static void test_boat_eq_matching_copy(void){
	Boat copy = {"Sea Ray", 24, 350};

	assert(boat_eq(&copy, &SEED_BOATS[0]) == 1);
	assert(boat_eq(&SEED_BOATS[0], &copy) == 1);
}

/** no two different seed boats are equal */
static void test_boat_eq_different_boats(void){
	for (size_t i = 0; i < ARR_LEN(SEED_BOATS); i++){
		for (size_t j = 0; j < ARR_LEN(SEED_BOATS); j++){
			if (i != j){
				assert(boat_eq(&SEED_BOATS[i], &SEED_BOATS[j]) == 0);
			}
		}
	}
}

/** boats sharing only some values with Sea Ray are not equal, in both orders */
static void test_boat_eq_near_misses(void){
	for (size_t i = 0; i < ARR_LEN(NEAR_MISSES); i++){
		assert(boat_eq(&NEAR_MISSES[i], &SEED_BOATS[0]) == 0);
		assert(boat_eq(&SEED_BOATS[0], &NEAR_MISSES[i]) == 0);
	}
}


/* ---------------------------- boat_repr ---------------------------- */

/** one boat prints exactly the expected line */
static void test_boat_repr_single(void){
	char out[OUT_BUF_SIZE];

	capture_output(boat_repr, &SEED_BOATS[0], out, sizeof(out));
	assert(strcmp(out, "boat: Sea Ray, length: 24, Engine size: 350\n") == 0);
}

/** every seed boat prints its own name, length, and engine size */
static void test_boat_repr_all_seeds(void){
	char out[OUT_BUF_SIZE];
	char expected[OUT_BUF_SIZE];

	for (size_t i = 0; i < ARR_LEN(SEED_BOATS); i++){
		const Boat *boat = &SEED_BOATS[i];

		snprintf(
			expected, sizeof(expected),
			"boat: %s, length: %d, Engine size: %d\n",
			boat->name, boat->length, boat->engineSize
			);

		capture_output(boat_repr, boat, out, sizeof(out));
		assert(strcmp(out, expected) == 0);
	}
}

/** a boat whose length and engine size differ shows both values */
static void test_boat_repr_near_miss(void){
	char out[OUT_BUF_SIZE];

	capture_output(boat_repr, &NEAR_MISSES[0], out, sizeof(out));
	assert(strcmp(out, "boat: Same length, length: 24, Engine size: 999\n") == 0);
}


/* --------------------------- containsArr --------------------------- */

/** finds boats at the first, middle, and last index */
static void test_containsArr_first_middle_last(void){
	Boat first = {"Sea Ray", 24, 350};
	Boat middle = {"Bayliner", 19, 250};
	Boat last = {"Kayak", 12, 0};
	size_t n = ARR_LEN(SEED_BOATS);

	assert(containsArr(SEED_BOATS, n, sizeof(Boat), &first, boat_eq) == 1);
	assert(containsArr(SEED_BOATS, n, sizeof(Boat), &middle, boat_eq) == 1);
	assert(containsArr(SEED_BOATS, n, sizeof(Boat), &last, boat_eq) == 1);
}

/** finds every seed boat */
static void test_containsArr_every_seed(void){
	size_t n = ARR_LEN(SEED_BOATS);

	for (size_t i = 0; i < n; i++){
		assert(containsArr(SEED_BOATS, n, sizeof(Boat), &SEED_BOATS[i], boat_eq) == 1);
	}
}

/** does not find any of the near misses */
static void test_containsArr_missing(void){
	size_t n = ARR_LEN(SEED_BOATS);

	for (size_t i = 0; i < ARR_LEN(NEAR_MISSES); i++){
		assert(containsArr(SEED_BOATS, n, sizeof(Boat), &NEAR_MISSES[i], boat_eq) == 0);
	}
}

/** an empty array contains nothing */
static void test_containsArr_empty(void){
	assert(containsArr(SEED_BOATS, 0, sizeof(Boat), &SEED_BOATS[0], boat_eq) == 0);
}

/** only searches the first `size` elements */
static void test_containsArr_respects_size(void){
	size_t n = ARR_LEN(SEED_BOATS);

	assert(containsArr(SEED_BOATS, 1, sizeof(Boat), &SEED_BOATS[0], boat_eq) == 1);
	assert(containsArr(SEED_BOATS, 1, sizeof(Boat), &SEED_BOATS[1], boat_eq) == 0);
	assert(containsArr(SEED_BOATS, n - 1, sizeof(Boat), &SEED_BOATS[n - 1], boat_eq) == 0);
}

/** works on an int array with a different compare function */
static void test_containsArr_ints(void){
	size_t n = ARR_LEN(SEED_INTS);
	int first = 4;
	int last = 42;
	int middle = 16;
	int missing = 7;
	int negative = -4;

	assert(containsArr(SEED_INTS, n, sizeof(int), &first, int_eq) == 1);
	assert(containsArr(SEED_INTS, n, sizeof(int), &middle, int_eq) == 1);
	assert(containsArr(SEED_INTS, n, sizeof(int), &last, int_eq) == 1);
	assert(containsArr(SEED_INTS, n, sizeof(int), &missing, int_eq) == 0);
	assert(containsArr(SEED_INTS, n, sizeof(int), &negative, int_eq) == 0);
}


/* ----------------------------- printArr ---------------------------- */

/** printArr() with boat_repr() prints the same thing boat_repr() does */
static void test_printArr_single_boat(void){
	char viaPrintArr[OUT_BUF_SIZE];
	char viaRepr[OUT_BUF_SIZE];

	capture_output(print_boats_with_printArr, &SEED_BOATS[0], viaPrintArr, sizeof(viaPrintArr));
	capture_output(boat_repr, &SEED_BOATS[0], viaRepr, sizeof(viaRepr));

	assert(strcmp(viaPrintArr, "boat: Sea Ray, length: 24, Engine size: 350\n") == 0);
	assert(strcmp(viaPrintArr, viaRepr) == 0);
}


/**
 * main(), runs every test, an assert() aborts on the first one that fails
 *
 * @return: int, 0 when every test passes
 */
int main(void){
	RUN_TEST(test_boat_eq_same_boat);
	RUN_TEST(test_boat_eq_matching_copy);
	RUN_TEST(test_boat_eq_different_boats);
	RUN_TEST(test_boat_eq_near_misses);

	RUN_TEST(test_boat_repr_single);
	RUN_TEST(test_boat_repr_all_seeds);
	RUN_TEST(test_boat_repr_near_miss);

	RUN_TEST(test_containsArr_first_middle_last);
	RUN_TEST(test_containsArr_every_seed);
	RUN_TEST(test_containsArr_missing);
	RUN_TEST(test_containsArr_empty);
	RUN_TEST(test_containsArr_respects_size);
	RUN_TEST(test_containsArr_ints);

	RUN_TEST(test_printArr_single_boat);

	printf("all tests passed\n");
	return 0;
}
