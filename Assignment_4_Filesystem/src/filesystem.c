/**
 * file: filesystem.c
 * authors: Emilio S <escott15@calpoly.edu>
 *			Paris Kalathas <pkalatha@calpoly.edu>
 * date: 2026-11-03
 *
 * filesystem.c - our very own filesystem and definitions
 */

/* must come before any system header, or it has no effect */
#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

/***********************************************************************
 *		TYPEDEFS
 ***********************************************************************/

/* ----- Some typedefs to make things easy to write and understand ----- */
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

/***********************************************************************
 *		STRUCTS
 ***********************************************************************/

/**
 * struct INode - inode implementation
 *
 * @fileName:       name of file
 * @nodeID:         unique ID for inode
 * @fileType:       single char, 'f' for files, 'd' for directories
 * @fileSize:       total file size of the inode, represented by a
 *					regular signed int
 * @ownerID:        unique owner ID
 * @groupID:        unique group ID
 * @permissions:    includes the permissions for the file, our
 *					implementation is for the user only, default
 *					is 6 (read and write)
 * @hardLinkNumber: number of hard links pointing to the inode
 * @blockOffset:    data block offset for semi-filled data blocks
 * @dataBlockArray: data block pointers, the last 2 are indirect
 */
struct INode {
	char *fileName;
	int nodeID;
	char fileType;
	int fileSize;
	uid_t ownerID;
	gid_t groupID;
	int permissions;
	int hardLinkNumber;
	int blockOffset;
	char *dataBlockArray[14];
};

/***********************************************************************
 *		FUNCTION PROTOTYPES
 ***********************************************************************/

/**
 * mkDir - creates a directory in the current working directory
 *
 * @dirname: name of the new directory
 *
 * @return: 0 on success, -1 on failure
 */
int mkDir(char *dirname);

/**
 * mkFile - creates a file in the current working directory
 *
 * @filename: name of the new file
 *
 * @return: 0 on success, -1 on failure
 */
int mkFile(char *filename);

/**
 * writeFile - writes numBytes from a buffer in a file
 *
 * @filename: name of the file to write to
 * @buffer:   data to write
 * @numBytes: number of bytes to write from the buffer
 *
 * @return: 0 on success, -1 on failure
 */
int writeFile(char *filename, char *buffer, int numBytes);

/**
 * readFile - takes a filename, buffer, and a number of bytes and goes through
 *			  the file to read byte by byte, as well as update the buffer
 *
 * @fileName: name of the specified file
 * @buffer:   memory that the bytes read from the file are copied into
 * @numBytes: number of bytes to read
 *
 * @return: 0 on success, -1 on failure
 */
int readFile(char *fileName, char *buffer, int numBytes);

/**
 * chDir - takes a dirPath and changes the current working directory to that
 *		   specific directory
 *
 * @dirPath: specified dirpath to take
 *
 * @return: void
 */
void chDir(char *dirPath);

/**
 * printINodeInfo - prints the inode info on screen, something like the
 *					ls -l command
 *
 * @filename: the specified file name to print the information of
 *
 * @return: void
 */
void printINodeInfo(char *filename);

/***********************************************************************
 *		GLOBALS
 ***********************************************************************/

/*
 * These values are mainly seed values and for comprehension,
 * allowing students to better understand how these values work.
 * These values are set later on by _initFS() function. And they
 * are only set once, hence the all caps. They are to be treated as
 * constants. They are not macros, and they are not const because
 * they are assigned once. Macros can't be assigned values, and const
 * makes the var immutable, so unable to set these values const. Again,
 * these values are mainly here for comprehension
 */

/* set the disk size for your file system */
int DISK_SIZE = 1048576;

/* this is here to match the slides. This functionality
 * is not supported by the back-end, so don't change it */
int CYL_BLOCK_NUM = 1;

/* set the size of your blocks */
int BLOCK_SIZE = 1024;

/* the number of blocks based on the disk and block size
 * (DISK_SIZE / BLOCK_SIZE), previously BLOCK_NUM */
int NUM_OF_BLOCKS = 1024;

/* the number of inodes, 10% of NUM_OF_BLOCKS */
int NUM_OF_INODES = 1024 * 0.1;

/* even though this is available, it can't be used directly
 * because of std=c99, inode struct, set it manually */
int INODE_PTR_ARRAY_SIZE = 14;

/* the amount of indirect pointers pointed by the last two elements
 * in the inode data block array. BLOCK_SIZE / 2 so each inode uses
 * one file for both of its indirect elements */
int INODE_INDIRECT_PTRS = (1024 / 2);

/* keeps track how many inodes are currently in use. This needs to
 * be recalculated in each session from the inode bitmap. It's
 * faster to do it once at the beginning of the session than calculate
 * it each time an inode needs to be created */
int inodeidx = 0;

/* keeps track of the current working directory. The PWD variable in
 * linux. This needs to be updated every time the user navigates to
 * a new directory */
char *workingDirectory = "~";

/* paths of the cylinder group files that mkDir works with */
#define INODE_ARRAY_PATH  "filesystem/cylinderBlock_0/inodearray"
#define INODE_BITMAP_PATH "filesystem/cylinderBlock_0/inodebitmap"
#define BLOCK_BITMAP_PATH "filesystem/cylinderBlock_0/blockbitmap"
#define DATA_BLOCK_PATH   "filesystem/cylinderBlock_0/datablocks/datablock_%d"

/* longest file name allowed, the same as the input buffer in main */
#define MAX_NAME_LEN 127

/* positions of the fields on an inode line in the inode array file,
 * every field is followed by one space */
#define FIELD_NODE_ID      1
#define FIELD_TYPE         2
#define FIELD_SIZE         3
#define FIELD_OWNER        4
#define FIELD_GROUP        5
#define FIELD_PERMISSIONS  6
#define FIELD_HARD_LINKS   7
#define FIELD_BLOCK_OFFSET 8
#define FIELD_DATA_ARRAY   9

/* data array slots 0 and 1 hold the parent and own directory names, and
 * the last 2 slots are indirect pointers, so slots 2 to 11 are the direct
 * data block pointers */
#define FIRST_BLOCK_SLOT 2
#define LAST_BLOCK_SLOT  11

/* number of data array slots, the same as INODE_PTR_ARRAY_SIZE, but that
 * is a variable, so it can't be used as the size of an array */
#define NUM_OF_SLOTS 14

/* number of fields on an inode line, the data array slots come last */
#define NUM_OF_FIELDS ( FIELD_DATA_ARRAY + NUM_OF_SLOTS )

/***********************************************************************
 *		HELPER FUNCTIONS
 ***********************************************************************/

/**
 * _initFS - helper function, initializes the file system
 *
 * @return: 0 on success, -1 on failure
 */
int _initFS(void);

