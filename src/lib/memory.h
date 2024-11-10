#ifndef MEMORY_H
#define MEMORY_H

#define DYNAMIC_MEMORY_SIZE 1024*1024*2
#define MAX_ALLOCATIONS 1024*8
#define NULL 0

typedef unsigned int size_t;

typedef struct
{
	void* address;
	size_t size;
} memory_allocation;

void* memory_allocate(size_t size);
void memory_free(void* address);
void memory_copy(void* source, void* destination, size_t size);
void memory_allocation_sizedown(void* address, size_t size);
unsigned char memory_compare(void* a, void* b, size_t size);
void memory_set(void* address, unsigned char value, size_t size);

#endif