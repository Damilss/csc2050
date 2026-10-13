# Notes

### 6f68c68 chore(csc2050): add w6_a2 instructions and teacher code

As we continue to learn C I notice the descrepancies within the teachers examples.
He's very clearly trying to maxmize practice with memory management in the heap,
But I can't help but cringe at the amount of memory being used.

In this commit, the teaver makes us allocate space twice for our `struct Employee`.
Granted I follow along because it was apart of the instruction, But my next commit
shall be to optimize this code. 

### 187ea45 refactor: main.c, refactors teachers example and finish assignment

refactored code, as well as added verbose inline documentation, Topics learned:

- string literals
- pointer decay
- enums 
- preprocessor directives
- folding constants
- pointer arithmetic
- pointer to struct attributes using `->`
- discarding qualifiers through having different variables access the same string literal
    - EX: assigning a `const char *` to a `char *`. which makes it modifiable

