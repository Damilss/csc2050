/**
 * file: fileSystem.h
 * author: Emilio L <escott15@calpoly.edu>
 * date: 2026-11-01a
 *
 * the header filer for the fileSystem file, containing the declarations
 * for inode and the pertaining functions
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <inttypes.h>
#include <time.h>

/* Pointer size in this implementation are 8 bytes */
#define POINTER_SIZE 8

/* Are determined blocks size for our implementation is 1024 bytes */
#define BLOCK_SIZE 1024

/* data starts at the very beginning of every block file */
#define BLOCK_OFFSET 0

/* the 15 data block pointers: 0-11 are direct, 12 is single indirect,
 * 13 is double indirect and 14 is triple indirect */
#define DIRECT_BLOCKS 12
#define SINGLE_INDIRECT 12
#define DOUBLE_INDIRECT 13
#define TRIPLE_INDIRECT 14
#define TOTAL_BLOCKS 15
#define INDIRECT_LEVELS 3

/* every block is its own file on disk, named after its block number */
#define BLOCK_FILE_FORMAT "block_%" PRIu64 ".dat"
#define BLOCK_PATH_SIZE 64

/* file types, stored in the upper bits of mode (same values Linux uses) */
#define INODE_TYPE_MASK 0170000
#define INODE_DIRECTORY 0040000
#define INODE_REGULAR 0100000
#define INODE_SYMLINK 0120000

/* permissions (rwxrwxrwx) are the lower 9 bits of mode */
#define INODE_PERMISSION_MASK 0777

#define INODE_IS_REGULAR(mode) (((mode) & INODE_TYPE_MASK) == INODE_REGULAR)
#define INODE_IS_DIRECTORY(mode) (((mode) & INODE_TYPE_MASK) == INODE_DIRECTORY)


/**
 * struct inode is our own specific implementation of inode using different
 * unsigned int types to represent the values
 *
 * @param: uint64_t inode_number, this is the file i-node number / serial
 *		   number to identify the inode
 * @param: uint32_t mode, modes / permissions, as well as declaring what
 *		   what kind of file it is (dir, symlink, FIFO, etc)
 * @param: uint32_t owner_uid User ID of the owner of the inode / file
 * @param: uint32_t owner_gid, User ID of the group of the inode / file
 * @param: uint64_t size, number of bytes of the selected file
 * @param: uint32_t link_count, keeps track of how many files / links point
 *		   to this specific inode
 * @param: uint32_t block_size, number of characters in each block file
 * @param: uint32_t block_offset, the bytes from the beginning of each block
 *		   file where the data is stored
 * @param: struct timespec access_time, the time the inode / file was
 *		   accessed
 * @param: struct timespec modification_time, the time the inode / file was
 *		   modified / changed
 * @param: struct timespec change_time, the time the inode's metadata was
 *		   changed
 * @param: uint64_t data_blocks[15], block numbers of the files holding the
 *		   data, 0 means the block hasn't been made yet. 0-11 point to data
 *		   blocks, 12-14 point to blocks full of more block numbers
 */
struct inode {
	uint64_t inode_number;
	uint32_t mode;
	uint32_t owner_uid;
	uint32_t owner_gid;
	uint64_t size;
	uint32_t link_count;
	uint32_t block_size;
	uint32_t block_offset;

	struct timespec access_time;
	struct timespec modification_time;
	struct timespec change_time;

	uint64_t data_blocks[TOTAL_BLOCKS];
};

/**
 * mkDir() creates a directory i-node, starting with a link count of 2 (its
 * entry in the parent directory and its own ".")
 *
 * @param: uint32_t permissions, the rwx bits for the directory, ex: 0755 for
 *		   rwxr-xr-x
 * @param: uint32_t owner_uid, User ID of the owner
 * @param: uint32_t owner_gid, Group ID of the owner
 *
 * @return: struct inode *, the new directory i-node, NULL if malloc failed.
 *			free it with freeInode()
 */
struct inode *mkDir(uint32_t permissions, uint32_t owner_uid, uint32_t owner_gid);

/**
 * mkFile() creates an empty regular file i-node, starting with a link count
 * of 1
 *
 * @param: uint32_t permissions, the rwx bits for the file, ex: 0644 for
 *		   rw-r--r--
 * @param: uint32_t owner_uid, User ID of the owner
 * @param: uint32_t owner_gid, Group ID of the owner
 *
 * @return: struct inode *, the new file i-node, NULL if malloc failed.
 *			free it with freeInode()
 */
struct inode *mkFile(uint32_t permissions, uint32_t owner_uid, uint32_t owner_gid);

/**
 * writeFile() writes data into a file i-node starting at offset, making any
 * block files it needs along the way. writing past the end grows the file
 *
 * @param: struct inode *node, the file i-node to write to
 * @param: uint64_t offset, the byte in the file to start writing at
 * @param: const char *data, the bytes to write
 * @param: uint64_t length, how many bytes of data to write
 *
 * @return: int64_t, the number of bytes written, -1 if node isn't a regular
 *			file, the write doesn't fit in the i-node, or a block file
 *			couldn't be written
 */
int64_t writeFile(struct inode *node, uint64_t offset, const char *data, uint64_t length);

/**
 * readFile() reads data out of a file i-node starting at offset. it stops at
 * the end of the file, and does not add a '\0' to the end of buffer
 *
 * @param: struct inode *node, the file i-node to read from
 * @param: uint64_t offset, the byte in the file to start reading at
 * @param: char *buffer, where the bytes get copied to
 * @param: uint64_t length, the most bytes to read
 *
 * @return: int64_t, the number of bytes read (0 at the end of the file), -1
 *			if node isn't a regular file or a block file couldn't be read
 */
int64_t readFile(struct inode *node, uint64_t offset, char *buffer, uint64_t length);

/**
 * freeInode() deletes every block file the i-node uses, then frees the
 * i-node itself
 *
 * @param: struct inode *node, the i-node to free, NULL does nothing
 */
void freeInode(struct inode *node);

#endif
