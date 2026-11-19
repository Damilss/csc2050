# CSC 2050 Week 15 Activity 2

write a program that uses a parent process to spawn one child process. The child process should read strings from the keyboard and store them into a file. If a string is more than 15 chars, the child should be send an alaram. The process should handle the alarm signal using a signal handler function. The signal handler should ask teh user if they want to continue and if the suer types 'n' then the functions should close the file descriptor and exit the program the program should continue normally
