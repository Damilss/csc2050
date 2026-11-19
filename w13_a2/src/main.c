/**
 * author: Emilio Scott <escottt15@calpoly.edu>
 * date; 2026-11-19
 * file: main.c
 */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int fd;

void handler(int sig){
	char ans[8];

	printf("String too long. Continue? (y/n): ");
	fgets(ans, sizeof(ans), stdin);
	if (ans[0] == 'n'){
		close(fd);
		exit(0);
	}
}

int main (int argc, char *argv[]){
	char buf[256];
	pid_t pid = fork();

	if (pid == 0){
		signal(SIGALRM, handler);
		fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

		while (printf("> "), fgets(buf, sizeof(buf), stdin)){
			write(fd, buf, strlen(buf));
			buf[strcspn(buf, "\n")] = '\0';
			if (strlen(buf) > 15){
				raise(SIGALRM);
			}
		}
		close(fd);
		exit(0);
	}

	wait(NULL);
	return 0;
}
