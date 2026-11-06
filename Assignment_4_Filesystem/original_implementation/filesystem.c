#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include "string.h"

// ----- Some type definitions to make things easy to write and unterstand -------//
typedef struct Filesystem Filesystem;
typedef struct Bootsector Bootsector;
typedef struct Superblock Superblock;
typedef struct Cylindergroup Cylindergroup;
typedef struct Cylindergroupinfo Cylindergroupinfo;
typedef struct Inodebitmap Inodemap;
typedef struct Blockbitmap Blockbitmap;
typedef struct Inodearray Inodearray;
typedef struct Blockarray Blockarray;
typedef struct INode INode;
typedef int fileDescriptor;

// ----- Some globals to reduce the parameters we pass around -------//
int DISK_SIZE = 1048576;                // set the disk size for your file system
int CYL_BLOCK_NUM = 1;                  // this is here to match the slides. This functionality is not supported by the back-end, so don't change it
int BLOCK_SIZE = 1024;                  // set the size of your blocks
int BLOCK_NUM = 1024;                   // the number of the blocks based on the disk and block sizes DISK_SIZE / BLOCK_SIZE
int INODE_NUM = 1024 * 0.1;             // the number of inodes, 10% of block number BLOCK_NUM * 0.1
int INODE_PTR_ARRAY_SIZE = 14;          // even though this is available, it cannot be used in the inode struct because of the std=c99. Set it manually
int INODE_INDIRECT_PTRS = 1024 / 2;     // the amount of indirect pointers pointed by the last two elements in the inode data block array. BLOCK_SIZE / 2 so each inode uses one file for both of its indirect elements 
int inodeidx = 0;                // keeps track how many inodes are currently in use. This needs to be recalculated in each session from the inode bitmap. It's faster to do it once at the beginning of the session than calculate it each time an inode needs to be created
char* workingDirectory = "~";    // keeps track of the current working directory. The PWD variable in linux. This needs to be updated every time the user navigates to a new dictory

// ----- Function prototypes -------//
int mkDir(char* dirName);       // creates a directory
int mkFile(char* fileName);     // creates a file
int writeFile(char* fileName, char* buffer, int numBytes);    // writes numBytes from the buffer in a file
int readFile(char* fileName, char* buffer, int numBytes);     // reads numBytes from a file into the buffer
void printINodeInfo( char* fileName );  // prints the inode info on the screen, something like the ls -l command
void chDir( char* dirPath );            // changes the current working directory, something like the cd command
int _initFS();                   // initializes the file system
void _processUserInput( char* userInput, int inputBytes, char* command, char* fileName, char* buffer, int* numBytes );  // processes the user input
int _executeCommand( char* command, char* fileName, char* buffer, int* numBytes );
int _updateSuperblock( int disksize, int cylblocknum, int blocksize, int blocknum, int inodenum, int inodeptrarraysize, int inodeindirectptrs, int nodeidx );
int _initGlobals();


