# csc2050-simplecalculator
> CSC 2050 software system mechanics

### task 1. Simple calculator

For this acitivty you need to create a simple calculator script. Your script should be called calc, and it should have three parameters for the operands and the operation to perform. Your script should be able to perform **addition**, **subtraction**, **division**, and **modulo**. Users should be able to use your script by typing `./calc 2+ 4`. Below is an example of how the script should run

```bash

user@sever~$ ./calc 2 + 4
6
user@server~$ ./calc 7 - 6
1
user@sever~$ ./calc 4 \* 5
20
user@sever~$ ./calc 4 / 2
2
user@server~$ ./calc 4/3
1.33
user@server~$ ./calc 4 = 3
wrong operator
user@serever~$ ./calc 4
Not enough arguments...
```

### notes
1. Use the `expr` command for every math operation except division
2, Use the bc command to support decimal division
3. you will need to figure out how to make * be passed as a symbol and avoid its expansion (the example above show how!). Also you will need to figure out how to make it work as a mathematical operator inside the script

### submission
When finished, downlaod it and submit it on CANVAS. 

