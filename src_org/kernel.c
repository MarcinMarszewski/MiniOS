#include "cpu/int.h"
#include "drivers/screen.h"

#define VMEM (char*)0xb8000

void keyboard_handler(registers_t regs){
	write_at("Keyboard interrupt", 0, 0);
}

int timer_ticks = 0;
void handler48(registers_t regs){
	write_at("Timer interrupt", timer_ticks++, 1);
}

void main(){
    *VMEM = 'X';
	*(VMEM+1)=0xf8;

	interrupt_install();
	register_interrupt_handler(33, keyboard_handler);
	register_interrupt_handler(32, handler48);
	asm volatile("sti");
	asm volatile("int $32");
	asm volatile("int $33");
	while(1);
}