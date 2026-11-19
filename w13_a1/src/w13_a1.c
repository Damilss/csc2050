/**
 * author: Emilio Scott <escott15@calpoly.edu>
 * date: 2026-11-19
 * file: w13_a1.c
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	char buf[256];
	int fd;

	/* first child: read strings from keyboard into the file */
	if (fork() == 0){
		fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		while (printf("> "), fgets(buf, sizeof(buf), stdin)){
			write(fd, buf, strlen(buf));
		}
		close(fd);
		exit(0);
	}

	/* wait for the first child so the file is done before wc runs */
	wait(NULL);

	/* second child: run wc on the file */
	if (fork() == 0){
		execlp("wc", "wc", "out.txt", NULL);
		exit(1);
	}

	wait(NULL);
	return 0;
}