/**
 * _processUserInput - helper function, splits the user input into tokens
 *
 * @userInput:  raw line read from stdin
 * @inputBytes: number of bytes in userInput
 * @command:    set to the 1st token, the command name
 * @fileName:   set to the 2nd token, or "" if there isn't one
 * @buffer:     set to the 3rd token, or "" if there isn't one
 * @numBytes:   set to the 4th token as an int, or 0 if there isn't one
 *
 * @return: void
 */
void _processUserInput(
		char *userInput,
		int inputBytes,
		char *command,
		char *fileName,
		char *buffer,
		int *numBytes
		);

/**
 * _executeCommand - helper function, runs the function for the command
 *
 * @command:  name of the command to run
 * @fileName: file or directory name passed to the command
 * @buffer:   data passed to writeFile and readFile
 * @numBytes: number of bytes passed to writeFile and readFile
 *
 * @return: status of the command, 0 on success, -1 on failure
 */
int _executeCommand(
		char *command,
		char *fileName,
		char *buffer,
		int *numBytes
		);

/**
 * _updateSuperblock - stores file system info inside the superblock
 *
 * @disksize:            size of the disk in bytes
 * @cylblocknum:         number of cylinder blocks
 * @blocksize:           size of each block in bytes
 * @blocknum:            number of blocks
 * @inodenum:            number of inodes
 * @inodedeptharraysize: number of pointers in the inode data block array
 * @inodeindirectptrs:   number of indirect pointers per inode
 * @inodeidx:            number of inodes currently in use
 *
 * @return: 0 on success, -1 if the superblock can't be opened
 */
int _updateSuperblock(
		int disksize,
		int cylblocknum,
		int blocksize,
		int blocknum,
		int inodenum,
		int inodedeptharraysize,
		int inodeindirectptrs,
		int inodeidx
		);

/**
 * _initGlobals - fetches the values for global variables from the super block
 *
 * @return: 0 on success, -1 if the superblock can't be opened
 */
int _initGlobals();

/**
 * _loadFile - reads a whole file into memory
 *
 * @path: path of the file to read
 * @size: set to the number of bytes read
 *
 * @return: the contents with a '\0' at the end, which the caller has to
 *			free, or NULL on failure
 */
char *_loadFile(const char *path, int *size);

/**
 * _replaceInFile - replaces part of a file's contents and saves the file
 *
 * @path:     path of the file to save
 * @contents: current contents of the file
 * @size:     size of contents
 * @start:    where the replaced part starts in contents
 * @oldLen:   length of the replaced part
 * @newText:  text that takes its place
 *
 * @return: 0 on success, -1 on failure
 */
int _replaceInFile(
		const char *path,
		const char *contents,
		int size,
		int start,
		int oldLen,
		const char *newText
		);

/**
 * _findInodeLine - finds an inode's line inside the inode array
 *
 * @contents: contents of the inode array file
 * @name:     file name of the inode, the 1st field on its line
 *
 * @return: start of the line, or NULL if there is no such inode
 */
char *_findInodeLine(char *contents, const char *name);

/**
 * _findField - finds one field on an inode line
 *
 * @line:  start of the inode line
 * @field: position of the field, 0 is the file name
 * @len:   set to the length of the field
 *
 * @return: start of the field, or NULL if the line has no such field
 */
char *_findField(char *line, int field, int *len);

/**
 * _getInodeField - copies one field of an inode from the inode array
 *
 * @name:    file name of the inode
 * @field:   position of the field, 0 is the file name
 * @out:     set to the field's text
 * @outSize: size of out
 *
 * @return: 0 on success, -1 if the field isn't found or doesn't fit in out
 */
int _getInodeField(const char *name, int field, char *out, int outSize);

/**
 * _setInodeField - changes one field of an inode in the inode array
 *
 * @name:  file name of the inode
 * @field: position of the field, 0 is the file name
 * @value: new text of the field
 *
 * @return: 0 on success, -1 on failure
 */
int _setInodeField(const char *name, int field, const char *value);

/**
 * _appendInode - writes a new inode on the free line of the inode array
 *
 * @node: inode to write
 *
 * @return: index of the inode's line, or -1 on failure
 */
int _appendInode(const INode *node);

/**
 * _findFreeBitmapLine - finds the first free (0) line of a bitmap file
 *
 * @path: path of the bitmap file
 *
 * @return: index of the line, or -1 if every line is in use
 */
int _findFreeBitmapLine(const char *path);

/**
 * _setBitmapLine - changes one line of a bitmap file
 *
 * @path:  path of the bitmap file
 * @index: index of the line
 * @value: '1' for in use, '0' for free
 *
 * @return: 0 on success, -1 on failure
 */
int _setBitmapLine(const char *path, int index, char value);

/**
 * _addHardLink - adds a hard link to a new inode in the parent directory's
 *				  data block
 *
 * @parentName: file name of the parent directory
 * @childName:  file name of the new inode
 * @childID:    nodeID of the new inode
 *
 * @return: 0 on success, -1 on failure
 */
int _addHardLink(const char *parentName, const char *childName, int childID);

/***********************************************************************
 *		MAIN
 ***********************************************************************/

/**
 * main - entry point of the program
 *
 * @argc: argument count
 * @argv: the argument variables as an array of chars
 *
 * @return: 0 on success, -1 on failure
 */
