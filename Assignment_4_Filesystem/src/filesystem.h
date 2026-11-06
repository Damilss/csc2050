/**
 * file: filesystem.h
 * author: Emilio S <escott15@calpoly.edu>,
 *		   Paris Kalathas <pkalatha@calpoly.edu>
 * date: 2026-11-03
 *
 * filesystem.h - declarations for our own filesystem
 *				  implementation
 */
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <sys/types.h>

#define _POSIX_C_SOURCE 200809L

/* ----- Some type defs to make things easy to write and understand ----- */
typedef struct Filesystem Filesystem;
typedef struct Bootsector Bootsector;
typedef struct Superblock Superblocks;
typedef struct Cylindergroup Cylindergroup;
typedef struct Cylindergroupinfo Cylindergroupinfo;
typedef struct Inodebitmap Inodebitmap;
typedef struct Blockbitmap Blockbitmap;
typedef struct Inodearray Inodearray;
typedef struct Blockarray Blockarray;
typedef struct INode INode;
typedef int fileDescriptor;

/* ----- Function prototypes -----*/

/* creates a dir */
int mkDir(char *dirname);

/* creates a file */
int mkFile(char *filename);

/* writes numBytes from a bugger in a file */
int writeFile(char *filename, char *buffer, int numBytes);

/**
 * readFile - ...
 *
 */
int readFile(char* fileName, char* buffer, int numBytes);

/**
 * chDir - ...
 *
 */

void chDir (char *dirPath);

/* reads numBytes from a file into the buffer
 * prints the inode info oncsreen, something like the ls -l command */
void printinodeinfo (char *filename);


/**
 * struct inode - inode implementation
 *
 * @filename: name of file
 * @nodeid: unique ID for inode
 * @filetype: single char, 'f' for files, 'd' for directories
 * @filesize: total file size of the inode represented by
 *			  by regular signed int
 * @ownerID: unique owner ID
 * @groupID: unique group ID
 * @permissions: includes the permissions for the file our
 *				 implementation is for the user only, defualt
 *				 is 6 ( read and write  )
 * @hardlink_count: number of hard links pointing to the inode
 * @int blockOffset: data block offset
 */
struct Inode {
	char *filename;
	int nodeid;
	char filetype;
	int filesize;
	uid_t ownerid;
	gid_t groupid;
	int permissions;
	int hardlinknumber;
	int blockoffest;
	char *datablockarray[14];
};
#endif
