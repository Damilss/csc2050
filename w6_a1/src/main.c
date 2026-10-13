/*
 * Author: Emilio Scott
 * main.c is a simple calculator used as first practice with heap memory
 * management
 */
#include <stdlib.h> 
#include <stdio.h>

/*
 * getValues() prompts user for two numbers that will be used
 * function will
 * @param: int* num1
 * @param: int* num2
 * @return: void
 */
void getValues (int* num1, int* num2){
	printf("Please enter two integers seperated by a space:");
	scanf("%d %d", num1, num2);
}

/*
 * sum, self explanatory
 * @param: int* num1
 * @param: int* num2
 * @return: int
 */
int sum (int* num1, int* num2){
	return (*num1 + *num2);
}

/*
 * diff, just subtraction
 * @param: int* num1
 * @param: int* num2
 * @return: int
 */
int diff (int* num1, int* num2){
	return (*num1 - *num2);
}

/*
 * Product, just multiplication between two arguments
 * @param: int* num1
 * @param: int* num2
 * @return: int
 */
int prod(int* num1, int* num2){
	return (*num1 * *num2);
}

/*
 * quot, just division between two arguments
 * @param: int* num1
 * @param: int* num2
 * @return: int
 */
int quot(int* num1, int* num2){
	return (*num1 / *num2);
}

/*
 * rem, just a modulo function between two arguments
 * @param: int* num1
 * @param: int* num2
 * @return: int
 */
int rem (int* num1, int* num2){
	return (*num1 % *num2);
}

/*
 * main function, start of program
 * @param: int argc (argument count)
 * @param: char* argv (arguments)
 * @return: int
 */
int main (int argc, char* argv[]){
	int* x = malloc(sizeof(int));
	int* y = malloc(sizeof(int));	
	printf("Hi, welcome to the simple calculator\n");

	//sets the x and y valus
	getValues(x, y);

	//each printline gets each function call
	printf("\nThe values entered are %d and %d", *x, *y);
	printf("\nThey are stored in the memory locations %p and %p", x, y);
	printf("\nThe sum is: %d", sum(x, y));
	printf("\nThe difference is: %d", diff(x,y));
	printf("\nThe product is: %d", prod(x, y));
	printf("\nThe remainder is: %d \n", rem(x, y));

	return 0;
}
