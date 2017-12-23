Carsten Hood
UIN: 922009787
MP2

// part 2 report included as “report.pdf”

// compiling / running instructions:

// compile with makefile:
$ make

// run client:
$ ./client

// added semaphore.C and mutex_guard.C
// test processes removed so main files are empty

// compile separately:
$ g++ -std=c++11 semaphore.C -o sema
$ ./sema
$ g++ -std=c++11 mutex_guard.C -o mg
$ ./mg
