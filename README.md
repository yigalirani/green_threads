green_threads
=============

C++ Green threads and coroutines library

Motivation:
* White faster multithreaded programs by utilizing reduced context switch time.
*Write shorter and more elegant programs by using the yield functionality that is modeled after the python yield keyword and co-routines.

Future development
*Integrate the library with IO functionality like sockets and files
*Further improve speed .
*Add dynamic number of green threads.

How does it work: 
The library explicitly allocate runtime stack for each green thread.  The gt_yield function switches the current green thread by manipulating, using assembler, the esp and ebp registers who point the current runtime stack.
