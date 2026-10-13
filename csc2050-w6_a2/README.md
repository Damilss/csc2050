# CSC2050 - Week 6 activity 2

For this activity you will need to build a simple employee databse in C.
The databse should be in the form of an array that should be able to hold 10
employees. The employee data should be stored in a struct datatype. You should
store the employee name and their salary. Your program should have the 7 following
functions. 

- `int main(int argc, char *argv[])` -> the program entry point
- `double avrgSal(empl *database, int size);` -> returns the average salaray
- `int minSal(empl *database, int size);` -> returns lowest saalary
- `int maxSal(empl *database, int size);` -> returns the highest salary
- `void printEmployees( empl *database, int size);` -> prints data rom databse on the screen

In your main function, you should declare and allocate space for the databse.
Then create 4 employees and initialize their names and their salary using random
numbers from $100,000 - $200,00. Use the `srand()` and `rand()` functions to do
this. Then add those employees into your databse, can call the functions to test
them

### Example (user input is in bold)

```sh
The average salary is 130477.25

The highest salary is 173961

The lowest Salary is 108105

Ema -> $108105

Mario -> $173961

Mona -> $119058

Charles -> $120785
```

**NOTE:** Make sure that all memory usage restored before the program exits. Make sure there are no warnings during the compliation.

