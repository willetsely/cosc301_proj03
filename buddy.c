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

void offset_updater()
{
    int *free_ptr = NULL;
    int *temp_ptr = heap_begin;
    while(temp_ptr[1] != -1)
    {
        if(temp_ptr[1] != 0)
            free_ptr = temp_ptr;
        if(free_ptr == NULL)
        {
            temp_ptr += (temp_ptr[0])/4;
            continue;
        }
        temp_ptr += (temp_ptr[0])/4;
    }
    return;
}
