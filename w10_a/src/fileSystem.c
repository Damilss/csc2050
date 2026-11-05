/**
 * file: fileSystem.c
 * author: Emilio L <escott15@calpoly.edu>
 * date: 2026-11-01
 *
 * implementation of the inode and its functions. every data block is its own
 * file on disk (see BLOCK_FILE_FORMAT), and indirect blocks are files full of
 * 8 byte block numbers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileSystem.h"

/* the next unused i-node number, so every i-node gets a unique id */
static uint64_t next_inode_number = 1;

/* the next unused block number, 0 is saved to mean "no block" */
static uint64_t next_block_number = 1;


/* ************************************************************************
	Helper Functions
************************************************************************ */

/**
 * currentTime() gets the current time for the i-node timestamps
 *
 * @return: struct timespec, the current time
 */
static struct timespec currentTime(void){
	struct timespec time = {0};
	timespec_get(&time, TIME_UTC);
	return time;
}

/**
 * maxFileSize() calculates the largest file an i-node can hold, using all
 * the direct, single, double and triple indirect blocks
 *
 * @param: const struct inode *node, the i-node to use the block size of
 *
 * @return: uint64_t, the max number of bytes in the file
 */
static uint64_t maxFileSize(const struct inode *node){
	uint64_t pointers = node->block_size / POINTER_SIZE;
	uint64_t blocks = DIRECT_BLOCKS + pointers + pointers * pointers
		+ pointers * pointers * pointers;

	return blocks * node->block_size;
}

/**
 * blockPath() builds the file name of a block from its block number
 *
 * @param: uint64_t block, the block number
 * @param: char *path, where the file name is stored
 * @param: size_t pathSize, size of path in bytes
 */
static void blockPath(uint64_t block, char *path, size_t pathSize){
	snprintf(path, pathSize, BLOCK_FILE_FORMAT, block);
}

/**
 * allocateBlock() creates a new block file filled with zeros, so unused
 * pointers in an indirect block read as 0
 *
 * @param: const struct inode *node, the i-node the block is for
 *
 * @return: uint64_t, the new block number, 0 if the file couldn't be made
 */
static uint64_t allocateBlock(const struct inode *node){
	uint64_t block = next_block_number++;
	char path[BLOCK_PATH_SIZE];
	blockPath(block, path, sizeof(path));

	FILE *file = fopen(path, "wb");
	if (file == NULL)
		return 0;

	for (uint32_t i = 0; i < node->block_offset + node->block_size; i++)
		fputc(0, file);

	int failed = ferror(file);
	if (fclose(file) != 0 || failed)
		return 0;

	return block;
}

/**
 * readBlock() reads bytes out of a block file
 *
 * @param: const struct inode *node, the i-node the block belongs to
 * @param: uint64_t block, the block number to read from
 * @param: uint64_t position, the byte in the block to start at
 * @param: void *buffer, where the bytes get copied to
 * @param: uint64_t length, number of bytes to read
 *
 * @return: int, 0 if successful, -1 if something went wrong
 */
static int readBlock(
		const struct inode *node,
		uint64_t block,
		uint64_t position,
		void *buffer,
		uint64_t length
		){

	char path[BLOCK_PATH_SIZE];
	blockPath(block, path, sizeof(path));

	FILE *file = fopen(path, "rb");
	if (file == NULL)
		return -1;

	int status = 0;
	if (fseek(file, (long)(node->block_offset + position), SEEK_SET) != 0 ||
			fread(buffer, 1, (size_t)length, file) != length)
		status = -1;

	fclose(file);
	return status;
}

/**
 * writeBlock() writes bytes into an existing block file
 *
 * @param: const struct inode *node, the i-node the block belongs to
 * @param: uint64_t block, the block number to write to
 * @param: uint64_t position, the byte in the block to start at
 * @param: const void *data, the bytes to write
 * @param: uint64_t length, number of bytes to write
 *
 * @return: int, 0 if successful, -1 if something went wrong
 */