int main( int argc, char* argv[])
{
    // welcomes the user, prints specs and instructions 
    printf( "!!--> File System Specs <--!!\n" );
    printf( "Disk size: %d\n", DISK_SIZE );
    printf( "Cylinder block number: %d\n", CYL_BLOCK_NUM );
    printf( "Block size: %d\n", BLOCK_SIZE );
    printf( "Block number: %d\n", BLOCK_NUM );
    printf( "I-node number: %d\n", INODE_NUM );
    printf( "I-node indirect pointer number: %d\n", INODE_INDIRECT_PTRS );
    printf( "------------------------------------------------------------------------\n\n" );

    printf( "Welcome to the CSC2050 File System!\n" );
    printf( "What would you like to do?\n" );
    printf( "Available choices are:\n" );
    printf( "\tmkDir char* dirName                                  -> to create a directory\n" );
    printf( "\tmkFile char* fileName                                -> to create a file\n" );
    printf( "\twriteFile char* fileName, char* buffer, int numBytes -> to write numBytes from the buffer in a file\n" );
    printf( "\treadFile char* fileName, char* buffer, int numBytes  -> to read numBytes from a file into the buffer\n" );
    printf( "\tprintINodeInfo char* fileName                        -> to print the inode info on the screen\n" );
    printf( "\tchDir char* dirPath                                  -> to change the current working directory\n" );
    printf( "\texit                                                 -> to exit the filesystem\n" );

    // Create the filesystem structure and creates the home directory
    if ( _initFS() == -1 ) { printf( "Error creating file system\n"); return -1; }

    char command[32]; char fileName[128]; char buffer[128]; int numBytes;     // variables to hold
    char userInput[512]; int inputBytes; int status = 0;                      // the userinput

    do
    {
        // resets the userinput state for every input
        numBytes = 0; inputBytes = 0;       

        // print the prompt symbol
        write( STDOUT_FILENO, ":> ", 3 );   
        
        // reads the userinput
        inputBytes = read( STDIN_FILENO, userInput, 511 );  

        // checks for empty input
        if( inputBytes == 0 ) { printf( "Sorry, didn't get that. Please type again\n" ); continue; }

        // processes the userinput to extract the tokens for each command. It can be replaces by strtok()
        _processUserInput( userInput, inputBytes, command, fileName, buffer, &numBytes );

        // selects and calls the commands. Returns -1 on error 0 on success
        status = _executeCommand( command, fileName, buffer, &numBytes );
        if( status == -1 ) { printf("Command failed\n"); }

    } while ( strcmp( command, "exit") != 0 );

    // updates the superblock before exiting the program
    _updateSuperblock( DISK_SIZE, CYL_BLOCK_NUM, BLOCK_SIZE, BLOCK_NUM, INODE_NUM, INODE_PTR_ARRAY_SIZE, INODE_INDIRECT_PTRS, inodeidx );
    return status;
}

struct INode
{
    char* fileName;
    int nodeID;
    char fileType;              // f for files, d for directories
    int fileSize;
    uid_t ownerID;
    gid_t groupID;
    int permissions;            // for the user only. Default is 6 (read & write)
    int hardLinkNumber;         // number of hard links pointing to the inode
    int blockOffset;            // data block offset for semi-filled data blocks
    char* dataBlockArray[14];   // 14 element data block array. The last 2 are indirect data block pointers
};

void _processUserInput( char* userInput, int inputBytes, char* command, char* fileName, char* buffer, int* numBytes )
{
    int userInputIdx = 0;       // index for the userinput char array
    int variableIdx = 0;        // index for each variable
    char tmpChar = userInput[userInputIdx];

    // first loop processes the command
    // reads characters from the beginning until it finds a line change or a space character
    while( tmpChar != '\n' && tmpChar != ' ' )
    {
        command[variableIdx] = tmpChar;         // stores every character in the command variable
        userInputIdx++; variableIdx++;          // updates the indices
        tmpChar = userInput[userInputIdx];      // updates to the next character
    }
    command[userInputIdx]= '\0';                // terminates the variable
    userInputIdx++;                             // updates the userinput index to the begining of the next token


    // second loop processes the filename if it exists
    if( userInputIdx < inputBytes)  // checks if the end of the line is reached the variable index
    {
        tmpChar = userInput[userInputIdx];          // updates to the next character
        variableIdx = 0;                            // resets the variable index

        while( tmpChar != '\n' && tmpChar != ' ' )
        {
            fileName[variableIdx] = tmpChar;
            userInputIdx++; variableIdx++;
            tmpChar = userInput[userInputIdx];
        }
        fileName[variableIdx]= '\0';
        userInputIdx++;                             // updates the userinput index to the begining of the next token

    }

    // third loop processes the buffer if it exists
    if( userInputIdx < inputBytes)
    {
        tmpChar = userInput[userInputIdx];          // updates to the next character
        variableIdx = 0;                            // resets the variable index

        while( tmpChar != '\n' && tmpChar != ' ' )
        {
            buffer[variableIdx] = tmpChar;
            userInputIdx++; variableIdx++;
            tmpChar = userInput[userInputIdx];
        }
        buffer[variableIdx]= '\0';
        userInputIdx++;                             // updates the userinput index to the begining of the next token

    }


    // forth loop processes the number of bytes if they exist
    if( userInputIdx < inputBytes)
    {
        tmpChar = userInput[userInputIdx];          // updates to the next character
        variableIdx = 0;                            // resets the variable index

        char numBytesbuffer[128];
        while( tmpChar != '\n' && tmpChar != ' ' )
        {
            numBytesbuffer[variableIdx] = tmpChar;
            userInputIdx++; variableIdx++;
            tmpChar = userInput[userInputIdx];
        }
        numBytesbuffer[variableIdx] = tmpChar;

        // converts numerical characters to integers
        int tmp = 1;
        *numBytes = 0;
        for( int i = 0; i < variableIdx; i++ )          // traverses through the string
        {
            tmp = 1;
            for( int j = i; j < variableIdx-1; j++)     // calculated the base multiplication 1, 10, 100, 1000
            {
                tmp *= 10;
            }

            // get the number by subtracting it's ASCII value from character '0'
            // then multiplies the number by it's base to get's it's numeric value
            // finally adds all the digits to get the actual number
            *numBytes += (numBytesbuffer[i] - '0') * tmp;
        }
    }
}

