/**
 * file: w9_a1/main.c
 * Author: Emilio L <milii76@outlook.com>
 * date: 2026-10-20
 *
 * add function adds any data type, if char, concats chars together
 */

#include <string.h>
#include <stdlib.h>

/**
 * function: void *add(char *type, void *value1, void *value2);
 * add() adds two variables together, if they're char they concatenate, if
 * they're ints, add them, if they're doubles, add two of them together.
 * else just return a (void*)result.
 *
 * @param: char *type, the type as a string literal / string.
 * @param: void *value1, first value to add together, type determined
 *         by char *type.
 * @param: void *value2, second value to add together. Type determined
 *         by char *type.
 * @return: void *, the result as a void pointer, with the value holding
 *         the result.
 */
void *add( char *type, void *value1, void *value2 ){

    // add two ints
    if (strcmp(type, "int") == 0){
       int *newvalue1 = (int*)value1;
       int *newvalue2 = (int*)value2;

       int *result = malloc(sizeof(int*));
       *result = *newvalue1 + *newvalue2;

       return (void*)result;
    
    // add two doubles
    } else if (strcmp(type, "double")){
        double *newvalue1 = (double*)value1;
        double *newvalue2 = (double*)value2;
        
        double *result = malloc(sizeof(double*));
        *result = *newvalue1 + *newvalue2; 
        
        return (void*)result;

    // concatenate two chars
    } else if (strcmp(type, "char")){
        char *newvalue1 = (char*)value1;
        char *newvalue2 = (char*)value2;

        char *result = malloc(3 * sizeof(char));
        result[0] = *newvalue1;
        result[1] = *newvalue2;
        result[2] = '\0';

        return (void*)result;
    }

    void *result;
    return result;
}

