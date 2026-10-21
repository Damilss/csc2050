# CSC2050 - Week 9 Activity 1

Extended the fucntion below so it adds integers, doubles, and characters.
The function should return the arithmetic result for integers and doubles.
For characters the function shoudl return the concatenation of the two
characters.

```c
void *add(char *type, void *vlhs, void *vrhs){

    if (strcmp(type, "int") == 0){
        int *lhs = (int*) vlhs;
        int* rhs = (int*) vrhs;
        
        return (void*)(*lhs + *rhs);

    } else if (strcmp(type, "double") == 0){
        double *lhs = (double*) vlhs;
        double *rhs = (double*) vrhs;

        return(void*)(*lhs + *rhs);
    }
}
```