int main(int argc, char *argv[]){
	/* welcomes the user, prints specs and instructions */
	printf( "!!--> File System Specs <--!!\n" );
	printf( "Disk size: %d\n", DISK_SIZE );
	printf( "Cylinder block number: %d\n", CYL_BLOCK_NUM );
	printf( "Block size: %d\n", BLOCK_SIZE );
	printf( "Block number: %d\n", NUM_OF_BLOCKS );
	printf( "I-node indirect pointer number: %d\n", INODE_INDIRECT_PTRS );
	printf(
		"------------------------------------"
		"------------------------------------\n\n"
	);

	printf( "Welcome to the CSC2050 File System!\n" );
	printf( "What would you like to do?\n" );
	printf( "Available choices are:\n" );

	printf(
		"\tmkDir char *dirName"
		"                                  -> to create a directory\n"
	);

	printf(
		"\tmkFile char *fileName"
		"                                -> to create a file\n"
	);

	printf(
		"\twriteFile char *fileName, char* buffer, int numBytes "
		"-> to write numBytes from the buffer in a file\n"
	);

	printf(
		"\treadFile char *fileName, char* buffer, int numBytes"
		"  -> to read numBytes from a file into the buffer\n"
	);

	printf(
		"\tprintINodeInfo char *fileName"
		"                        -> to print the inode info on the screen\n"
	);

	printf(
		"\tchDir char *dirPath"
		"                                  -> to change the current "
		"working directory\n"
	);

	printf(
		"\texit"
		"                                                 -> to exit "
		"the filesystem\n"
	);

	/* Create the filesystem structure and creates the home directory */
	if ( _initFS() == -1 ){
		printf( "Error creating file system\n");
		return -1;
	}

	/* variables to hold */
	char command[32];
	char fileName[128];
	char buffer[128];
	int numBytes;

	/* the userinput */
	char userInput[512];
	int inputBytes;
	int status = 0;

	do {
		/* resets the userinput state for every input */
		numBytes = 0; inputBytes = 0;

		/* print the prompt symbol */
		write( STDOUT_FILENO, ":> ", 3 );

		/* reads the userinput */
		inputBytes = read( STDIN_FILENO, userInput, 511 );

		/* checks for empty input */
		if ( inputBytes == 0 ){
			printf( "Sorry, didn't get that. Please type again\n" );
			continue;
		}

		/* processes the userinput to extract the tokens for each command.
		 * It can be replaced by strtok() */
		_processUserInput(
			userInput,
			inputBytes,
			command,
			fileName,
			buffer,
			&numBytes
		);

		/* selects and calls the commands. Returns -1 on error 0 on success */
		status = _executeCommand( command, fileName, buffer, &numBytes );
		if ( status == -1 ){
			printf("Command failed\n");
		}

	} while ( strcmp( command, "exit") != 0 );

	/* updates the superblock before exiting the program */
	_updateSuperblock(
		DISK_SIZE,
		CYL_BLOCK_NUM,
		BLOCK_SIZE,
		NUM_OF_BLOCKS,
		NUM_OF_INODES,
		INODE_PTR_ARRAY_SIZE,
		INODE_INDIRECT_PTRS,
		inodeidx
	);
	return status;
}

/**
 * _processUserInput - helper function, splits the user input into tokens
 *
 * @userInput:  raw line read from stdin
 * @inputBytes: number of bytes in userInput
 * @command:    set to the 1st token, the command name
 * @fileName:   set to the 2nd token, or "" if there isn't one
 * @buffer:     set to the 3rd token, or "" if there isn't one
 * @numBytes:   set to the 4th token as an int, or 0 if there isn't one
 *
 * @return: void
 */
void _processUserInput(
		char *userInput,
		int inputBytes,
		char *command,
		char *fileName,
		char *buffer,
		int *numBytes
		){
	int userInputIdx = 0;   /* index for the userinput char array */
	int variableIdx = 0;    /* index for each variable */
	char tmpChar = userInput[userInputIdx];

	/* the tokens after the command are optional, and main reuses the same
	 * arrays for every command, so they are cleared first. Otherwise a
	 * command typed without a name would get the previous command's name,
	 * or garbage if it's the first command */
	fileName[0] = '\0';
	buffer[0] = '\0';
	*numBytes = 0;

	/* first loop processes the command, reads characters from the
	 * beginning until it finds a line change or a space character */
	while ( tmpChar != '\n' && tmpChar != ' ' ){
		/* stores every character in the command variable */
		command[variableIdx] = tmpChar;
		userInputIdx++; variableIdx++;      /* updates the indices */
		tmpChar = userInput[userInputIdx];  /* updates to the next character */
	}
	command[userInputIdx] = '\0';  /* terminates the variable */

	/* updates the userinput index to the beginning of the next token */
	userInputIdx++;

	/* second loop processes the filename if it exists, checks if the
	 * end of the line is reached the variable index */
	if ( userInputIdx < inputBytes ){
		tmpChar = userInput[userInputIdx];  /* updates to the next character */
		variableIdx = 0;                    /* resets the variable index */

		while ( tmpChar != '\n' && tmpChar != ' ' ){
			fileName[variableIdx] = tmpChar;
			userInputIdx++; variableIdx++;
			tmpChar = userInput[userInputIdx];
		}
		fileName[variableIdx] = '\0';

		/* updates the userinput index to the beginning of the next token */
		userInputIdx++;
	}

	/* third loop processes the buffer if it exists */
	if ( userInputIdx < inputBytes ){
		tmpChar = userInput[userInputIdx];  /* updates to the next character */
		variableIdx = 0;                    /* resets the variable index */

		while ( tmpChar != '\n' && tmpChar != ' ' ){
			buffer[variableIdx] = tmpChar;
			userInputIdx++; variableIdx++;
			tmpChar = userInput[userInputIdx];
		}
		buffer[variableIdx] = '\0';

		/* updates the userinput index to the beginning of the next token */
		userInputIdx++;
	}

	/* fourth loop processes the number of bytes if they exist */
	if ( userInputIdx < inputBytes ){
		tmpChar = userInput[userInputIdx];  /* updates to the next character */
		variableIdx = 0;                    /* resets the variable index */

		char numBytesbuffer[128];
		while ( tmpChar != '\n' && tmpChar != ' ' ){
			numBytesbuffer[variableIdx] = tmpChar;
			userInputIdx++; variableIdx++;
			tmpChar = userInput[userInputIdx];
		}
		numBytesbuffer[variableIdx] = tmpChar;

		/* converts numerical characters to integers */
		int tmp = 1;
		*numBytes = 0;

		/* traverses through the string */
		for ( int i = 0; i < variableIdx; i++ ){
			tmp = 1;

			/* calculates the base multiplication 1, 10, 100, 1000 */
			for ( int j = i; j < variableIdx-1; j++ ){
				tmp *= 10;
			}

			/* get the number by subtracting its ASCII value from character '0'
			 * then multiplies the number by its base to get its numeric value
			 * finally adds all the digits to get the actual number */
			*numBytes += (numBytesbuffer[i] - '0') * tmp;
		}
	}
}

/**
 * _executeCommand - helper function, runs the function for the command
 *
 * @command:  name of the command to run
 * @fileName: file or directory name passed to the command
 * @buffer:   data passed to writeFile and readFile
 * @numBytes: number of bytes passed to writeFile and readFile
 *
 * @return: status of the command, 0 on success, -1 on failure
 */
int _executeCommand(
		char *command,
		char *fileName,
		char *buffer,
		int *numBytes
		){
	int status = 0;
	if ( strcmp(command, "mkDir" ) == 0 ){
		status = mkDir( fileName );

	} else if ( strcmp(command, "mkFile" ) == 0 ){
		status = mkFile( fileName );

	} else if ( strcmp(command, "writeFile" ) == 0 ){
		status = writeFile( fileName, buffer, *numBytes );

	} else if ( strcmp(command, "readFile" ) == 0 ){
		status = readFile( fileName, buffer, *numBytes );

	} else if ( strcmp(command, "printINodeInfo" ) == 0 ){
		printINodeInfo( fileName );

	} else if ( strcmp(command, "chDir" ) == 0 ){
		chDir( fileName );

	} else if ( strcmp(command, "exit" ) == 0 ){
		printf( "Goodbye, thanks for passing by...\n" );

	} else {
		printf( "Sorry, didn't get that. Please type again\n" );
	}

	return status;
}

