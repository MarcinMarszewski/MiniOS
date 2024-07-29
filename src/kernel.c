#include "cpu/int.h"

#define VMEM (char*)0xb8000

void main(){
    *VMEM = 'X';
	*(VMEM+1)=0xf8;

	interrupt_install();
	asm volatile("sti");
	
	asm volatile("int $5");
}
