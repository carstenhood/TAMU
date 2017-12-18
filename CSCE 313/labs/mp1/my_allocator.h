/*
 File: my_allocator.h
 
 Author: R.Bettati
 Department of Computer Science
 Texas A&M University
 Date  : 08/02/08
 
 Modified: declared helper functions to organize code.
 
 */

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr;

/*--------------------------------------------------------------------------*/
/* MODULE   MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

// =========================================================================
// initialization functions

// determines number of fibonacci values that fit within given length
unsigned int get_fib_count(unsigned int _length,
                           unsigned int _basic_block_size);

// populates array that maps indices to fibonacci numbers
void generate_fibs(unsigned int fib_count);

// initializes array of free list pointers and starting block
void init_free_lists();

/* This function initializes the memory allocator and makes a portion of
 ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as
 its minimal unit of allocation. The function returns the amount of
 memory made available to the allocator. If an error occurred,
 it returns 0.
 */
unsigned int init_allocator(unsigned int _basic_block_size,
                            unsigned int _length);

// =========================================================================
// release function

// returns any allocated memory to the operating system
int release_allocator();

// =========================================================================
// allocation functions

// determines the index of fibs/free-lists arrays that satisfies length
unsigned int determine_index(size_t minimum_length);

// splits parent block into two children and adds children to lists
void split_block(char * parent_pointer, char ** left_pointer,
                                        char ** right_pointer);

// recursively splits blocks until block with desired fib is available
char * get_block(char * parent_pointer, unsigned int desired_index);

// returns pointer to block space
Addr my_malloc(size_t _length);

// =========================================================================
// free functions

// merges a block with its buddy if possible, returns NULL if not
char * merge_block(char * block_pointer);

// frees section of physical previously allocated memory
int my_free(Addr _a);

// =========================================================================
// test functions

// prints each free list
void print_free_lists();

#endif 
