# notes

### oct 2, 10:15AM

Something that I just newly learned is that like in our example:

```c
typedef struct{
    int *arr = malloc(10 * sizeof(int));
    int size = 0;
    int capacity = 10;
} intArr;
```

where the you have something like `int *arr` treating a pointer as an array and
using something like `arr[i]` where i is some arbitrary integer (asssuming within
range of the pointer). It is essentially:

```c
arr[i] = *(arr + i)
```

and using something like the `[ ]` automatically derefences the pointer which is
a lot easier to read then having to do something like `*(arr + i)`

### oct 6, 12:00AM

I found the difference between
```c
intArr initArr(void){
	int *new_arr = calloc(10, sizeof(int));	

	return (intArr){
		.arr = new_arr,
		.capacity = 10,
		.size = 0
	};
}
```
 and the difference between initaiting without a function:
 ```c
 intArr *result = &(intArr){
		.arr = calloc(10, sizeof(int)),
		.capacity = 10,
		.size = 0
	};
```
---
int test_dynamicarray.c, in the first function initArr, only returns type initArr 
and it is the caller's function to assign change the `intArr` to `intArr *` not the callee's.
The callee's only specific job is to return the intArr, but in the second function, since the
creation fo intArr and the assigning of the pointer is happening all in one pass, the `&(intArr)`
is haappening all in one pass and needs to happen all in one pass. It is those kind of subtle
pointer differnces between the caller and the callee that throw me off sometimes lol. 

### oct 6 8:19AM

I realized that I could create helper functions for creating seeds, but I decided against it
as I would have to change all my `intArr expected` values to `intArr *expected` in each of my
test functions. I could do 
```c
int _set_seed_values(intArr *arr, int idx){
    if (arr->capacity < idx){
        return 1; 
    } 
     for(int i = 0; i < idx; i++){
        arr->arr[i] = rand() % 100;

    }
    
    return 0;
}
```

Or something along those lines, not sure. if that's valid but that's the idea. However I will
just stick to running for loops in each function because I don't want to go through and change
each function.

### oct 7 1:35PM

I noticed that with the assertion count, even though assertion_count
isn't initialized as a pointer, it has the pointer-like behavior of being
able to obtain the address through `&assertion_count`. I can apply that to my test function where I have `intArr *result = &(intArr){...}` I can just do
`intArr result = (intArr){...}`, and then do `function(&result)`, it looks
cleaner and is easier to read from a programming standpoint


### oct 7 4:57PM

I just remembered that I added exit statuses for most of the functions
that I programmed, I will have to go back and add assertions for those as well...


### oct 7 5:11PM

I just realized that I forgot to include nullptr checks in my original 
functions, and that I'm not operating on null pointers, I only checked
for NULL pointers in my appendItem function... smh...
