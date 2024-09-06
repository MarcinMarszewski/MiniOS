#include "memory.h"
#include "../drivers/screen.h"

char _dynamic_memory[DYNAMIC_MEMORY_SIZE];
memory_allocation _allocations[MAX_ALLOCATIONS] = {0};
int total_allocations = 0;

void* memory_allocate(size_t size){
	if(size == 0)
		return NULL;
	if(total_allocations >= MAX_ALLOCATIONS-1)
		return NULL;

	if(total_allocations == 0){
		if(size < DYNAMIC_MEMORY_SIZE){
			memory_allocation new_allocation = {_dynamic_memory, size};
			_allocations[0] = new_allocation;
			total_allocations++;
			return new_allocation.address;
		}
		return NULL;
	}

	if((char*)_allocations[0].address - _dynamic_memory >= size){
		memory_allocation new_allocation = {_dynamic_memory, size};
		int i;
		for(i=total_allocations;i>0;i--){
			_allocations[i] = _allocations[i-1];
		}
		_allocations[0] = new_allocation;
		total_allocations++;
		return new_allocation.address;
	}

	int i;
	for(i=0;i<total_allocations-1;i++){
		if(_allocations[i+1].address - _allocations[i].address - _allocations[i].size >=size){
			memory_allocation new_allocation = {_allocations[i].address + _allocations[i].size, size};
			int j;
			for(j=total_allocations;j>i;j--){
				_allocations[j+1] = _allocations[j];
			}
			_allocations[i+1] = new_allocation;
			total_allocations++;
			return new_allocation.address;
		}
	}

	if(_allocations[total_allocations-1].address + _allocations[total_allocations-1].size + size < _dynamic_memory + DYNAMIC_MEMORY_SIZE){
		memory_allocation new_allocation = {_allocations[total_allocations-1].address + _allocations[total_allocations-1].size, size};
		_allocations[total_allocations++] = new_allocation;
		return new_allocation.address;
	}
	return NULL;
}

void memory_free(void* address){
	int i;
	for(i=0;i<total_allocations;i++){
		if(_allocations[i].address == address){
			int j;
			for(j=i;j<total_allocations-1;j++){
				_allocations[j] = _allocations[j+1];
			}
			total_allocations--;
			return;
		}
	}
}

void memory_copy(void* source, void* destination, size_t size){
	int i;
	for(i=0;i<size;i++){
		((char*)destination)[i] = ((char*)source)[i];
	}
}

void memory_allocation_sizedown(void* address, size_t size){
	int i;
	for(i=0;i<total_allocations;i++){
		if(_allocations[i].address == address){
			if(size < _allocations[i].size)
			_allocations[i].size = size;
			return;
		}
	}
}

unsigned char memory_compare(void* a, void* b, size_t size){
	unsigned int i;
	for(i=0;i<size;i++){
		if(((char*)a)[i] != ((char*)b)[i]){
			return 0;
		}
	}
	return 1;
}