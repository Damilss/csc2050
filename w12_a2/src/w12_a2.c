/**
 * author: Paris Kalathas <pkalatha@calpoly.edu> <escott15@calpoly.edu>
 * date: 2026-11-12
 * file: w12_a2.c
 */
#include <_stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdlib.h>

const struct rlimit nproc_lim = {
	.rlim_cur = 10,
	.rlim_max = 10
};

int main(int argc, char* argv[]){

	if (setrlimit(RLIMIT_NPROC, &nproc_lim)== -1){
		perror("setrlimit(): something went wrong\n");

	return EXIT_FAILURE;
	}

	pid_t spawnpid = -5;
	int ten = 10;
	printf("My name is Emilio id: %d\n", spawnpid);

	spawnpid = fork();

	switch (spawnpid){

	case -1:
		perror( "Fork Failed!" );
		return -1;
	case 0:
		ten = ten + 1;
		printf("I am the child! ten = %d, spawnpid = %d\n", ten, spawnpid);
		break;
	default:
		ten = ten - 1;
		printf("I am the parent! ten = %d , spawnpid = %d\n", ten, spawnpid);
		break;
	}

	printf("This will be executed by both of us!\n");
    return 0;
}