static int writeBlock(
		const struct inode *node,
		uint64_t block,
		uint64_t position,
		const void *data,
		uint64_t length
		){

	char path[BLOCK_PATH_SIZE];
	blockPath(block, path, sizeof(path));

	FILE *file = fopen(path, "r+b");
	if (file == NULL)
		return -1;

	int status = 0;
	if (fseek(file, (long)(node->block_offset + position), SEEK_SET) != 0 ||
			fwrite(data, 1, (size_t)length, file) != length)
		status = -1;

	if (fclose(file) != 0)
		status = -1;

	return status;
}

/**
 * findBlock() finds the block number holding a block of the file, going
 * through the indirect blocks when it's past the 12 direct blocks
 *
 * @param: struct inode *node, the i-node to look in
 * @param: uint64_t index, which block of the file, ex: 0 is bytes 0-1023
 * @param: int allocate, 1 = make any missing blocks, 0 = don't
 * @param: uint64_t *block, set to the block number, or 0 if the block was
 *		   never made and allocate is 0
 *
 * @return: int, 0 if successful, -1 if something went wrong
 */
static int findBlock(struct inode *node, uint64_t index, int allocate, uint64_t *block){
	uint64_t pointers = node->block_size / POINTER_SIZE;
	uint64_t span = 1;
	int depth = 0;
	uint64_t *slot;

	*block = 0;

	// pick which of the 15 pointers covers this index, and how deep it goes
	if (index < DIRECT_BLOCKS){
		slot = &node->data_blocks[index];
	} else {
		index -= DIRECT_BLOCKS;
		depth = 1;
		span = pointers;

		while (index >= span){
			index -= span;
			span *= pointers;
			depth++;

			// past what the triple indirect pointer can reach
			if (depth > INDIRECT_LEVELS)
				return -1;
		}
		slot = &node->data_blocks[DIRECT_BLOCKS + depth - 1];
	}

	if (*slot == 0){
		if (!allocate)
			return 0;

		*slot = allocateBlock(node);
		if (*slot == 0)
			return -1;
	}

	// walk down through the indirect blocks, one level each loop
	uint64_t current = *slot;
	while (depth > 0){
		span /= pointers;
		uint64_t entry = index / span;
		index %= span;

		uint64_t next = 0;
		if (readBlock(node, current, entry * POINTER_SIZE, &next, POINTER_SIZE) != 0)
			return -1;

		if (next == 0){
			if (!allocate)
				return 0;

			next = allocateBlock(node);
			if (next == 0 ||
					writeBlock(node, current, entry * POINTER_SIZE, &next, POINTER_SIZE) != 0)
				return -1;
		}

		current = next;
		depth--;
	}

	*block = current;
	return 0;
}

/**
 * removeBlock() deletes a block file, and if it's an indirect block, every
 * block it points to first
 *
 * @param: const struct inode *node, the i-node the block belongs to
 * @param: uint64_t block, the block number to delete, 0 does nothing
 * @param: int depth, 0 for a data block, 1-3 for single-triple indirect
 */
static void removeBlock(const struct inode *node, uint64_t block, int depth){

	if (block == 0)
		return;

	if (depth > 0){
		uint64_t pointers = node->block_size / POINTER_SIZE;

		for (uint64_t entry = 0; entry < pointers; entry++){
			uint64_t child = 0;
			if (readBlock(node, block, entry * POINTER_SIZE, &child, POINTER_SIZE) == 0)
				removeBlock(node, child, depth - 1);
		}
	}

	char path[BLOCK_PATH_SIZE];
	blockPath(block, path, sizeof(path));
	remove(path);
}

/**
 * createInode() allocates an i-node and fills in the fields shared by
 * mkDir() and mkFile()
 *
 * @param: uint32_t type, INODE_DIRECTORY, INODE_REGULAR, etc
 * @param: uint32_t permissions, the rwx bits
 * @param: uint32_t owner_uid, User ID of the owner
 * @param: uint32_t owner_gid, Group ID of the owner
 * @param: uint32_t link_count, the starting hard link count
 *
 * @return: struct inode *, the new i-node, NULL if malloc failed
 */