int _executeCommand( char* command, char* fileName, char* buffer, int* numBytes )
{
    int status = 0;
    if( strcmp(command, "mkDir" ) == 0 )
    {
        status = mkDir( fileName );
    }else if( strcmp(command, "mkFile" ) == 0 )
    {
        status = mkFile( fileName );

    } else if( strcmp(command, "writeFile" ) == 0 )
    {
        status = writeFile( fileName, buffer, *numBytes );

    } else if( strcmp(command, "readFile" ) == 0 )
    {
        status = readFile( fileName, buffer, *numBytes );
            
    } else if( strcmp(command, "printINodeInfo" ) == 0 )
    {
        printINodeInfo( fileName );
            
    } else if( strcmp(command, "chDir" ) == 0 )
    {
        chDir( fileName );
    } else if( strcmp(command, "exit" ) == 0 )
    {
        printf( "Goodbye, thanks for passing by...\n" );
    } else
    {
        printf( "Sorry, didn't get that. Please type again\n" );
    }
    return status;
}

// initializes the filesystem
int _initFS()
{
    DIR* fs;
    if ( ( fs = opendir( "filesystem" ) ) != NULL )    // checks if the file system directory already exists
    { 
        printf( "File system already exists!\n");
        closedir( fs );
        _initGlobals();     // if the file system directory exists, initializes the globals
        return 0;
    }      

    if ( mkdir( "filesystem", 0700 ) == -1 )    // if the file system doesn't exist it creates it
    { 
        printf( "Error creating filesystem folder\n");
        return -1;
    }      
        
    fileDescriptor bootsector = open( "filesystem/bootsector", O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( bootsector == -1 ) { printf( "Error creating boot sector file\n"); return -1; }        // creates the boot sector file
    fileDescriptor superblock = open( "filesystem/superblock", O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( superblock == -1 ) { printf( "Error creating super block file\n"); return -1; }        // creates the super block file

    // close the files
    close(bootsector);
    close(superblock);

    char* tmpNameD = (char*) malloc( 256 );
    char* tmpNameD2 = (char*) malloc( 512 );
    char* tmpNameF = (char*) malloc( 1024 );

    // create all other directories and files
    for( int i = 0; i < CYL_BLOCK_NUM; i++ )
    {
        // Cylinder block directory
        snprintf(tmpNameD, 255, "filesystem/cylinderBlock_%d", i);
        if ( mkdir( tmpNameD, 0700 ) == -1 ) { printf( "Error creating cylinder block folder\n"); return -1; } 

        // Super block copy file
        snprintf(tmpNameF, 1023, "%s/superblockcopy", tmpNameD);
        fileDescriptor superblockcopy = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( superblockcopy == -1 ) { printf( "Error creating super block copy file\n"); return -1; }  
        close(superblockcopy);

        // Cylinder group info file
        snprintf(tmpNameF, 1023, "%s/cylindergroupinfo", tmpNameD);
        fileDescriptor cylindergroupinfo = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( cylindergroupinfo == -1 ) { printf( "Error creating cylinder group info file\n"); return -1; }    
        close(cylindergroupinfo);

        // I-node bitmap file. Shows which i-nodes are free
        snprintf(tmpNameF, 1023, "%s/inodebitmap", tmpNameD);
        fileDescriptor inodebitmap = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( inodebitmap == -1 ) { printf( "Error creating inode bitmap file\n"); return -1; }    
        
        for ( int i = 0; i < INODE_NUM-1; i++ )
        {
            write( inodebitmap, "0\n", 2);  // Initializes the inode bitmap with 0s
        }
        write( inodebitmap, "0", 1);        // adds the last 0 without a new line
        
        close(inodebitmap);

        // Block bitmap file. Shows which blocks are free
        snprintf(tmpNameF, 1023, "%s/blockbitmap", tmpNameD);
        fileDescriptor datablockbitmap = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( datablockbitmap == -1 ) { printf( "Error creating block bitmap file\n"); return -1; }    
        
        for ( int i = 0; i < BLOCK_NUM-1; i++ )
        {
            write( datablockbitmap, "0\n", 2);  // Initialize the data block bitmap with 0s
        }
        write( datablockbitmap, "0", 1);        // adds the last 0 without a new line
        
        close(datablockbitmap);

        // I-node array file. The actual i-nodes
        snprintf(tmpNameF, 1023, "%s/inodearray", tmpNameD);
        fileDescriptor inodearray = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
        if ( inodearray == -1 ) { printf( "Error creating inode array file\n"); return -1; }    
        write( inodearray, "?\n", 2 );
        close(inodearray);

        // Block array folder. Where the actual data blocks are
        snprintf(tmpNameD2, 511, "%s/datablocks", tmpNameD);
        if ( mkdir( tmpNameD2, 0700 ) == -1 ) { printf( "Error creating data block folder\n"); return -1; }   

        // Initializes a string to the data block size to use for the data block initialization later
        char* my0Char = (char*) malloc( BLOCK_SIZE );
        for( int j = 0; j < BLOCK_SIZE; j++ )
        {
            my0Char[j] = '0';
        }

        // Data block files. The actual user data initialized with the string from above
        for ( int i = 0; i < BLOCK_NUM; i++ )
        {
            snprintf(tmpNameF, 1023, "%s/datablock_%d", tmpNameD2, i);
            fileDescriptor datablock = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
            if ( datablock == -1 ) { printf( "Error creating data block %s file\n", tmpNameF ); return -1; }   
            write( datablock, my0Char, BLOCK_SIZE);
            close(datablock);
        }

        // Indirect pointer blocks folder.
        snprintf(tmpNameD2, 511, "%s/indirectptrblocks", tmpNameD);
        if ( mkdir( tmpNameD2, 0700 ) == -1 ) { printf( "Error creating indirect pointer block folder\n"); return -1; }   

        // Indirect pointer files. Each inode has 2 indirect pointers, each one pointing to BLOCK_SIZE/2 direct pointers
        for ( int i = 0; i < INODE_NUM; i++ )
        {
            snprintf(tmpNameF, 1023, "%s/indirectptrblock_%d", tmpNameD2, i);
            fileDescriptor indirectptrblock = open( tmpNameF, O_RDWR | O_CREAT | O_APPEND, 0700 );
            if ( indirectptrblock == -1 ) { printf( "Error creating indeirect pointer block %s file\n", tmpNameF ); return -1; }   
            write( indirectptrblock, my0Char, INODE_INDIRECT_PTRS*2);
            close(indirectptrblock);
        }

        free(my0Char);

        if ( mkDir("~") == -1 ) { printf( "Error creating HOME directory\n"); return -1; }
    }

    free(tmpNameD);
    free(tmpNameD2);
    free(tmpNameF);
	return 0;
}

// stores file system info inside the superblock
int _updateSuperblock( int disksize, int cylblocknum, int blocksize, int blocknum, int inodenum, int inodeptrarraysize, int inodeindirectptrs, int nodeidx )
{
    FILE* superblock = fopen( "filesystem/superblock", "r+" );
    if ( superblock == NULL ) { printf( "Error opening super block file\n"); return -1; }

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

int _initGlobals()  // fetches the values for the global variables from the superblock
{
    FILE* superblock = fopen( "filesystem/superblock", "r+" );
    if ( superblock == NULL ) { printf( "Error opening super block file\n"); return -1; }

    char buffer[128]; char* error;

    fgets( buffer, sizeof(buffer), superblock );
    DISK_SIZE = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    CYL_BLOCK_NUM = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    BLOCK_SIZE = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    BLOCK_NUM = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    INODE_NUM = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    INODE_PTR_ARRAY_SIZE = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    INODE_INDIRECT_PTRS = (int) strtol( buffer, &error, 10 );
    fgets( buffer, sizeof(buffer), superblock );
    inodeidx = (int) strtol( buffer, &error, 10 );

    fclose(superblock); 
    return 0;   
}

int mkDir(char* dirName)
{
    // Checking if there are available inodes in the filesystem
    if( inodeidx == INODE_NUM ) { write( 1, "No inodes available...\n", 23 ); return -1;} 

    //---------- Create the new inode ---------------//
    INode* newNode = (INode*) malloc( sizeof(INode) );
    if( newNode == NULL ) { write( STDOUT_FILENO, "Error creating inode...\n", 23 ); return -1;}
    
    newNode->fileName = dirName;
    newNode->nodeID = inodeidx;
    newNode->fileType = 'd';
    newNode->fileSize = 0;
    newNode->ownerID = getuid();
    newNode->groupID = getgid();
    newNode->permissions = 6;
    newNode->hardLinkNumber = 1;
    newNode->blockOffset = 0;

    for( int i = 0; i < INODE_PTR_ARRAY_SIZE; i++ ) { newNode->dataBlockArray[i] = 0; }

    // if it's the home directory it doesn't have a parent directory
    if( strcmp( dirName, "~" ) == 0 )
    {
        newNode->dataBlockArray[0] = dirName;
        newNode->dataBlockArray[1] = dirName;
    } else
    {
        newNode->dataBlockArray[0] = workingDirectory;
        newNode->dataBlockArray[1] = dirName;
    }
    //--------------------------------------------------------//

    //---------- Open the inodearray file ---------------//
    char tmpString[1024]; char tmpString2[512]; int stringSize; char charReader[2]; int found = 0;
    
    fileDescriptor inodearray = open( "filesystem/cylinderBlock_0/inodearray", O_RDWR );
    if( inodearray == -1 ) { write( STDOUT_FILENO, "Error opening the inode array file\n", 35 ); return -1; }


    //---------- Checks if the parent directory has pointers available for new inodes ---------------//
    stringSize = snprintf( tmpString, 1023, "%s", workingDirectory );
    read( inodearray, tmpString2, stringSize );
    tmpString2[stringSize] = '\0';

    if( strcmp( dirName, "~" ) != 0 )
    {
        // searches the inodearray for the parent directory inode
        while( strcmp( tmpString, tmpString2) != 0 )
        {
            // if the parent directory is not found, it bypasses the line
            read( inodearray, charReader, 1 );
            charReader[1] = '\0';

            while( strcmp( charReader, "\n" ) != 0 )
            {
                read( inodearray, charReader, 1 );
                charReader[1] = '\0';
            }
            // it reads again the first word from the next line
            read( inodearray, tmpString2, stringSize );
        }

        // when it finds the parent directory inode, it searches to find the next 
        // available location in its pointer array to add the hard link for the new inode
        while( strcmp( tmpString, "NULL" ) != 0 )
        {
            stringSize = 0; 
            read( inodearray, charReader, 1 );
            charReader[1] = '\0';

            if ( strcmp( charReader, "\n" ) == 0 )
            { 
                printf( "No direct pointer available\n" );
                found = -1;
                return -1; 
            }

            while( strcmp( charReader, " " ) != 0 )
            {
                tmpString[stringSize] = charReader[0];
                stringSize++;
                read( inodearray, charReader, 1 );
            }
            tmpString[stringSize] = '\0';
        }
    }

    lseek( inodearray, 0, SEEK_SET );

    //---------- Add the new node in the inode array ---------------//
    // finds the next available inode
    do
    {
        read( inodearray, charReader, 1 );
        charReader[1] = '\0';

        if( strcmp( charReader, "?" ) != 0 )
        {
            while( strcmp( charReader, "\n" ) != 0 )
            {
                read( inodearray, charReader, 1 );
                charReader[1] = '\0';
            }
        }
    } while ( strcmp( charReader, "?" ) != 0 );
    
    lseek( inodearray, -1, SEEK_CUR );

    // writes the inode info
    stringSize = snprintf( tmpString, 1023, "%s ", newNode->fileName );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->nodeID );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%c ", newNode->fileType );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->fileSize );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->ownerID );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->groupID );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->permissions );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->hardLinkNumber );
    write( inodearray, tmpString, stringSize );
    stringSize = snprintf( tmpString, 1023, "%d ", newNode->blockOffset );
    write( inodearray, tmpString, stringSize );

    // writes the contents from the inode ptr array
    for( int i = 0; i < INODE_PTR_ARRAY_SIZE; i++ )
    { 
        if( newNode->dataBlockArray[i] == NULL )
        {
            write( inodearray, "NULL ", 5 );
        }else{
            stringSize = snprintf( tmpString, 1023, "%s ", newNode->dataBlockArray[i] );
            write( inodearray, tmpString, stringSize );
        }
    }

    // writes the ? character for the next time the function is called
    write( inodearray, "\n", 1 );
    write( inodearray, "?\n", 2 );

    // if it's the home directory the function ends here
    // otherwise continues to create the hard link from the parent directory to the new inode
    if( strcmp( dirName, "~" ) == 0 )
    {
        close( inodearray );
        free( newNode );
        inodeidx++;
        return 0;
    }

    int filesize = lseek( inodearray, 0, SEEK_END );
    lseek( inodearray, 0, SEEK_SET );

    //---------- Creates the hard link from the parent directory ---------------//
    stringSize = snprintf( tmpString, 1023, "%s", workingDirectory );
    read( inodearray, tmpString2, stringSize );
    tmpString2[stringSize] = '\0';

    // searches the inodearray for the parent directory inode
    while( strcmp( tmpString, tmpString2) != 0 )
    {
        // if the parent directory is not found, it bypasses the line
        read( inodearray, charReader, 1 );
        charReader[1] = '\0';

        while( strcmp( charReader, "\n" ) != 0 )
        {
            read( inodearray, charReader, 1 );
            charReader[1] = '\0';
        }
        // it reads again the first word from the next line
        read( inodearray, tmpString2, stringSize );
    }

    // when it finds the parent directory inode, it searches to find the next 
    // available location in its pointer array to add the hard link for the new inode
    while( strcmp( tmpString, "NULL" ) != 0 )
    {
        stringSize = 0; 
        read( inodearray, charReader, 1 );
        charReader[1] = '\0';

        if ( strcmp( charReader, "\n" ) == 0 )
        { 
            printf( "No direct pointer available\n" );
            found = -1;
            return -1; 
        }

        while( strcmp( charReader, " " ) != 0 )
        {
            tmpString[stringSize] = charReader[0];
            stringSize++;
            read( inodearray, charReader, 1 );
        }
        tmpString[stringSize] = '\0';
    }

    // after it finds the position to add the new inode
    // it reads the rest of the file into the remainingData variable
    // this is important because the new inode's name will change
    // the order of the data in the file
    int curOffset = lseek( inodearray, 0, SEEK_CUR );
    char remainingData[filesize];

    read( inodearray, remainingData, filesize-curOffset );
    remainingData[filesize-curOffset] = '\0';

    lseek( inodearray, -(filesize-curOffset) , SEEK_END );

    // it erases everithing in the file
    for( int i = 0; i < filesize-curOffset; i++ )
        write( inodearray, " ", 1 );

    lseek( inodearray, -(filesize-curOffset+5) , SEEK_END );

    // then it writes the new inode, and the rest of the data after
    stringSize = snprintf( tmpString, 1023, "%s ", dirName );
    write( inodearray, tmpString, stringSize );
    write( inodearray, remainingData, filesize-curOffset );

    close( inodearray );
    free( newNode );
    inodeidx++;

    return 0;
}

int mkFile(char* fileName)
{
    write( STDOUT_FILENO, "Under Construction! Come back later, sorry...\n", 46 );
    return 0;
}

int writeFile(char* fileName, char* buffer, int numBytes)
{
    write( STDOUT_FILENO, "Under Construction! Come back later, sorry...\n", 46 );
    return 0;
}
int readFile(char* fileName, char* buffer, int numBytes)
{
    write( STDOUT_FILENO, "Under Construction! Come back later, sorry...\n", 46 );
    return 0;
}
void printINodeInfo( char* fileName )
{
    write( STDOUT_FILENO, "Under Construction! Come back later, sorry...\n", 46 );

}
void chDir( char* dirPath )
{
    write( STDOUT_FILENO, "Under Construction! Come back later, sorry...\n", 46 );

}
