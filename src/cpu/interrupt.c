#include "interrupt.h"
#include "../drivers/port.h"
#include "interrupt_functions.h"

void set_interrupt_gate(int n, unsigned int handler){
	interrupts[n].low_offset=low_16(handler);
	interrupts[n].sel=KERNEL_CS;
	interrupts[n].always0=0;
	interrupts[n].flags=0x8E;
	interrupts[n].high_offset=high_16(handler);
}

void set_interrupt(){
	interrupt_reg.base=(unsigned int)&interrupts;
	interrupt_reg.limit=INTERRUPT_ENTRIES*sizeof(interrupt_gate_t)-1;
	__asm__ __volatile__("lidtl (%0)" : : "r" (&interrupt_reg));
}

void set_interrupt_ports(){
	port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0); 
}

void assign_interrupts(){
	set_interrupt_gate(0, (unsigned int)empty_interrupt);
	set_interrupt_gate(1, (unsigned int)empty_interrupt);
	set_interrupt_gate(2, (unsigned int)empty_interrupt);
	set_interrupt_gate(3, (unsigned int)empty_interrupt);
	set_interrupt_gate(4, (unsigned int)empty_interrupt);
	set_interrupt_gate(5, (unsigned int)out_of_bounds_interrupt);
	set_interrupt_gate(6, (unsigned int)empty_interrupt);
	set_interrupt_gate(7, (unsigned int)empty_interrupt);
	set_interrupt_gate(8, (unsigned int)empty_interrupt);
	set_interrupt_gate(9, (unsigned int)empty_interrupt);
	set_interrupt_gate(10, (unsigned int)empty_interrupt);
	set_interrupt_gate(11, (unsigned int)empty_interrupt);
	set_interrupt_gate(12, (unsigned int)empty_interrupt);
	set_interrupt_gate(13, (unsigned int)empty_interrupt);
	set_interrupt_gate(14, (unsigned int)empty_interrupt);
	set_interrupt_gate(15, (unsigned int)empty_interrupt);
}