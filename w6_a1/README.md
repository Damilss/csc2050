SC 2050 - Week 6 Activity 1

For this activity you will need to build a calculator in C. Your calculator
should read two integer values from the user, store them in theheap, and then use
them to calculate their sum, difference, product, and remainder. Your program
should have the 7 following functions.

- `int main(int argc, char* argv[]);`
- `void getValues(int* val1, int* val2) -> gest the value from the user
- `int sum (int* num1, int* num2) -> returns the sum
- `int diff (int* num1, int* num2) -> returns the difference
- `int prod(int* num1, int* num2) -> returns the product
- `int quot(int* num1, int* num2) -> returns the qoutient
- `int rem(int* num1, int* num2) -> returns remainder

in your main function, you should use the functions to get the inputs from
the user, perform the calculations and print the results in the order sum,
difference, product, quotient, and remainder.

## <u>Example (user input is in bold)</u>

Hi, welcome to the simple calculator
Please enter two integers separated by a space: **4** **5**
The values entered are 4 and 5
They are stored in the memory locations 0x32426 and 0x42372
the sum is: 9
the differense is: -1
the product is: 20
the quotient is: 20
The remainder is:4

## **Extra credit (2 points)**

Give the user the option to enter values or having the values created randomly
between 0 - 100 included. make sure the random numbers are different every time
the program runs.