/**
 * _initFS - helper function, initializes the file system
 *
 * @return: 0 on success, -1 on failure
 */
int _initFS(){
	DIR *fs;

	/* checks if the file system directory already exists */
	if ( ( fs = opendir( "filesystem" ) ) != NULL ){
		printf( "File system already exists!\n");
		closedir( fs );

		/* if the file system directory exists, initializes the globals */
		_initGlobals();

		return 0;
	}

	/* if the file system doesn't exist it creates it */
	if ( mkdir( "filesystem", 0700 ) == -1 ){
		printf( "Error creating filesystem folder\n");
		return -1;
	}

	/* creates the boot sector file */
	fileDescriptor bootsector =
		open( "filesystem/bootsector", O_RDWR | O_CREAT | O_APPEND, 0700 );

	if ( bootsector == -1 ){
		printf( "Error creating boot sector file\n");
		return -1;
	}

	/* creates the super block file */
	fileDescriptor superblock =
		open( "filesystem/superblock", O_RDWR | O_CREAT | O_APPEND, 0700 );

	if ( superblock == -1 ){
		printf( "Error creating super block file\n");
		return -1;
	}

	/* close the files */
	close(bootsector);
	close(superblock);

	char *tmpNameD = (char *) malloc( 256 );
	char *tmpNameD2 = (char *) malloc( 512 );
	char *tmpNameF = (char *) malloc( 1024 );

	/* create all other directories and files */
	for ( int i = 0; i < CYL_BLOCK_NUM; i++ ){

		/* Cylinder block directory */
		snprintf(tmpNameD, 255, "filesystem/cylinderBlock_%d", i);
		if ( mkdir( tmpNameD, 0700 ) == -1 ){
			printf( "Error creating cylinder block folder\n");
			return -1;
		}

		/* Super block copy file */
		snprintf(tmpNameF, 1023, "%s/superblockcopy", tmpNameD);
		fileDescriptor superblockcopy =
			open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

		if ( superblockcopy == -1 ){
			printf( "Error creating super block copy file\n");
			return -1;
		}

		close(superblockcopy);

		/* Cylinder group info file */
		snprintf(tmpNameF, 1023, "%s/cylindergroupinfo", tmpNameD);

		fileDescriptor cylindergroupinfo =
			open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

		if ( cylindergroupinfo == -1 ){
			printf( "Error creating cylinder group info file\n");
			return -1;
		}

		close(cylindergroupinfo);

		/* I-node bitmap file. Shows which i-nodes are free */
		snprintf(tmpNameF, 1023, "%s/inodebitmap", tmpNameD);

		fileDescriptor inodebitmap =
			open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

		if ( inodebitmap == -1 ){
			printf( "Error creating inode bitmap file\n");
			return -1;
		}

		/* Initializes the inode bitmap with 0s */
		for ( int i = 0; i < NUM_OF_INODES-1; i++ ){
			write( inodebitmap, "0\n", 2);
		}

		/* adds the last 0 without a new line */
		write( inodebitmap, "0", 1);

		close(inodebitmap);

		/* Block bitmap file. Shows which blocks are free */
		snprintf(tmpNameF, 1023, "%s/blockbitmap", tmpNameD);
		fileDescriptor datablockbitmap =
			open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

		if ( datablockbitmap == -1 ){
			printf( "Error creating block bitmap file\n");
			return -1;
		}

		/* Initialize the data block bitmap with 0s */
		for ( int i = 0; i < NUM_OF_BLOCKS-1; i++ ){
			write( datablockbitmap, "0\n", 2);
		}

		/* adds the last 0 without a new line */
		write( datablockbitmap, "0", 1);

		close(datablockbitmap);

		/* I-node array file. The actual i-nodes */
		snprintf(tmpNameF, 1023, "%s/inodearray", tmpNameD);

		fileDescriptor inodearray =
			open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

		if ( inodearray == -1 ){
			printf( "Error creating inode array file\n" );
			return -1;
		}
		write( inodearray, "?\n", 2 );
		close(inodearray);

		/* Block array folder. Where the actual data blocks are */
		snprintf(tmpNameD2, 511, "%s/datablocks", tmpNameD);

		if ( mkdir( tmpNameD2, 0700 ) == -1 ){
			printf( "Error creating data block folder\n" );
			return -1;
		}

		/* Initializes a string to the data block size to
		 * use for the data block initialization later */
		char *my0Char = (char *) malloc( BLOCK_SIZE );

		for ( int j = 0; j < BLOCK_SIZE; j++ )
			my0Char[j] = '0';

		/* Data block files. The actual user data initialized with
		 * the string from above */
		for ( int i = 0; i < NUM_OF_BLOCKS; i++ ){
			snprintf(tmpNameF, 1023, "%s/datablock_%d", tmpNameD2, i);

			fileDescriptor datablock =
				open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

			if ( datablock == -1 ){
				printf( "Error creating data block %s file\n", tmpNameF );
				return -1;
			}
			write( datablock, my0Char, BLOCK_SIZE);
			close(datablock);
		}

		/* Indirect pointer blocks folder. */
		snprintf(tmpNameD2, 511, "%s/indirectptrblocks", tmpNameD);

		if ( mkdir( tmpNameD2, 0700 ) == -1 ){
			printf( "Error creating indirect pointer block folder\n" );
			return -1;
		}

		/* Indirect pointer files. Each inode has 2 indirect pointers,
		 * each one pointing to BLOCK_SIZE/2 direct pointers */
		for ( int i = 0; i < NUM_OF_INODES; i++ ){
			snprintf(tmpNameF, 1023, "%s/indirectptrblock_%d", tmpNameD2, i);
			fileDescriptor indirectptrblock =
				open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );

			if ( indirectptrblock == -1 ){
				printf(
					"Error creating indeirect pointer block %s file\n",
					tmpNameF
				);
				return -1;
			}

			write( indirectptrblock, my0Char, INODE_INDIRECT_PTRS*2);
			close(indirectptrblock);
		}

		free(my0Char);

		if ( mkDir("~") == -1 ){
			printf( "Error creating HOME directory\n");
			return -1;
		}
	}

	free(tmpNameD);
	free(tmpNameD2);
	free(tmpNameF);
	return 0;
}

/**
 * _updateSuperblock - stores file system info inside the superblock
 *
 * @disksize:          size of the disk in bytes
 * @cylblocknum:       number of cylinder blocks
 * @blocksize:         size of each block in bytes
 * @blocknum:          number of blocks
 * @inodenum:          number of inodes
 * @inodeptrarraysize: number of pointers in the inode data block array
 * @inodeindirectptrs: number of indirect pointers per inode
 * @nodeidx:           number of inodes currently in use
 *
 * @return: 0 on success, -1 if the superblock can't be opened
 */