static struct inode *createInode(
		uint32_t type,
		uint32_t permissions,
		uint32_t owner_uid,
		uint32_t owner_gid,
		uint32_t link_count
		){

	// calloc so size is 0 and every block pointer starts as 0 (unused)
	struct inode *node = calloc(1, sizeof(struct inode));
	if (node == NULL){
		fprintf(stderr, "createInode(): calloc returned NULL\n");
		return NULL;
	}

	node->inode_number = next_inode_number++;
	node->mode = type | (permissions & INODE_PERMISSION_MASK);
	node->owner_uid = owner_uid;
	node->owner_gid = owner_gid;
	node->link_count = link_count;
	node->block_size = BLOCK_SIZE;
	node->block_offset = BLOCK_OFFSET;

	struct timespec time = currentTime();
	node->access_time = time;
	node->modification_time = time;
	node->change_time = time;

	return node;
}


/* ************************************************************************
	I-node Functions
************************************************************************ */

struct inode *mkDir(uint32_t permissions, uint32_t owner_uid, uint32_t owner_gid){
	return createInode(INODE_DIRECTORY, permissions, owner_uid, owner_gid, 2);
}

struct inode *mkFile(uint32_t permissions, uint32_t owner_uid, uint32_t owner_gid){
	return createInode(INODE_REGULAR, permissions, owner_uid, owner_gid, 1);
}

int64_t writeFile(struct inode *node, uint64_t offset, const char *data, uint64_t length){

	if (node == NULL || data == NULL || !INODE_IS_REGULAR(node->mode))
		return -1;

	uint64_t maxSize = maxFileSize(node);
	if (offset > maxSize || length > maxSize - offset)
		return -1;

	uint64_t written = 0;
	while (written < length){
		uint64_t position = offset + written;
		uint64_t inBlock = position % node->block_size;
		uint64_t chunk = node->block_size - inBlock;

		// the last chunk can be smaller than what's left in the block
		if (chunk > length - written)
			chunk = length - written;

		uint64_t block = 0;
		if (findBlock(node, position / node->block_size, 1, &block) != 0 ||
				writeBlock(node, block, inBlock, data + written, chunk) != 0)
			break;

		written += chunk;
	}

	if (written > 0 && offset + written > node->size)
		node->size = offset + written;

	struct timespec time = currentTime();
	node->modification_time = time;
	node->change_time = time;

	if (written < length)
		return -1;

	return (int64_t)written;
}

int64_t readFile(struct inode *node, uint64_t offset, char *buffer, uint64_t length){

	if (node == NULL || buffer == NULL || !INODE_IS_REGULAR(node->mode))
		return -1;

	// stop at the end of the file
	uint64_t available = offset < node->size ? node->size - offset : 0;
	if (length > available)
		length = available;

	uint64_t done = 0;
	while (done < length){
		uint64_t position = offset + done;
		uint64_t inBlock = position % node->block_size;
		uint64_t chunk = node->block_size - inBlock;

		if (chunk > length - done)
			chunk = length - done;

		uint64_t block = 0;
		if (findBlock(node, position / node->block_size, 0, &block) != 0)
			return -1;

		// a block that was skipped over and never written reads as zeros
		if (block == 0)
			memset(buffer + done, 0, (size_t)chunk);
		else if (readBlock(node, block, inBlock, buffer + done, chunk) != 0)
			return -1;

		done += chunk;
	}

	node->access_time = currentTime();
	return (int64_t)done;
}

void freeInode(struct inode *node){

	if (node == NULL)
		return;

	for (int i = 0; i < TOTAL_BLOCKS; i++){
		// 0 for the direct blocks, then 1, 2, 3 for single, double, triple
		int depth = i < DIRECT_BLOCKS ? 0 : i - DIRECT_BLOCKS + 1;
		removeBlock(node, node->data_blocks[i], depth);
	}

	free(node);
}
