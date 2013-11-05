#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// function declarations
void *malloc(size_t);
void free(void *);
void dump_memory_map(void);


const int HEAPSIZE = (1*1024*1024); // 1 MB
const int MINIMUM_ALLOC = sizeof(int) * 2;

// global file-scope variables for keeping track
// of the beginning of the heap.
void *heap_begin = NULL;


void *malloc(size_t request_size) {

    // if heap_begin is NULL, then this must be the first
    // time that malloc has been called.  ask for a new
    // heap segment from the OS using mmap and initialize
    // the heap begin pointer.
    if (!heap_begin) {
        heap_begin = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        atexit(dump_memory_map);
    }


}

void free(void *memory_block) {

}

void dump_memory_map(void) {

}

void offset_updater(void) {
    int *free_ptr = NULL;
    int *temp_ptr = heap_begin;
    while (temp_ptr[1] != -1)
    {
	if (temp_ptr[1] != 0) {
	    free_ptr = temp_ptr;
    	}
	if (free_ptr == NULL) {
	    temp_ptr += (temp_ptr[0]/4);
	    continue;
	}
    	if (temp_ptr[1] == 0 && free_ptr != NULL)
	    free_ptr[1] += (temp_ptr[0]/4);
	temp_ptr += (temp_ptr[0]/4);
    }
    return;
}





  
