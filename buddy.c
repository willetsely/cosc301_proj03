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
void offset_updater(int *, int);

const int HEAPSIZE = (1*1024*1024); // 1 MB
const int MINIMUM_ALLOC = sizeof(int) * 2;

// global file-scope variables for keeping track
// of the beginning of the heap.
void *heap_begin = NULL;
int *free_list = NULL;

/*
the gronk function is the work horse of malloc, performing all of the 
recursive work of splitting free blocks into chunks of the correct size, 
hence the namesake gronk

all honor to his name
*/
int *gronk(int *heap_pointer,  int alloc_size)
{
    int *heap_ptr = heap_pointer;
            
    if (heap_ptr[0] == alloc_size) //heap_ptr points to a block of the correct size
    {
    printf("allocating\n");
    printf("heap_ptr = %d\n", heap_ptr);
    printf("free_list = %d\n", free_list);
        if(heap_ptr == free_list) //if this block is the first free block
        {
            free_list += heap_ptr[1]/4; //update freelist to point to next free block
            printf("heap_ptr[1] = %d\n", heap_ptr[1]);
        }
        int old_offset = heap_ptr[1]; //save the old offset
        heap_ptr[1] = 0;  //mark as allocated
        offset_updater(heap_ptr, old_offset);
        return heap_ptr + 2;
    }

    if (heap_ptr[0] < alloc_size) //heap_ptr point to block that is too small
    {
        if (heap_ptr[1] == 0) //if this is the last free block
            return NULL;
        int ptr_offset = heap_ptr[1]/4;
        return gronk(heap_ptr + ptr_offset, alloc_size);
                                                            
    } else {
        printf("heap_ptr[0] = %d\n", heap_ptr[0]);
        int buddy = heap_ptr[0]/2;  //buddy = size of block/2
        printf("buddy = %d\n\n", buddy);
        int old_offset = heap_ptr[1]; //save the old offset
        heap_ptr[0] = buddy; //set the size of the current block
        heap_ptr[1] = buddy; //set the offset to point to the buddy

        int buddy_offset = buddy/4;
        heap_ptr[buddy_offset] = buddy; //set size of buddy block
        if(old_offset == 0) 
            heap_ptr[buddy_offset + 1] = 0;
        else                      
            heap_ptr[buddy_offset + 1] = old_offset - buddy;   
        return gronk(heap_ptr, alloc_size);
    }
}

void *malloc(size_t request_size)
{
    void *alloc_ptr;
    // if heap_begin is NULL, then this must be the first
    // time that malloc has been called.  ask for a new
    // heap segment from the OS using mmap and initialize
    // the heap begin pointer.   
    if (!heap_begin)
    {
        heap_begin = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        atexit(dump_memory_map);
        //initialize first header
        free_list = heap_begin;
        free_list[0] = HEAPSIZE;
        free_list[1] = 0; //0 for last free block
        
    }
    
    request_size = request_size + 8;
    int alloc_size = MINIMUM_ALLOC;

    while(alloc_size < request_size)
        alloc_size *= 2;    
    
    alloc_ptr = gronk(free_list, alloc_size);
    return alloc_ptr;    
}

void free(void *memory_block)
{

}

void dump_memory_map(void) {
    int *heap_ptr = heap_begin;
    int *free_ptr = free_list;
    int offset = 0;
    printf("~~~~~~~~~~~~~~~~~~~~Memory Dump~~~~~~~~~~~~~~~~~~~~\n");
    if (heap_ptr != free_ptr)		//checks for allocated memory before the freelist pointer
    {
	int size = 0;
	while (heap_ptr != free_ptr)
	{
	    size += heap_ptr[0];
	    heap_ptr += (heap_ptr[0]/4);
	}
	printf("Block size: %d, offset %d, allocated\n", size, offset);
	offset = size;
    }
    while (free_ptr[1] != 0) 		//traverses through the free memory list
    {
	int alloc_size = free_ptr[1] - free_ptr[0];
	if (alloc_size != 0)		//takes into account allocated memory that is bypassed in the offset
	{
	    printf("Block size: %d, offset %d, free\n", free_ptr[0], offset);
	    offset += free_ptr[0];
	    printf("Block size: %d, offset %d, allocated\n", alloc_size, offset);
	    offset += alloc_size;
	}
	else				//next block is free memory
	{
	    printf("Block size: %d, offset %d, free\n", free_ptr[0], offset);
	    offset += free_ptr[0];
	}
	free_ptr += (free_ptr[1]/4);
    }
    printf("Block size: %d, offset %d, free\n", free_ptr[0], offset);
    offset += free_ptr[0];
    if (HEAPSIZE > offset)		//checks for allocated memory at the end of the heap,
    {					//after the last free memory space
	int last_size = HEAPSIZE - offset;
	printf("Block size: %d, offset %d, free\n", last_size, offset);
	offset += last_size;
    }
    printf("~~~~~~~~~~~~~~~~~~End Memory Dump~~~~~~~~~~~~~~~~~~\n");
    return;
}


void offset_updater(int *heap_ptr, int old_offset) {
    printf("\ngot to offset_updater\n");
    int *temp_ptr = free_list;
    printf("temp_ptr[0] = %d\n", temp_ptr[0]);
    printf("temp_ptr[1] = %d\n", temp_ptr[1]);
    while ((temp_ptr + temp_ptr[1]) != heap_ptr || temp_ptr[1] == 0)
    {
        printf("entered loop\n");
        if(temp_ptr[1] == 0)
            return;
        temp_ptr += temp_ptr[1]/4;
    }
    if(old_offset == 0)
        temp_ptr[1] = 0;
    else
        temp_ptr[1] += old_offset/4;
    return;
}
