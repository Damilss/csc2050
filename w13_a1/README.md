# CSC 2050 - Week 13 Activity 1

Write a program that uses a parent process to spawn two child processes. The first child process should read strings from the keyboard and store them into a file. The second child process should run the *wc* command on the file with the strings and print its output on the screen.

__Note__: make sure you synchronize your processes so the data are available when each process needs them.

```
main{} Process A
      | fork()
     / \
    /   First child: Read strings and store them into a file
   | fork()
  / \
 /   Second child: Use one of the exec functions to run wc on the file
Parent A1
```

__Submission__: Submit your .c file on CANVAS on W13 activity 1.
