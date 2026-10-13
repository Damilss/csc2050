```bash
emilio@raspberrypi:~/practice/programming-practice-2026/csc2050-w6_a2/src $ valgrind --leak-check=yes ./out
==1911== Memcheck, a memory error detector
==1911== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==1911== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
==1911== Command: ./out
==1911==
The average salary is 149947.00
The highest salary is 184511
The lowest salary is 124837

Sam -> $148900
Jenice -> $184511
Tom -> $124837
Jack -> $173168
Ema -> $170626
Mario -> $125080
Mona -> $125204
Charles -> $147255
==1911==
==1911== HEAP SUMMARY:
==1911==     in use at exit: 0 bytes in 0 blocks
==1911==   total heap usage: 2 allocs, 2 frees, 1,184 bytes allocated
==1911==
==1911== All heap blocks were freed -- no leaks are possible
==1911==
==1911== For lists of detected and suppressed errors, rerun with: -s
==1911== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

