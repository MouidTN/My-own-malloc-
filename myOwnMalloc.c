#include "halde.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>

/// Magic value for occupied memory chunks.
#define MAGIC ((void*)0xbaadf00d) //is used to indicate uninitialised allocated heap memory when the debug heap is used._!!!!!_!!!!__!!!

/// Size of the heap (in bytes).
#define SIZE (1024*1024*1)

/// Memory-chunk structure.
struct mblock {
	struct mblock *next;
	size_t size;
	char memory[];
};

/// Heap-memory area.
char memory[SIZE];

/// Pointer to the first element of the free-memory list.
static struct mblock *head;

/// Helper function to visualise the current state of the free-memory list.
void halde_print(void) {
	struct mblock* lauf = head;
	if ( head == NULL ) {fprintf(stderr, "(empty)\n");return;}
	while ( lauf ) {
		fprintf(stderr, "(addr: 0x%08zx, off: %7zu, ", (uintptr_t) lauf, (uintptr_t)lauf - (uintptr_t)memory);
		fflush(stderr);
		fprintf(stderr, "size: %7zu)", lauf->size);
		fflush(stderr);
		if ( lauf->next != NULL ) {
			fprintf(stderr, "  -->  ");
			fflush(stderr);
		}
		lauf = lauf->next;
	}
	fprintf(stderr, "\n");
	fflush(stderr);
}


int Mblock_size = 8;

// Malloc
void *halde_malloc (size_t size) {
	if(size == 0 ){
		perror("cannot allocate 0 bytes!");
        exit(EXIT_FAILURE);
	}
	if ( size >= SIZE - Mblock_size){
        perror("no enough memory avalaible for  the desired to allocate memory!");
        exit(EXIT_FAILURE);
    }
	struct mblock* block;
	if (head == NULL) {
        Mblock_size = sizeof(struct mblock);
        block = (struct mblock*) memory;
        block->next = NULL;
        block->size = SIZE - Mblock_size;
    } else {
        block = (struct mblock*)head;
    }

    if(block->memory + size + Mblock_size < memory + SIZE){
        head = (struct mblock*) (block->memory + size);
        head->next = NULL;
        head->size =  size - Mblock_size;
    } else {
        struct mblock* block = (struct mblock*) memory;
	    while((*block).memory  < memory + SIZE){
		   if((*block).next == MAGIC ) continue;
		   if((*block).memory + (*block).size >= memory + SIZE) return NULL;
		   head= block;
	    }
        head = block;
        if(head == NULL){
            head = (struct mblock*) memory;
        }
    }
	return block->memory;
}




