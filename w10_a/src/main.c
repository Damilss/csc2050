/**
 * file: main.c
 * author: Emiio L <escott15@calpoly.edu>
 * date: 2026-11-01
 *
 * entry point of the program for week 10 activity
 *
 * assert() based tests for the inode functions in fileSystem.c. the block
 * files get made in the folder the program is run from, and freeInode()
 * deletes them at the end of each test
 *
 * build: cc -Wall -Wextra -o out main.c fileSystem.c
 * run:   ./out
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileSystem.h"

#define POINTERS_PER_BLOCK ((uint64_t)BLOCK_SIZE / POINTER_SIZE)

/* the first byte of the file reached by each kind of block pointer */
#define SINGLE_START ((uint64_t)DIRECT_BLOCKS * BLOCK_SIZE)

/* get the double start of an inode */
#define DOUBLE_START (SINGLE_START + POINTERS_PER_BLOCK * BLOCK_SIZE)

/* get the triple start of a specific inode */
#define TRIPLE_START (DOUBLE_START + POINTERS_PER_BLOCK * POINTERS_PER_BLOCK * BLOCK_SIZE)

/* max file size of an inode */
#define MAX_FILE_SIZE (TRIPLE_START \
		+ POINTERS_PER_BLOCK * POINTERS_PER_BLOCK * POINTERS_PER_BLOCK * BLOCK_SIZE)
/* having the do while loops has the preprocessor treat the code as one
 * statement rather than multiple statements */
