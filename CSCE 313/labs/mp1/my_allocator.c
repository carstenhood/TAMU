/*
 File: my_allocator.c
 
 Author: Carsten Hood
 Department of Computer Science
 Texas A&M University
 Date  : July 15, 2016
 
 Modified: Implemented full fibonacci buddy allocator July 15, 2016.
 
 This file contains the implementation of the module "MY_ALLOCATOR".
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "my_allocator.h"
#include "free_list.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

static unsigned int pool_size;  // memory size of entire pool
static char * pool_start;       // pointer to beginning of memory pool
static unsigned int base_size; // memory size of smallest block unit
static unsigned int * fibs;     // maps indices to fibonacci numbers
static FL_HEADER ** free_lists; // stores pointers to free lists
static unsigned int list_count; // stores number of free lists & fibonacci #s

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

// =========================================================================
// initialization functions

/* This function initializes the memory allocator and makes a portion of
 ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as
 its minimal unit of allocation. The function returns the amount of
 memory made available to the allocator. If an error occurred,
 it returns 0. */
unsigned int init_allocator(unsigned int _basic_block_size,
                            unsigned int _length) {
    
    // calculate number of free lists based on block size & length
    base_size = _basic_block_size;
    list_count = get_fib_count(_length, base_size);
    if (list_count == 0) return 0;  // pool must fit at least one block
    
    // generate and store fibonacci numbers
    generate_fibs(list_count);
    if (!fibs) return 0;            // fibs array allocation failed
    
    // allocate pool space
    unsigned int fib_max = fibs[list_count-1];
    pool_size = fib_max * base_size;// may be less than requested length
    pool_start = (char *)malloc(pool_size);
    if (!pool_start) return 0;      // memory pool allocation failed
    
    // initialize free list array and generate starting block
    init_free_lists();
    if (!free_lists) return 0;      // list pointers allocation failed
    
    return pool_size;
}

// determines number of fibonacci values that fit within given length
unsigned int get_fib_count(unsigned int _length,
                           unsigned int _basic_block_size) {
    unsigned int fib_count = 0;
    unsigned int fib1 = 1;
    unsigned int fib2 = 1;
    while (fib2*_basic_block_size <= _length) {
        unsigned int temp = fib2;
        fib2 += fib1;
        fib1 = temp;
        ++fib_count;
    }
    return fib_count;
}

// populates array that maps indices to fibonacci numbers
void generate_fibs(unsigned int _fib_count) {
    unsigned int fibs_array_size = sizeof(int) * _fib_count;
    fibs = (unsigned int *)malloc(fibs_array_size);
    if (!fibs) return;
    unsigned int fib1 = 1;
    unsigned int fib2 = 1;
    for (int i=0; i<_fib_count; ++i) {
        fibs[i] = fib2;
        fib2 += fib1;
        fib1 = fibs[i];
    }
}

// initializes array of free list pointers and starting block
void init_free_lists() {
    // initialize array of free list pointers
    unsigned int free_lists_size = sizeof(FL_HEADER *) * list_count;
    free_lists = (FL_HEADER **)malloc(free_lists_size);
    if (!free_lists) return;  // allocation failed
    for (int i=0; i<list_count; ++i) {
        free_lists[i] = NULL;
    }
    
    // generate initial block comprising entire pool
    FL_HEADER * start_block = (FL_HEADER *)pool_start;
    unsigned int last_index = list_count-1;
    FL_init(start_block, last_index, 'r', 'r');
    free_lists[last_index] = start_block;
}

// =========================================================================
// release function

// returns any allocated memory to the operating system
int release_allocator() {
    free(pool_start);
    free(free_lists);
    free(fibs);
    return 0;
}

// =========================================================================
// allocation functions

// returns pointer to new block space
Addr my_malloc(size_t _length) {
    // printf("my_malloc():\n");

    // determine index of fib value that satisfies request
    size_t minimum_length = _length + sizeof(FL_HEADER);
    unsigned int desired_index = determine_index(minimum_length);
    if (desired_index == -1) return NULL;   // requested length too big
    
    // return address of final block from recursively splitting blocks
    char * start_block = NULL;
    char * alloc_block = NULL;
    unsigned int start_index = desired_index;
    for (unsigned int i=start_index; i<list_count; ++i) {
        start_block = (char *)(free_lists[i]);  // get first free block
        if (desired_index == 0 && i == 1)   // fib[1] can't be split
            continue;
        if (start_block != NULL) {  // free list is not empty
            alloc_block = get_block(start_block, desired_index);
            alloc_block += sizeof(FL_HEADER);
            break;
        }
    }
    
    // print current blocks for testing
    // print_free_lists();
    return alloc_block;
}

// determines the index of fibs/free-lists arrays that satisfies length
unsigned int determine_index(size_t minimum_length) {
    for (int i=0; i<list_count; ++i) {
        size_t block_size  = fibs[i] * base_size;
        if (block_size >= minimum_length) {
            // printf("index %d length %d\n", i, minimum_length);
            return i;   // index of blocks nearest to minimum length
        }
    }
    return -1; // requested length too big
}

