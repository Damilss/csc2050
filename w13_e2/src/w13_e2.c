/**
 * author: Emilio Scott <escott15@calpoly.edu
 * date: 2016-11-19
 * file: main.c
 */
#include <stdio.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <signal.h>

int count = 0;

/**
 * sig_fnct - catches SIGINT, makes sure the count is less than 5 then kills if
 * 			  the count of SIGINTs sent is greater than 5
 */
void sig_fnct(int sig){
	count++;
	printf("Total SIGINT signals caught %d\n", count);
	if (count > 5){
		raise (SIGKILL);
	}
}
/**
 * main - entry point of program
 * argc: argument count
 * argv: argument variables
 *
 * return: 0 if successful, nonzero is unsuccessful
 *
 */
int main (int argc, char *argv[]){
	for (;;){
		signal(SIGINT, sig_fnct);
	}
	return 0;
}