int _updateSuperblock(
		int disksize,
		int cylblocknum,
		int blocksize,
		int blocknum,
		int inodenum,
		int inodeptrarraysize,
		int inodeindirectptrs,
		int nodeidx
		){
	FILE *superblock = fopen( "filesystem/superblock", "r+" );

	if ( superblock == NULL ){
		printf( "Error opening super block file\n");
		return -1;
	}

	fprintf( superblock, "%d\n", disksize );
	fprintf( superblock, "%d\n", cylblocknum );
	fprintf( superblock, "%d\n", blocksize );
	fprintf( superblock, "%d\n", blocknum );
	fprintf( superblock, "%d\n", inodenum );
	fprintf( superblock, "%d\n", inodeptrarraysize );
	fprintf( superblock, "%d\n", inodeindirectptrs );
	fprintf( superblock, "%d\n", nodeidx );

	fclose(superblock);
	return 0;
}

/**
 * _initGlobals - fetches the values for global variables from the super block
 *
 * @return: 0 on success, -1 if the superblock can't be opened
 */
int _initGlobals(){
	FILE *superblock = fopen( "filesystem/superblock", "r+" );

	if ( superblock == NULL ){
		printf( "Error opening super block file\n");
		return -1;
	}

	char buffer[128];
	char *error;

	fgets( buffer, sizeof(buffer), superblock );
	DISK_SIZE = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	CYL_BLOCK_NUM = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	BLOCK_SIZE = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	NUM_OF_BLOCKS = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	NUM_OF_INODES = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	INODE_PTR_ARRAY_SIZE = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	INODE_INDIRECT_PTRS = (int) strtol( buffer, &error, 10 );
	fgets( buffer, sizeof(buffer), superblock );
	inodeidx = (int) strtol( buffer, &error, 10 );

	fclose(superblock);
	return 0;
}

/**
 * _loadFile - reads a whole file into memory
 *
 * @path: path of the file to read
 * @size: set to the number of bytes read
 *
 * @return: the contents with a '\0' at the end, which the caller has to
 *			free, or NULL on failure
 */
char *_loadFile(const char *path, int *size){
	fileDescriptor fd = open( path, O_RDONLY );
	if ( fd == -1 ){
		return NULL;
	}

	/* moving to the end of the file gives its size */
	off_t end = lseek( fd, 0, SEEK_END );
	if ( end == -1 || lseek( fd, 0, SEEK_SET ) == -1 ){
		close( fd );
		return NULL;
	}

	/* + 1 for the '\0', so the string functions know where it ends */
	char *contents = (char *) malloc( end + 1 );
	if ( contents == NULL ){
		close( fd );
		return NULL;
	}

	if ( read( fd, contents, end ) != end ){
		free( contents );
		close( fd );
		return NULL;
	}

	close( fd );
	contents[end] = '\0';
	*size = (int) end;
	return contents;
}

/**
 * _replaceInFile - replaces part of a file's contents and saves the file
 *
 * @path:     path of the file to save
 * @contents: current contents of the file
 * @size:     size of contents
 * @start:    where the replaced part starts in contents
 * @oldLen:   length of the replaced part
 * @newText:  text that takes its place
 *
 * @return: 0 on success, -1 on failure
 */
int _replaceInFile(
		const char *path,
		const char *contents,
		int size,
		int start,
		int oldLen,
		const char *newText
		){
	int newLen = (int) strlen( newText );
	int after = size - start - oldLen;  /* bytes after the replaced part */
	int newSize = start + newLen + after;

	/* if newText is longer or shorter than the old part, everything after
	 * it moves, so the whole file is rebuilt instead of written in place */
	char *updated = (char *) malloc( newSize );
	if ( updated == NULL ){
		return -1;
	}
	memcpy( updated, contents, start );
	memcpy( updated + start, newText, newLen );
	memcpy( updated + start + newLen, contents + start + oldLen, after );

	/* O_TRUNC empties the file first, so a file that got shorter doesn't
	 * keep old bytes at its end */
	int status = -1;
	fileDescriptor fd = open( path, O_WRONLY | O_TRUNC );
	if ( fd != -1 ){
		if ( write( fd, updated, newSize ) == newSize ){
			status = 0;
		}
		close( fd );
	}

	free( updated );
	return status;
}

/**
 * _findInodeLine - finds an inode's line inside the inode array
 *
 * @contents: contents of the inode array file
 * @name:     file name of the inode, the 1st field on its line
 *
 * @return: start of the line, or NULL if there is no such inode
 */
char *_findInodeLine(char *contents, const char *name){
	int nameLen = (int) strlen( name );
	char *line = contents;

	while ( line != NULL && *line != '\0' ){
		/* the whole 1st field has to match, so "ab" doesn't match "abc" */
		if ( strncmp( line, name, nameLen ) == 0
				&& ( line[nameLen] == ' ' || line[nameLen] == '\n'
					|| line[nameLen] == '\0' ) ){
			return line;
		}

		/* moves to the start of the next line */
		line = strchr( line, '\n' );
		if ( line != NULL ){
			line++;
		}
	}
	return NULL;
}

/**
 * _findField - finds one field on an inode line
 *
 * @line:  start of the inode line
 * @field: position of the field, 0 is the file name
 * @len:   set to the length of the field
 *
 * @return: start of the field, or NULL if the line has no such field
 */
char *_findField(char *line, int field, int *len){
	for ( int i = 0; i < field; i++ ){
		line += strcspn( line, " \n" );  /* skips to the end of the field */
		if ( *line != ' ' ){
			return NULL;                 /* the line ended first */
		}
		line++;                          /* skips the space */
	}

	*len = (int) strcspn( line, " \n" );
	if ( *len == 0 ){
		return NULL;
	}
	return line;
}

/**
 * _getInodeField - copies one field of an inode from the inode array
 *
 * @name:    file name of the inode
 * @field:   position of the field, 0 is the file name
 * @out:     set to the field's text
 * @outSize: size of out
 *
 * @return: 0 on success, -1 if the field isn't found or doesn't fit in out
 */
int _getInodeField(const char *name, int field, char *out, int outSize){
	int size;
	char *contents = _loadFile( INODE_ARRAY_PATH, &size );
	if ( contents == NULL ){
		return -1;
	}

	int len = 0;
	char *value = NULL;
	char *line = _findInodeLine( contents, name );
	if ( line != NULL ){
		value = _findField( line, field, &len );
	}

	int status = -1;
	if ( value != NULL && len < outSize ){
		memcpy( out, value, len );
		out[len] = '\0';
		status = 0;
	}

	free( contents );
	return status;
}