// recursively splits blocks until block with desired fib is available
char * get_block(char * parent_pointer, unsigned int desired_index) {
    FL_HEADER * parent = (FL_HEADER *)parent_pointer;

    // printf("get curr %d desired %d:\n", parent->index, desired_index);
    int index_diff = parent->index - desired_index;
    if (index_diff == 0) {
        parent->is_free = 0; // mark block as occupied
        FL_remove(&free_lists[parent->index], parent);
        return parent_pointer;
    } else if (index_diff < 0 || parent->index < 2) {
        return NULL;
    } else { // if index_diff > 0 then split blocks
        char * left_pointer = NULL;
        char * right_pointer = NULL;
        split_block(parent_pointer, &left_pointer, &right_pointer);
        
        // choose which block to proceed with recursively
        char * chosen_child;
        if (desired_index == 0 && index_diff == 3)  // fib[2] can be split
            chosen_child = right_pointer;           // but fib[1] cannot be
        else if (index_diff == 1)               // if close then
            chosen_child = right_pointer;       // go with next fib down
        else // index differential is > 1
            chosen_child = left_pointer;
        return get_block(chosen_child, desired_index);
    }
}

// splits parent block into two children and adds children to lists
void split_block(char * parent_pointer, char ** left_pointer,
                                        char ** right_pointer) {
    FL_HEADER * parent = (FL_HEADER *)parent_pointer;
    char left_inherit = parent->side;
    char right_inherit = parent->inheritance;
    unsigned int left_index = parent->index - 2;
    unsigned int right_index = parent->index - 1;
    FL_remove(&free_lists[parent->index], parent);
    
    * left_pointer = parent_pointer;
    FL_HEADER * left_child = (FL_HEADER *)(* left_pointer);
    FL_init(left_child, left_index, 'l', left_inherit);
    FL_add(&free_lists[left_index], left_child);
    
    size_t right_shift = fibs[left_index]*base_size;
    * right_pointer = (char *)left_child + right_shift;
    FL_HEADER * right_child = (FL_HEADER *)(* right_pointer);
    FL_init(right_child, right_index, 'r', right_inherit);
    FL_add(&free_lists[right_index], right_child);
}

// =========================================================================
// free functions

// frees section of physical previously allocated memory
int my_free(Addr _a) {
    //printf("my_free():\n");

    FL_HEADER * block = (FL_HEADER *)((char *)_a - sizeof(FL_HEADER));
    if (!block) return -1;
    if (block->is_free == 1) return -1;
    block->is_free = 1; // mark as available
    FL_add(&free_lists[block->index], block);
    
    // merge blocks until list runs out or buddy isn't mergeable
    while (block && block->index < list_count-1) { // can't merge last
        block = (FL_HEADER *)merge_block((char *)block);
    }
    
    // print current blocks for testing
    // print_free_lists();
    return 0;   // everything okay
}

// merges a block with its buddy if possible, returns NULL if not
char * merge_block(char * block_pointer) {
    FL_HEADER * block = (FL_HEADER *)block_pointer;
    FL_HEADER * left_block = NULL;
    FL_HEADER * right_block = NULL;
    
    // determine side buddy is on and whether buddy can merge
    if (block->side == 'l') {
        left_block = block;
        unsigned int right_index = left_block->index + 1;
        size_t right_shift = fibs[left_block->index] * base_size;
        right_block = (FL_HEADER *)((char *)left_block + right_shift);
        if ((right_index != right_block->index) // is sub-block of buddy
            || (right_block->side != 'r')       // block can't be buddy
            || (right_block->is_free != 1)) {   // block is alloced
            return NULL;
        }
    } else {
        right_block = block;
        unsigned int left_index = right_block->index - 1;
        size_t left_shift = fibs[left_index] * base_size;
        left_block = (FL_HEADER *)((char *)right_block - left_shift);
        if ((left_index != left_block->index)  // is sub-block of buddy
            || (left_block->side != 'l')       // block can't be buddy
            || (left_block->is_free != 1)) {   // block is alloced
            return NULL;
        }
    }
    FL_remove(&free_lists[left_block->index], left_block);
    FL_remove(&free_lists[right_block->index], right_block);
    
    // create merged block
    FL_HEADER * merged_block = left_block;
    char merge_side = left_block->inheritance;
    char merge_inher = right_block->inheritance;
    FL_init(merged_block, right_block->index+1, merge_side, merge_inher);
    FL_add(&free_lists[merged_block->index], merged_block);
    
    return (char *)merged_block;
}

// =========================================================================
// test functions

// prints each free list
void print_free_lists() {
    printf("Printing %d free lists:", list_count);
    if (!free_lists) {
        printf(" null pointer.\n");
    } else {
        printf("\n");
        for (int i=0; i<list_count; ++i) {
            printf("LIST ");
            printf("index: %d, fib val: %d, size: %d, pointer: %p, ",
                   i, fibs[i], fibs[i]*base_size, free_lists[i]);
            FL_print(free_lists[i]);
        }
    }
    printf("\n");
}