#define RUN_TEST(test) \
	do { \
		printf("running %s\n", #test); \
		fflush(stdout); \
		test(); \
	} while (0)


/* ----------------------------- helpers ----------------------------- */

/**
 * blockExists() checks if the file for a block number is on disk
 *
 * @param: uint64_t block, the block number
 * @return: 1 = true, 0 = false
 */
static int blockExists(uint64_t block){
	char path[BLOCK_PATH_SIZE];
	snprintf(path, sizeof(path), BLOCK_FILE_FORMAT, block);

	FILE *file = fopen(path, "rb");
	if (file == NULL)
		return 0;

	fclose(file);
	return 1;
}

/**
 * blockFileSize() gets how many bytes are in a block file
 *
 * @param: uint64_t block, the block number
 * @return: long, the size of the file in bytes, -1 if it couldn't be opened
 */
static long blockFileSize(uint64_t block){
	char path[BLOCK_PATH_SIZE];
	snprintf(path, sizeof(path), BLOCK_FILE_FORMAT, block);

	FILE *file = fopen(path, "rb");
	if (file == NULL)
		return -1;

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fclose(file);
	return size;
}

/**
 * fillPattern() fills a buffer with a-z over and over, so every block of a
 * big write has different data in it
 *
 * @param: char *buffer, the buffer to fill
 * @param: size_t length, number of bytes to fill
 */
static void fillPattern(char *buffer, size_t length){
	for (size_t i = 0; i < length; i++)
		buffer[i] = (char)('a' + i % 26);
}

/**
 * checkRoundTrip() writes text into node at offset, then reads it back and
 * checks it matches and the size grew to cover it
 *
 * @param: struct inode *node, the file i-node to use
 * @param: uint64_t offset, where in the file to write
 * @param: const char *text, the string to write (without the '\0')
 */
static void checkRoundTrip(struct inode *node, uint64_t offset, const char *text){
	uint64_t length = strlen(text);
	char buffer[64] = {0};

	int64_t written = writeFile(node, offset, text, length);
	assert(written == (int64_t)length);
	assert(node->size == offset + length);

	int64_t read = readFile(node, offset, buffer, length);
	assert(read == (int64_t)length);
	assert(memcmp(buffer, text, length) == 0);
}


/* ------------------------- mkFile / mkDir -------------------------- */

/** a new file i-node is regular, empty, and has the given owner and perms */
static void test_mkFile_fields(void){
	struct inode *node = mkFile(0644, 1000, 100);
	assert(node != NULL);

	assert(INODE_IS_REGULAR(node->mode));
	assert(!INODE_IS_DIRECTORY(node->mode));
	assert((node->mode & INODE_PERMISSION_MASK) == 0644);
	assert(node->owner_uid == 1000);
	assert(node->owner_gid == 100);
	assert(node->size == 0);
	assert(node->link_count == 1);
	assert(node->block_size == BLOCK_SIZE);
	assert(node->block_offset == BLOCK_OFFSET);

	assert(node->access_time.tv_sec != 0);
	assert(node->modification_time.tv_sec != 0);
	assert(node->change_time.tv_sec != 0);

	for (int i = 0; i < TOTAL_BLOCKS; i++)
		assert(node->data_blocks[i] == 0);

	freeInode(node);
}

/** a new directory i-node is a directory with a link count of 2 */
static void test_mkDir_fields(void){
	struct inode *node = mkDir(0755, 501, 20);
	assert(node != NULL);

	assert(INODE_IS_DIRECTORY(node->mode));
	assert(!INODE_IS_REGULAR(node->mode));
	assert((node->mode & INODE_PERMISSION_MASK) == 0755);
	assert(node->owner_uid == 501);
	assert(node->owner_gid == 20);
	assert(node->size == 0);
	assert(node->link_count == 2);

	for (int i = 0; i < TOTAL_BLOCKS; i++)
		assert(node->data_blocks[i] == 0);

	freeInode(node);
}

/** extra bits passed in as permissions can't change the file type */
static void test_permissions_are_masked(void){
	struct inode *node = mkFile(INODE_DIRECTORY | 07777, 0, 0);
	assert(node != NULL);

	assert(INODE_IS_REGULAR(node->mode));
	assert((node->mode & INODE_PERMISSION_MASK) == 0777);

	freeInode(node);
}

/** every i-node gets its own number, files and directories alike */
static void test_inode_numbers_unique(void){
	struct inode *nodes[] = {
		mkFile(0644, 0, 0),
		mkDir(0755, 0, 0),
		mkFile(0600, 0, 0),
		mkDir(0700, 0, 0),
	};
	size_t count = sizeof(nodes) / sizeof(nodes[0]);

	for (size_t i = 0; i < count; i++){
		assert(nodes[i] != NULL);
		for (size_t j = i + 1; j < count; j++)
			assert(nodes[i]->inode_number != nodes[j]->inode_number);
	}

	for (size_t i = 0; i < count; i++)
		freeInode(nodes[i]);
}


/* ----------------------- writeFile / readFile ---------------------- */

/** a small write uses one direct block, and reads back the same */
static void test_write_read_small(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	checkRoundTrip(node, 0, "hello, inode");

	assert(node->data_blocks[0] != 0);
	assert(node->data_blocks[1] == 0);
	assert(blockExists(node->data_blocks[0]));
	assert(blockFileSize(node->data_blocks[0]) == BLOCK_OFFSET + BLOCK_SIZE);

	freeInode(node);
}

/** a write bigger than one block spreads over the direct blocks */
static void test_write_across_blocks(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char data[3000];
	char buffer[3000];
	fillPattern(data, sizeof(data));

	int64_t written = writeFile(node, 0, data, sizeof(data));
	assert(written == (int64_t)sizeof(data));
	assert(node->size == sizeof(data));

	// 3000 bytes needs blocks 0, 1 and 2
	assert(node->data_blocks[0] != 0);
	assert(node->data_blocks[1] != 0);
	assert(node->data_blocks[2] != 0);
	assert(node->data_blocks[3] == 0);

	int64_t read = readFile(node, 0, buffer, sizeof(buffer));
	assert(read == (int64_t)sizeof(buffer));
	assert(memcmp(buffer, data, sizeof(data)) == 0);

	// a read that starts in block 0 and ends in block 1
	read = readFile(node, 1000, buffer, 100);
	assert(read == 100);
	assert(memcmp(buffer, data + 1000, 100) == 0);

	freeInode(node);
}

/** writing over the middle of a file only changes those bytes */
static void test_overwrite_middle(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char buffer[16] = {0};

	assert(writeFile(node, 0, "hello world", 11) == 11);
	assert(writeFile(node, 6, "WORLD", 5) == 5);
	assert(node->size == 11);

	assert(readFile(node, 0, buffer, 11) == 11);
	assert(memcmp(buffer, "hello WORLD", 11) == 0);

	freeInode(node);
}

/** writing right at the end of the file adds on to it */
static void test_write_appends(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char buffer[16] = {0};

	assert(writeFile(node, 0, "abc", 3) == 3);
	assert(writeFile(node, 3, "def", 3) == 3);
	assert(node->size == 6);

	assert(readFile(node, 0, buffer, sizeof(buffer)) == 6);
	assert(memcmp(buffer, "abcdef", 6) == 0);

	freeInode(node);
}

/** reads stop at the end of the file, and return 0 past it */
static void test_read_stops_at_end(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char buffer[100] = {0};

	assert(writeFile(node, 0, "0123456789", 10) == 10);

	assert(readFile(node, 4, buffer, sizeof(buffer)) == 6);
	assert(memcmp(buffer, "456789", 6) == 0);

	assert(readFile(node, 10, buffer, sizeof(buffer)) == 0);
	assert(readFile(node, 50, buffer, sizeof(buffer)) == 0);

	freeInode(node);
}

/** reading an empty file gives back nothing */
static void test_read_empty_file(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char buffer[8];
	assert(readFile(node, 0, buffer, sizeof(buffer)) == 0);

	freeInode(node);
}


/* ------------------------- indirect blocks ------------------------- */

/** the first byte past the direct blocks goes through the single indirect */
static void test_single_indirect(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	checkRoundTrip(node, SINGLE_START, "single");

	assert(node->data_blocks[SINGLE_INDIRECT] != 0);
	assert(node->data_blocks[DOUBLE_INDIRECT] == 0);
	assert(node->data_blocks[TRIPLE_INDIRECT] == 0);

	// the direct blocks were skipped, so they were never made
	for (int i = 0; i < DIRECT_BLOCKS; i++)
		assert(node->data_blocks[i] == 0);

	// and reading the skipped part gives back zeros
	char buffer[16];
	memset(buffer, 'x', sizeof(buffer));
	assert(readFile(node, 0, buffer, sizeof(buffer)) == (int64_t)sizeof(buffer));
	for (size_t i = 0; i < sizeof(buffer); i++)
		assert(buffer[i] == 0);

	freeInode(node);
}

/** the first byte past the single indirect goes through the double indirect */
static void test_double_indirect(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	checkRoundTrip(node, DOUBLE_START, "double");

	assert(node->data_blocks[SINGLE_INDIRECT] == 0);
	assert(node->data_blocks[DOUBLE_INDIRECT] != 0);
	assert(node->data_blocks[TRIPLE_INDIRECT] == 0);

	freeInode(node);
}

/** the first byte past the double indirect goes through the triple indirect */
static void test_triple_indirect(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	checkRoundTrip(node, TRIPLE_START, "triple");

	assert(node->data_blocks[SINGLE_INDIRECT] == 0);
	assert(node->data_blocks[DOUBLE_INDIRECT] == 0);
	assert(node->data_blocks[TRIPLE_INDIRECT] != 0);

	freeInode(node);
}

/** a write that crosses from the direct blocks into the single indirect */
static void test_write_into_indirect(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	checkRoundTrip(node, SINGLE_START - 3, "abcdef");

	assert(node->data_blocks[DIRECT_BLOCKS - 1] != 0);
	assert(node->data_blocks[SINGLE_INDIRECT] != 0);

	freeInode(node);
}

/** the very last byte fits, anything past it doesn't */
static void test_max_file_size(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	checkRoundTrip(node, MAX_FILE_SIZE - 1, "z");

	assert(writeFile(node, MAX_FILE_SIZE - 1, "zz", 2) == -1);
	assert(writeFile(node, MAX_FILE_SIZE, "z", 1) == -1);
	assert(node->size == MAX_FILE_SIZE);

	freeInode(node);
}


/* ------------------------------ errors ----------------------------- */

/** a directory i-node can't be written to or read from like a file */
static void test_directory_rejects_read_write(void){
	struct inode *node = mkDir(0755, 0, 0);
	assert(node != NULL);

	char buffer[8];
	assert(writeFile(node, 0, "data", 4) == -1);
	assert(readFile(node, 0, buffer, sizeof(buffer)) == -1);
	assert(node->size == 0);
	assert(node->data_blocks[0] == 0);

	freeInode(node);
}

/** NULL i-nodes and buffers are rejected */
static void test_null_arguments(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char buffer[8];
	assert(writeFile(NULL, 0, "data", 4) == -1);
	assert(writeFile(node, 0, NULL, 4) == -1);
	assert(readFile(NULL, 0, buffer, sizeof(buffer)) == -1);
	assert(readFile(node, 0, NULL, sizeof(buffer)) == -1);

	freeInode(node);
	freeInode(NULL);
}


/* ---------------------------- timestamps --------------------------- */

/** writing sets mtime and ctime, but not atime */
static void test_write_updates_times(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	struct timespec zero = {0};
	node->access_time = zero;
	node->modification_time = zero;
	node->change_time = zero;

	assert(writeFile(node, 0, "data", 4) == 4);

	assert(node->modification_time.tv_sec != 0);
	assert(node->change_time.tv_sec != 0);
	assert(node->access_time.tv_sec == 0);

	freeInode(node);
}

/** reading sets atime, but not mtime or ctime */
static void test_read_updates_atime(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	char buffer[8];
	assert(writeFile(node, 0, "data", 4) == 4);

	struct timespec zero = {0};
	node->access_time = zero;
	node->modification_time = zero;
	node->change_time = zero;

	assert(readFile(node, 0, buffer, sizeof(buffer)) == 4);

	assert(node->access_time.tv_sec != 0);
	assert(node->modification_time.tv_sec == 0);
	assert(node->change_time.tv_sec == 0);

	freeInode(node);
}


/* ----------------------------- freeInode --------------------------- */

/** freeInode() deletes the block files the i-node was using */
static void test_freeInode_removes_blocks(void){
	struct inode *node = mkFile(0644, 0, 0);
	assert(node != NULL);

	assert(writeFile(node, 0, "direct", 6) == 6);
	assert(writeFile(node, SINGLE_START, "single", 6) == 6);
	assert(writeFile(node, DOUBLE_START, "double", 6) == 6);

	uint64_t direct = node->data_blocks[0];
	uint64_t single = node->data_blocks[SINGLE_INDIRECT];
	uint64_t doubleBlock = node->data_blocks[DOUBLE_INDIRECT];

	assert(blockExists(direct));
	assert(blockExists(single));
	assert(blockExists(doubleBlock));

	freeInode(node);

	assert(!blockExists(direct));
	assert(!blockExists(single));
	assert(!blockExists(doubleBlock));
}


/**
 * int main(), this is the main entry point of the program, containing the
 * test functions for fileSystem.c
 *
 * @param: int argc, which is the amount of arguments in the function
 * @param: char *argv[]. the argument variables in order
 *
 * @return: int, return status, zero if successful, non-zero if something went
 *			wrong
 */
int main(int argc, char *argv[]){

	// void out the arugments; the tests don't take any arguments
	(void)argc;
	(void)argv;

	RUN_TEST(test_mkFile_fields);
	RUN_TEST(test_mkDir_fields);
	RUN_TEST(test_permissions_are_masked);
	RUN_TEST(test_inode_numbers_unique);

	RUN_TEST(test_write_read_small);
	RUN_TEST(test_write_across_blocks);
	RUN_TEST(test_overwrite_middle);
	RUN_TEST(test_write_appends);
	RUN_TEST(test_read_stops_at_end);
	RUN_TEST(test_read_empty_file);

	RUN_TEST(test_single_indirect);
	RUN_TEST(test_double_indirect);
	RUN_TEST(test_triple_indirect);
	RUN_TEST(test_write_into_indirect);
	RUN_TEST(test_max_file_size);

	RUN_TEST(test_directory_rejects_read_write);
	RUN_TEST(test_null_arguments);

	RUN_TEST(test_write_updates_times);
	RUN_TEST(test_read_updates_atime);

	RUN_TEST(test_freeInode_removes_blocks);

	printf("all tests passed\n");
	return 0;
}
