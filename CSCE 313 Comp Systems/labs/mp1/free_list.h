/*
 File: free_list.h
 
 Author: Carsten Hood
 Department of Computer Science
 Texas A&M University
 Date  : July 10, 2016
 
 This file contains the declaration of the module "FREE_LIST".
 */

#ifndef free_list_h
#define free_list_h

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

// header in free list represents a free block
typedef struct fl_header FL_HEADER;
struct fl_header {
    struct fl_header * prev;
    struct fl_header * next;
    unsigned int index;
    char side;  // left/right bit
    char inheritance; // inheritance bit
    int is_free;
};

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE FREE_LIST */
/*--------------------------------------------------------------------------*/

// initialize header to default values
void FL_init(FL_HEADER * header, unsigned int index,
             char side, char inheritance);

// remove given block from given free list
void FL_remove(FL_HEADER ** free_list, FL_HEADER * node);

// adds a block to the free list
void FL_add(FL_HEADER ** free_list, FL_HEADER * node);

// prints content of free list
void FL_print(FL_HEADER * free_list);

// print information of a single block
void FL_print_block(FL_HEADER * block);

#endif /* free_list_h */