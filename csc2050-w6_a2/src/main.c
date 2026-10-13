#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * is a small employee databse practice with arrays and malloc and structs
 * Author: Emilio Scott
 */

/*
 * typedef allows us to create a sort of alias over our struct employee and 
 * so we don't have to continue to type out `struct Employee` every single time
 * Essentially `struct Employee` = `empl`
 */
typedef struct{

	// switched to const since pointing to string literals type `const char *names[]`
	const char *name;
	int salary;
}empl;

double avrgSal(empl *employee, int size);
int minSal(empl *employee, int size);
int maxSal(empl *employee, int size);
void printEmployees(empl *employee, int size);

/*
 * alternatively could use `#define SIZE 8` preprocessor directive.
 * SIZE is used as the curent seeded count of employees
 */
enum { SIZE = 8 };

/*
 * main(), entry point of program
 * @param: int argc argument count
 * @param: char *argv[] argument variables
 * @return int, exit status
 */
int main(int argc, char *argv[]){

	/*
	 * These are the seed names,
	 * Modern C treats string literals as read-only memory to be safe
	 * even if the string literals arent' actually places within
	 * read only memory by the compiler. It's important to not treat
	 * string literals this way as modifing through like:
	 *
	 * names[0][0] = 'P'
	 *
	 */
	const char *names[SIZE] = {
		"Sam", "Jenice",
		"Tom", "Jack",
		"Ema", "Mario",
		"Mona", "Charles"
	};


	//update random seed	
	srand(time(NULL));
	
	//init database
	empl *employeedatabase = malloc(sizeof(empl)*10);	
	
	//seed employees
	for(int i = 0; i <= SIZE; i++){
		employeedatabase[i].salary = rand() % 100001 + 100000;
		employeedatabase[i].name = names[i];
	}		
	
	printf("The average salary is %0.2lf\n", avrgSal(employeedatabase, SIZE));
	printf("The highest salary is %d\n", maxSal(employeedatabase, SIZE));
	printf("The lowest salary is %d\n", minSal(employeedatabase, SIZE));
	printf("\n");
	printEmployees(employeedatabase, SIZE);
	
	// dealloc; Everything is relational to employeedatabse, so only one free needed
	free(employeedatabase);
	employeedatabase = NULL;

	return 0;
}

/*
 * avrgSal() takes the all of the employees salaries within our declared
 * employee databse and finds the average salary of all of them
 * @param: empl* databse
 * @param: int size
 * @return: double, avergae of all of the salaries
 */
double avrgSal(empl *empldatabase, int size){

	int sum = 0;
	for ( int i = 0; i < size; i++ ){
		sum = sum + (empldatabase + i)->salary;
	}

	return (sum/size);
}

/*
 * printEmployees() does exaclty that
 * @param: empl *empldatabase, the employeedatabase goes here
 * @param: int size, count of employees in database
 * @return: void
 */
void printEmployees(empl *empldatabase, int size){
	for(int i = 0; i < size; i++){
		printf(
		"%s -> $%d\n", empldatabase[i].name, empldatabase[i].salary
		);
	}
}

/*
 * minSal() returns the minimum salary out of all of the employees within 
 * the specified parammeter database
 * @param: empl *empldatabase, the specific employee databse
 * @param: int size, count of employees in database
 * @return: int, returns the smallest salary out of all the employees
 */
int minSal(empl *empldatabase, int size){
	int min;
	for(int i = 0; i < size; i++){
		if (i == 0){
			min = empldatabase[i].salary;
		} else if (empldatabase[i].salary < min){
			min = empldatabase[i].salary;
		}
	}

	return min;
}

/*
 * maxSal() returns the maximum salary out of all the employees within the
 * specified paramter database
 * @param: empl *empldatabase, the specific employee databse
 * @param: int size, the count of employees within specified database
 * @return int, returns the biggest salary out of all of the employees or zero if 
 * unassigned
 */
int maxSal(empl *empldatabase, int size){
	int max = 0; 
	for(int i = 0; i < size; i++){
		if(empldatabase[i].salary >= max){
			max = empldatabase[i].salary;
		}
	}
	
	//eror checking
	if(max == 0){
		printf("maxSal(): max is 0\n");
	}

	return max;
}

