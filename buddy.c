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
void offset_updater();

const int HEAPSIZE = (1*1024*1024); // 1 MB
const int MINIMUM_ALLOC = sizeof(int) * 2;

// global file-scope variables for keeping track
// of the beginning of the heap.
void *heap_begin = NULL;

void int *gronk(int * heap_pointer,  int alloc_size)
{
    int *heap_ptr = heap_pointer;
            
    if (heap_ptr[0] == alloc_size)
    {
        heap_ptr[1] = 0;
        offset_updater();
        return heap_ptr[2];
    }

    if (heap_ptr[0] < alloc_size)
    {
        if (ptr[1] == -1)
            return NULL;
        int ptr_offset = ptr[1]/4;
        return gronk(heap_ptr + ptr_offset, alloc_size);
                                                            
    } else {
        int buddy = ptr[0]/2;
        int former_offset = heap_ptr[1];
        heap_ptr[0] = buddy;
        heap_ptr[1] = buddy;
        int buddy_offset = buddy/4;
        heap_ptr[buddy_offset] = buddy;                      
        heap_ptr[buddy_offset+1] = buddy;   
        return gronk(heap_ptr, alloc_size);
    }
}

void *malloc(size_t request_size)
{
    void *alloc_ptr
    // if heap_begin is NULL, then this must be the first
    // time that malloc has been called.  ask for a new
    // heap segment from the OS using mmap and initialize
    // the heap begin pointer.   
    if (!heap_begin)
    {
        heap_begin = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        atexit(dump_memory_map);
    }
    
    request_size = request_size + 8;
    int alloc_size = MINIMUM_ALLOC;

    while(alloc_size < request_size)
        alloc_size *= 2;    
    
    alloc_ptr = Gronk(/*heap_begin*/, alloc_size);    
}

void free(void *memory_block)
{

}

void dump_memory_map(void)
{

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
