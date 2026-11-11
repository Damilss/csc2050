/**
 * file: printenv.c
 * author: Emilio Scott <escott15@calpoly.edu
 * date: 2026-11-10
 *
 * w12_a1 activity
 */

#include <stdlib.h>
#include <stdio.h>

/**
 * printenv - gets enviornment variable specified and prints to
 *			  stdout
 * @name: specified enviornment variable
 *
 * @return: void
 */
void printenv(const char *name);

/**
 * main - entry point of program
 * @argc: argument count
 * @argv: argument variables
 *
 * @return: exit status, 0 = success, non-zero = unsucessful
 */
int main(int argc, char *argv[]){
	char *value0 =  "LOGNAME";
	char *value1 = "PWD";
	char *value2 = "COLUMNS";
	char *value3 = "TERM";

	printenv(value0);
	printenv(value1);
	printenv(value2);
	printenv(value3);

	return 0;
}

void printenv(const char *name){
	char *envvalue = getenv(name);

	if (envvalue == NULL)
		envvalue = "env not found";

	printf("%s\n", envvalue);
}