/**
 * _setInodeField - changes one field of an inode in the inode array
 *
 * @name:  file name of the inode
 * @field: position of the field, 0 is the file name
 * @value: new text of the field
 *
 * @return: 0 on success, -1 on failure
 */
int _setInodeField(const char *name, int field, const char *value){
	int size;
	char *contents = _loadFile( INODE_ARRAY_PATH, &size );
	if ( contents == NULL ){
		return -1;
	}

	int len = 0;
	char *start = NULL;
	char *line = _findInodeLine( contents, name );
	if ( line != NULL ){
		start = _findField( line, field, &len );
	}

	/* the new value can be a different length than the old one, e.g. a
	 * hard link number going from 9 to 10, so _replaceInFile moves the
	 * rest of the file to keep it aligned */
	int status = -1;
	if ( start != NULL ){
		status = _replaceInFile(
			INODE_ARRAY_PATH,
			contents,
			size,
			(int) ( start - contents ),
			len,
			value
		);
	}

	free( contents );
	return status;
}

/**
 * _appendInode - writes a new inode on the free line of the inode array
 *
 * @node: inode to write
 *
 * @return: index of the inode's line, or -1 on failure
 */
int _appendInode(const INode *node){
	/* builds the inode line, every field is followed by a space */
	char line[4096];
	int len = snprintf(
		line,
		sizeof(line),
		"%s %d %c %d %d %d %d %d %d ",
		node->fileName,
		node->nodeID,
		node->fileType,
		node->fileSize,
		(int) node->ownerID,
		(int) node->groupID,
		node->permissions,
		node->hardLinkNumber,
		node->blockOffset
	);

	/* empty data array slots are written as NULL, the len check keeps
	 * sizeof(line) - len from going below 0 */
	for ( int i = 0; i < INODE_PTR_ARRAY_SIZE; i++ ){
		const char *ptr = node->dataBlockArray[i];
		if ( len < (int) sizeof(line) ){
			len += snprintf( line + len, sizeof(line) - len, "%s ",
					ptr == NULL ? "NULL" : ptr );
		}
	}

	/* ends the line and adds a new "?" line for the next inode */
	if ( len < (int) sizeof(line) ){
		len += snprintf( line + len, sizeof(line) - len, "\n?\n" );
	}

	/* snprintf returns the length it needed, so this means it didn't fit */
	if ( len >= (int) sizeof(line) ){
		return -1;
	}

	int size;
	char *contents = _loadFile( INODE_ARRAY_PATH, &size );
	if ( contents == NULL ){
		return -1;
	}

	/* the "?" line marks where the next inode goes */
	char *freeLine = _findInodeLine( contents, "?" );
	if ( freeLine == NULL ){
		free( contents );
		return -1;
	}

	/* the new inode's index is the number of lines before it */
	int start = (int) ( freeLine - contents );
	int index = 0;
	for ( int i = 0; i < start; i++ ){
		if ( contents[i] == '\n' ){
			index++;
		}
	}

	/* replaces everything from the "?" line on with the new lines */
	int status = _replaceInFile(
		INODE_ARRAY_PATH,
		contents,
		size,
		start,
		size - start,
		line
	);

	free( contents );
	if ( status == -1 ){
		return -1;
	}
	return index;
}

/**
 * _findFreeBitmapLine - finds the first free (0) line of a bitmap file
 *
 * @path: path of the bitmap file
 *
 * @return: index of the line, or -1 if every line is in use
 */
int _findFreeBitmapLine(const char *path){
	int size;
	char *contents = _loadFile( path, &size );
	if ( contents == NULL ){
		return -1;
	}

	/* every line is one character and a '\n', so line i is at byte 2 * i */
	int index = -1;
	for ( int i = 0; i < size; i += 2 ){
		if ( contents[i] == '0' ){
			index = i / 2;
			break;
		}
	}

	free( contents );
	return index;
}

/**
 * _setBitmapLine - changes one line of a bitmap file
 *
 * @path:  path of the bitmap file
 * @index: index of the line
 * @value: '1' for in use, '0' for free
 *
 * @return: 0 on success, -1 on failure
 */
int _setBitmapLine(const char *path, int index, char value){
	fileDescriptor fd = open( path, O_WRONLY );
	if ( fd == -1 ){
		return -1;
	}

	/* line i is at byte 2 * i, and the value is a single character, so it
	 * is overwritten in place without moving the rest of the file */
	int status = -1;
	off_t size = lseek( fd, 0, SEEK_END );
	if ( index >= 0 && 2 * index < size
			&& lseek( fd, 2 * index, SEEK_SET ) != -1
			&& write( fd, &value, 1 ) == 1 ){
		status = 0;
	}

	close( fd );
	return status;
}

/**
 * _addHardLink - adds a hard link to a new inode in the parent directory's
 *				  data block
 *
 * @parentName: file name of the parent directory
 * @childName:  file name of the new inode
 * @childID:    nodeID of the new inode
 *
 * @return: 0 on success, -1 on failure
 */
int _addHardLink(const char *parentName, const char *childName, int childID){
	/* the hard link is a line with the new inode's name and nodeID */
	char entry[256];
	int entryLen =
		snprintf( entry, sizeof(entry), "%s %d\n", childName, childID );
	if ( entryLen >= (int) sizeof(entry) ){
		return -1;
	}

	/* blockOffset is where the free space starts in the last data block */
	char value[32];
	if ( _getInodeField( parentName, FIELD_BLOCK_OFFSET, value,
			sizeof(value) ) == -1 ){
		return -1;
	}
	int offset = atoi( value );

	/* finds the last data block the parent uses and its first empty slot,
	 * slots are filled in order, so the last block is right before it */
	int block = -1;
	int freeSlot = -1;
	for ( int slot = FIRST_BLOCK_SLOT; slot <= LAST_BLOCK_SLOT; slot++ ){
		if ( _getInodeField( parentName, FIELD_DATA_ARRAY + slot, value,
				sizeof(value) ) == -1 ){
			return -1;
		}

		if ( strcmp( value, "NULL" ) == 0 ){
			freeSlot = slot;
			break;
		}
		block = atoi( value );
	}

	/* the parent needs a new data block if it has none yet, or if the
	 * entry doesn't fit in the space left in its last one */
	int newBlock = ( block == -1 || offset + entryLen > BLOCK_SIZE );
	if ( newBlock ){
		if ( freeSlot == -1 ){
			printf( "No direct pointer available\n" );
			return -1;
		}

		block = _findFreeBitmapLine( BLOCK_BITMAP_PATH );
		if ( block == -1 ){
			printf( "No data blocks available\n" );
			return -1;
		}
		offset = 0;
	}

	/* a data block file always holds BLOCK_SIZE characters, so the entry
	 * overwrites the '0' filler at the offset without moving anything */
	char path[256];
	snprintf( path, sizeof(path), DATA_BLOCK_PATH, block );

	fileDescriptor datablock = open( path, O_WRONLY );
	if ( datablock == -1 ){
		return -1;
	}

	int written = -1;
	if ( lseek( datablock, offset, SEEK_SET ) != -1 ){
		written = (int) write( datablock, entry, entryLen );
	}
	close( datablock );

	if ( written != entryLen ){
		return -1;
	}

	/* marks a new block as used and connects it to the parent's data array */
	if ( newBlock ){
		snprintf( value, sizeof(value), "%d", block );
		int slotField = FIELD_DATA_ARRAY + freeSlot;

		if ( _setBitmapLine( BLOCK_BITMAP_PATH, block, '1' ) == -1 ){
			return -1;
		}
		if ( _setInodeField( parentName, slotField, value ) == -1 ){
			return -1;
		}
	}

	/* moves the parent's blockOffset past the new entry */
	snprintf( value, sizeof(value), "%d", offset + entryLen );
	return _setInodeField( parentName, FIELD_BLOCK_OFFSET, value );
}

