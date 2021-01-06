
# nanodtypes
Simple, portable, space-efficient data structures

## Overview
Nanodatatypes (ND) is a small library written in pure C with no dependencies other than
the C Standard Library.
It's simple to compile and use on any UNIX/Linux system.

## What's included?
ND consists of the following data structures:

- **nStack**, a no-frills stack abstraction
- **nList**, a circular, doubly-linked list
- **nTable**, a Patricia trie which stores key-value pairs

## Features

- **Opaque data types**: use it with `int`, `char`, `void *`, or your own `struct` or `union`
- **Memory management**: malloc and free are handled for you


## How do I use it?
Here's a simple example of how you use nlist (which is really a deque):

First, declare a list data structure.
It's pretty small, so you can put on the stack if you want,
as long as you keep it in scope while you are using it.

    struct nList myList;

Before you use your list, you have to initialize it.
Let's assume you want its elements to be of type `unsigned short`.

    nListInit(&myList, sizeof(unsigned short));

So far, so good. Now let's add a few items to your list:

    unsigned short first=1, second=2;
    nListInsertHead(&myList, &first);
    nListInsertHead(&myList, &second);

Actually, `nListInsertHead` returns a status code, but for now, we'll assume that
everything worked without errors.

Now we want to remove the first element we added:

    unsigned short result;
    nListRemoveTail(&myList, &result);
    printf("%hu\n", result);
        ==> 1

We get the first item we inserted, since we removed it from the opposite end of your list.

That's it!
The interface to the other data types is similar (and just as easy to use).
ND provides simple, fast access to your data.
    
## How do I build/install it?
Open a terminal and navigate to the root of the repository--you should see a file
called "Makefile."
Type `make` to build nanodtypes.
This will create a library called "nanodtypes.a" in the "lib" directory.
To make sure the library functions properly on your system, type `./bin/test_all`,
which runs a comprehensive set of tests.

In source files where you need ND, include the header:

    #include "nanodtypes.h"

Then add it to the compilation step of your project:

    gcc -I/path/to/nd_repo/include <your_project.c> /path/to/nd_repo/lib/nanodtypes.a 
