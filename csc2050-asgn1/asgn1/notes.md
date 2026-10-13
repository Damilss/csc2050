# Notes for assignment

## Overview 

In this assignment you will write a Bash script to calculate averages and medians from an input file of numbers. 

The input file will have whole number values separated by tabs (\t) (common separator in spreadsheets), and 
each line of this file will have the same number of values. For example, the file may be the students' 
grades for the class. Each row is one student's scores for every assignment, activity, quiz, etc in the class. 
Each column is every student's grade for a specific assignment, activity, etc. Your script should be able to 
calculate the average and median across the rows or down the columns. For example, if I want to calculate the 
average and median grades for every student, I should be able to call the script to calculate these stats for 
every row.If I want to calculate the average and median for every assignment, I should be able to call the 
script and calculate these stats for every column.

Your script should be called **statistics**. The general formate of the **statistics** command should be

```sh
statistics {-[r]ows|-[c]olumns} [input_file]
```

When parameters are in curly braces separated by a vertical bar, it means that that parameter is mandatory. The
vertical bar indicates that you could choose one of those parameters. For example, you must choose either `-rows`
or `-columns`. The option `-rows` calculates the average and median across the rows; the option `-cols` calculates
the average and median down the columns. When the parameters are in square braces it means they are optional;
you can include them or not, as you choose. If you specify an `input_file`, the script should read data from that
file; otherwise, it should read from the standard input.

Here is a sample run of what your script should return using the provided `test_file`. 

> Note, do not open test_file in windows.

It may add characters and corrupt the file. Upload it on the server and display its contents using cat command

```sh
cat test_file
```

**% cat test_file**

```sh
1   1   1   1   1
9   3   4   5   5
6   7   8   9   7
3   6   8   9   1
3   4   2   1   4
6   4   4   7   7
```
**% statistics -rows test_file**

```sh
Average Median
1   1
5   5
7   7
5   6
3   3
6   6
```
**%cat test_file | statistics -c**

```sh
Averages:
5   4   5   5   4
Medians:
6   4   4   7   5
```

**%echo $?**
`0`

**%statistics**

```sh
Usage: statistics {-rows|-columns} [file]
```

**%statistics -r test_file nya-nya-nya

```sh
Usage: statistics {-rows|-columns} [file]
```

**%statistics -columns test_file**

```sh
statistics: cannot read test_file
```

**%statistics -both test_file**

```sh
Usage: statistics {-rows|-columns} [file]
```

**%chmod -r test_file**

**%statistics -columns no_such_file**

```sh
statistics: cannot read no_such_file
```

**%echo $?**
`1`

## Specifications

1. You will probably need the following commands, so please read up on them:
    - trap
    - read
    - expr
    - cut
    - head
    - tail
    - wc
    - sort
    - bc


2. You must check for the right number of arugments to statistics. If the command has too many or two few argument, you should output an error mesage to standard error (1>&2). You should also output an error message to standard
error (1>&2) if the input file is specified, but is not readable

3. You must check for the right format of args to statistics. Allow users to abbreviate `-rows` and `-columns`;
any word beginning with a lowercase r is taken to be rows and any word beginning with a lowercase c is taken
to be cols. So, for example, you would get average and medians across the rows with `-2`, -rowwise, &
-rumplestlitskin, but not -Rows.

4. You should output statistics to standard output in the format shown on the example above. Be sure all error
messages are sent to standard error (1>&2), and the statistics are sent to standard output (&1). if there is any
error, the exit value should be 1; if the statistics program runs successfully the exit value should be 0.

5. You can assume that each row will be last than 1000 bytes long, but don't make any assumptions about the 
number of rows. You can assume that all rows will have the same number of values; you don't have to implement any
error checking on this

6. You will probably need to use temporary files, For this assignment, the temporary files should be put in the
currrent working directory. Be sure that temporary files use the process id as part of its name, so that there
will not be any conflicts if the statistics program is running more than once.

7. Be sure you remove any temporary files when your statistics program is done. You should use the trap command
to catch any interrupt, hangup, and terminate signals, and remove and temporary files if the statistics program is
terminated unexpectedly

8. All values and results are and must be whole numbers. You may use the `expr` or `bc` commands to do your 
calculations, or any other bash shell scripting, 

> ! warning
> but you may not do the calculations by dropping into another language, like awk, perl, python, etc.

Note that `expr` only works with whole numbers. When you calculate the average, you should round to the nearest
whole number, where half values raound up, i.e., 7.5 rounds up to 8.

9. To calculate the median, sort the values using the sort command, and take the middle value. When there is an
even number of values, choose the larger of the two middle values. For example, the median of 97, 90, 83, is 90.
The median of 97, 90, 83, 54 should still be 90.

10. Your script, statistics, must be entirely contained in that file. Do not split this assignment into multiple
files or programs. 

11. To make it easy to see how you're doing, the actual grading script is included in the assignment's folder. 
Compare your output to a perfect solution, `p1cleantestscript` file that show what you should gtet if everything
is working correctly. 

12. The assgnmnt1gradingscript itself is a good resource for seeing how some of the more complex shell scripting
commands work too

### hint

- One problem that will especially challenging is to read in the values from a specified file. The **read** command is exactly what you need (see man page for more info). However read is meant to read from standard input and the input file is not necessarily standard input to the statistics command. You will have to figure out (by reading the lecture slides!) how to get read to read from a file (Pipe maybe???)

- Another problem will be calculating the median. There is a straightforward pipeline command that will do this, using cut, sort, head, and tail. Maybe you can figure out other ways, too. Experiement!!

- the **expr** command and the shell can have conflicts over special characters. If you try **expr 5 * (4+2)**, the shell will think * is a filename wildcard and the parentheses mean command grouping. You will have to use blackslashes, like this:

expr 5\*(4+2)

## Submission and grading 

After submit on CANVAS, 

Grading will be based on successfully passing the script - 128 points, **AND on your style, readability, and
commenting / documentation** (32 PTS). Comment well, often, and verbosely. We mainly want to see that you are
telling us WHY you are doing things, but also tell us WHAT you are doing if it's needed. Read the course document
about proper documentation. 

Correlation study of student AI usage behaviors 

**https://stevens.qualtrics.com/jfe/form/SV_42w3fWc8opmt0I8?Course=CSC2050&Assignment=1**