/**
 * mkDir - creates a directory in the current working directory
 *
 * @dirname: name of the new directory
 *
 * @return: 0 on success, -1 on failure
 */
int mkDir(char *dirname){
	int isHome = ( strcmp( dirname, "~" ) == 0 );

	/* "?" and "NULL" have special meanings in the inode array */
	if ( dirname[0] == '\0' || strlen( dirname ) > MAX_NAME_LEN
			|| strcmp( dirname, "?" ) == 0 || strcmp( dirname, "NULL" ) == 0 ){
		printf( "Invalid directory name\n" );
		return -1;
	}

	/* checks if there are available inodes in the filesystem */
	if ( inodeidx >= NUM_OF_INODES ){
		printf( "No inodes available...\n" );
		return -1;
	}

	/* inodes are looked up by name, so the name can't be in use already */
	char type[2];
	if ( _getInodeField( dirname, FIELD_TYPE, type, sizeof(type) ) == 0 ){
		printf( "%s already exists\n", dirname );
		return -1;
	}

	/* the parent has to be an existing directory */
	if ( !isHome ){
		int found = _getInodeField(
			workingDirectory,
			FIELD_TYPE,
			type,
			sizeof(type)
		);
		if ( found == -1 || type[0] != 'd' ){
			printf( "Parent directory %s not found\n", workingDirectory );
			return -1;
		}
	}

	/* ----- Create the new inode ----- */
	INode newNode;
	newNode.fileName = dirname;
	newNode.nodeID = inodeidx;
	newNode.fileType = 'd';
	newNode.fileSize = 0;
	newNode.ownerID = getuid();
	newNode.groupID = getgid();
	newNode.permissions = 6;
	newNode.hardLinkNumber = 1;
	newNode.blockOffset = 0;

	for ( int i = 0; i < INODE_PTR_ARRAY_SIZE; i++ )
		newNode.dataBlockArray[i] = NULL;

	/* slot 0 holds the parent directory and slot 1 the directory itself,
	 * the home directory is its own parent */
	newNode.dataBlockArray[0] = isHome ? dirname : workingDirectory;
	newNode.dataBlockArray[1] = dirname;

	/* ----- Add the hard link in the parent's data block ----- */
	/* done before anything else is written, so if the parent is full the
	 * filesystem is left unchanged */
	if ( !isHome ){
		if ( _addHardLink( workingDirectory, dirname, newNode.nodeID ) == -1 ){
			printf( "Error adding the hard link to %s\n", workingDirectory );
			return -1;
		}
	}

	/* ----- Add the new inode to the inode array ----- */
	int index = _appendInode( &newNode );
	if ( index == -1 ){
		printf( "Error writing the inode array file\n" );
		return -1;
	}
	inodeidx++;

	/* ----- Mark the new inode as used in the inode bitmap ----- */
	if ( _setBitmapLine( INODE_BITMAP_PATH, index, '1' ) == -1 ){
		printf( "Error updating the inode bitmap file\n" );
		return -1;
	}

	/* the home directory has no parent, so it's done here */
	if ( isHome ){
		return 0;
	}

	/* ----- Increase the parent's hard link number ----- */
	/* the new directory's slot 0 links back to its parent, like ".." */
	char links[16];
	int status = _getInodeField(
		workingDirectory,
		FIELD_HARD_LINKS,
		links,
		sizeof(links)
	);
	if ( status == 0 ){
		snprintf( links, sizeof(links), "%d", atoi( links ) + 1 );
		status = _setInodeField( workingDirectory, FIELD_HARD_LINKS, links );
	}
	if ( status == -1 ){
		printf( "Error updating the hard links of %s\n", workingDirectory );
		return -1;
	}

	return 0;
}

/**
 * mkFile - creates a file in the current working directory
 *
 * @fileName: name of the new file
 *
 * @return: 0 on success, -1 on failure
 */
int mkFile(char *fileName){
	/* "?" and "NULL" have special meanings in the inode array */
	if ( fileName[0] == '\0' || strlen( fileName ) > MAX_NAME_LEN
			|| strcmp( fileName, "?" ) == 0 || strcmp( fileName, "NULL" ) == 0 ){
		printf( "Invalid file name\n" );
		return -1;
	}

	/* checks if there are available inodes in the filesystem */
	if ( inodeidx >= NUM_OF_INODES ){
		printf( "No inodes available...\n" );
		return -1;
	}

	/* inodes are looked up by name, so the name can't be in use already */
	char type[2];
	if ( _getInodeField( fileName, FIELD_TYPE, type, sizeof(type) ) == 0 ){
		printf( "%s already exists\n", fileName );
		return -1;
	}

	/* the parent has to be an existing directory */
	int found = _getInodeField(
		workingDirectory,
		FIELD_TYPE,
		type,
		sizeof(type)
	);
	if ( found == -1 || type[0] != 'd' ){
		printf( "Parent directory %s not found\n", workingDirectory );
		return -1;
	}

	/* ----- Create the new inode ----- */
	INode newNode;
	newNode.fileName = fileName;
	newNode.nodeID = inodeidx;
	newNode.fileType = 'f';
	newNode.fileSize = 0;
	newNode.ownerID = getuid();
	newNode.groupID = getgid();
	newNode.permissions = 6;
	newNode.hardLinkNumber = 1;
	newNode.blockOffset = 0;

	/* unlike a directory, a file has no parent or self entry in slots 0
	 * and 1, and a new file has no data yet, so every slot starts empty */
	for ( int i = 0; i < INODE_PTR_ARRAY_SIZE; i++ )
		newNode.dataBlockArray[i] = NULL;

	/* ----- Add the hard link in the parent's data block ----- */
	/* done before anything else is written, so if the parent is full the
	 * filesystem is left unchanged */
	if ( _addHardLink( workingDirectory, fileName, newNode.nodeID ) == -1 ){
		printf( "Error adding the hard link to %s\n", workingDirectory );
		return -1;
	}

	/* ----- Add the new inode to the inode array ----- */
	int index = _appendInode( &newNode );
	if ( index == -1 ){
		printf( "Error writing the inode array file\n" );
		return -1;
	}
	inodeidx++;

	/* ----- Mark the new inode as used in the inode bitmap ----- */
	if ( _setBitmapLine( INODE_BITMAP_PATH, index, '1' ) == -1 ){
		printf( "Error updating the inode bitmap file\n" );
		return -1;
	}

	/* the parent's hard link number stays the same, only a subdirectory
	 * adds one, because its slot 0 links back to the parent like ".." */
	return 0;
}

