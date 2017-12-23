/*
 File: free_list.c
 
 Author: Carsten Hood
 Department of Computer Science
 Texas A&M University
 Date  : July 10, 2016
 
 This file contains the implementation of the module "FREE_LIST".
 */


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "free_list.h"
#include <stdio.h>

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

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE FREE_LIST */
/*--------------------------------------------------------------------------*/

// initialize header to default values
void FL_init(FL_HEADER * header, unsigned int index,
             char side, char inheritance) {
    header->prev = NULL;
    header->next = NULL;
    header->index = index;
    header->side = side;
    header->inheritance = inheritance;
    header->is_free = 1;
}

// remove given block from given free list
void FL_remove(FL_HEADER ** free_list, FL_HEADER * node) {
    if (!node || !free_list || !(*free_list)) { printf("NULL REMOVE\n"); return; }
    FL_HEADER * prev_node = node->prev;
    FL_HEADER * next_node = node->next;
    
    // check if need to update free list pointer
    if ((* free_list) == node) {
        if (prev_node) * free_list = prev_node;
        else if (next_node) * free_list = next_node;
        else *free_list = NULL;
    }
    
    if (prev_node) prev_node->next = next_node;
    if (next_node) next_node->prev = prev_node;
}

// adds a block to the free list
void FL_add(FL_HEADER ** free_list, FL_HEADER * node) {
    if (!node || !free_list) { printf("NULL ADD\n"); return; }
    FL_HEADER * next_node = * free_list;
    node->next = next_node;
    if (next_node) {
        node->prev = next_node->prev;
        if (node->prev) node->prev->next = node;
        next_node->prev = node;
    }
    * free_list = node;
}

// print content of free list
void FL_print(FL_HEADER * free_list) {
    printf("blocks:");
    if (!free_list) {
        printf(" none.\n");
    } else {
        printf("\n");
        FL_HEADER * node = free_list;
        while (node) {
            printf("  BLOCK ");
            FL_print_block(node);
            node = node->next;
        }
    }
}

// print information of a single block
void FL_print_block(FL_HEADER * node) {
    if (!node) {
        printf("NULL.\n");
        return;
    }
    if (node->is_free == 1) printf("(FREE) - ");
    else printf("IN-USE - ");
    printf("index: %d, side: %c, inherits: %c, pointer: %p;\n",
           node->index, node->side, node->inheritance, (char *) node);
}

/* end free_list.c */