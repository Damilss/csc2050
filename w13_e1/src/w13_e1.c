/**
 * author: Emilio Scott <escott15@calpoly.edu>
 * date: 2026-11-19
 * file: w13_e1.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * main - entry point of program
 * argc: argument count
 * argv: argument variables
 *
 * return: 0 if successful, nonzero is unsuccessful
 *
 */
int main (int argc, char *argv[]){
	/* child: run ls -l (ls is in the PATH, so execlp finds it) */
	if (fork() == 0){
		execlp("ls", "ls", "-l", NULL);
		exit(1);
	}

	wait(NULL);
	return 0;
}