/**
 * writeFile - writes numBytes from a buffer in a file
 *
 * @fileName: name of the file to write to
 * @buffer:   data to write
 * @numBytes: number of bytes to write from the buffer
 *
 * @return: 0 on success, -1 on failure
 */
int writeFile(char *fileName, char *buffer, int numBytes){
	write(
		STDOUT_FILENO,
		"Under Construction! Come back later, sorry...\n",
		46
	);
	return 0;
}

/**
 * readFile - takes a filename, buffer, and a number of bytes and goes through
 *			  the file to read byte by byte, as well as update the buffer
 *
 * @fileName: name of the specified file
 * @buffer:   memory that the bytes read from the file are copied into
 * @numBytes: number of bytes to read
 *
 * @return: 0 on success, -1 on failure
 */
int readFile(char *fileName, char *buffer, int numBytes){
	write(
		STDOUT_FILENO,
		"Under Construction! Come back later, sorry...\n",
		46
	);
	return 0;
}

/**
 * printINodeInfo - prints the inode info on screen, something like the
 *					ls -l command
 *
 * @fileName: the specified file name to print the information of
 *
 * @return: void
 */
void printINodeInfo(char *fileName){
	/* "?" marks the free line of the inode array, it isn't an inode */
	if ( fileName[0] == '\0' || strlen( fileName ) > MAX_NAME_LEN
			|| strcmp( fileName, "?" ) == 0 ){
		printf( "Invalid file name\n" );
		return;
	}

	int size;
	char *contents = _loadFile( INODE_ARRAY_PATH, &size );
	if ( contents == NULL ){
		printf( "Error reading the inode array file\n" );
		return;
	}

	char *line = _findInodeLine( contents, fileName );
	if ( line == NULL ){
		printf( "%s: no such file or directory\n", fileName );
		free( contents );
		return;
	}

	/* finds every field of the line once. The fields aren't '\0'
	 * terminated, so they are printed with "%.*s", which prints only
	 * len characters of the string */
	char *field[NUM_OF_FIELDS];
	int len[NUM_OF_FIELDS];
	for ( int i = 0; i < NUM_OF_FIELDS; i++ ){
		field[i] = _findField( line, i, &len[i] );
		if ( field[i] == NULL ){
			printf( "The inode of %s is missing fields\n", fileName );
			free( contents );
			return;
		}
	}

	/* atoi stops at the space after a number, so it can read the fields */
	char type = field[FIELD_TYPE][0];
	int ownerID = atoi( field[FIELD_OWNER] );
	int groupID = atoi( field[FIELD_GROUP] );
	int permissions = atoi( field[FIELD_PERMISSIONS] );

	/* the names are looked up like ls -l does, the user or group could have
	 * been deleted since the inode was made, so they can be missing */
	struct passwd *ownerEntry = getpwuid( (uid_t) ownerID );
	struct group *groupEntry = getgrgid( (gid_t) groupID );
	const char *ownerName =
		( ownerEntry != NULL ) ? ownerEntry->pw_name : "unknown";
	const char *groupName =
		( groupEntry != NULL ) ? groupEntry->gr_name : "unknown";

	/* the mode is shown like ls -l, our permissions are for the user only,
	 * so the group and others parts are always "---" */
	char mode[11];
	snprintf(
		mode,
		sizeof(mode),
		"%c%c%c%c------",
		( type == 'd' ) ? 'd' : '-',
		( permissions & 4 ) ? 'r' : '-',
		( permissions & 2 ) ? 'w' : '-',
		( permissions & 1 ) ? 'x' : '-'
	);

	/* ----- The ls -l line ----- */
	printf(
		"%s %.*s %s %s %.*s %s\n",
		mode,
		len[FIELD_HARD_LINKS], field[FIELD_HARD_LINKS],
		ownerName,
		groupName,
		len[FIELD_SIZE], field[FIELD_SIZE],
		fileName
	);

	/* ----- Every field of the inode ----- */
	printf( "  nodeID:         %.*s\n",
			len[FIELD_NODE_ID], field[FIELD_NODE_ID] );
	printf( "  fileType:       %c (%s)\n",
			type, ( type == 'd' ) ? "directory" : "file" );
	printf( "  fileSize:       %.*s bytes\n",
			len[FIELD_SIZE], field[FIELD_SIZE] );
	printf( "  ownerID:        %d (%s)\n", ownerID, ownerName );
	printf( "  groupID:        %d (%s)\n", groupID, groupName );
	printf( "  permissions:    %d (%.3s)\n", permissions, mode + 1 );
	printf( "  hardLinkNumber: %.*s\n",
			len[FIELD_HARD_LINKS], field[FIELD_HARD_LINKS] );
	printf( "  blockOffset:    %.*s\n",
			len[FIELD_BLOCK_OFFSET], field[FIELD_BLOCK_OFFSET] );

	/* each slot is labeled with what it's used for, the same layout that
	 * mkDir and _addHardLink use */
	printf( "  dataBlockArray:\n" );
	for ( int slot = 0; slot < NUM_OF_SLOTS; slot++ ){
		const char *use = "direct";
		if ( slot > LAST_BLOCK_SLOT ){
			use = "indirect";
		} else if ( type == 'd' && slot == 0 ){
			use = "parent";
		} else if ( type == 'd' && slot == 1 ){
			use = "self";
		}

		int i = FIELD_DATA_ARRAY + slot;
		printf( "    [%2d] %-9s%.*s\n", slot, use, len[i], field[i] );
	}

	free( contents );
}

/**
 * chDir - takes a dirPath and changes the current working directory to that
 *		   specific directory
 *
 * @dirPath: specified dirpath to take
 *
 * @return: void
 */
void chDir(char *dirPath){
	write(
		STDOUT_FILENO,
		"Under Construction! Come back later, sorry...\n",
		46
	);
}
