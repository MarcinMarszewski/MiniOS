#include "int.h"
#include "../drivers/port.h"

interrupt_t interrupt_handlers[256];

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

void interrupt_handler(registers_t r) {
	*(char*)0xb8000 = '0'+ r.int_no;//temporary test
	interrupt_handlers[r.int_no](r);
}

void register_interrupt_handler(unsigned char n, interrupt_t handler) {
    interrupt_handlers[n] = handler;
}

void interrupt_install() {

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

    set_interrupt_gate(0, (unsigned int)interrupt0);
    set_interrupt_gate(1, (unsigned int)interrupt1);
    set_interrupt_gate(2, (unsigned int)interrupt2);
    set_interrupt_gate(3, (unsigned int)interrupt3);
    set_interrupt_gate(4, (unsigned int)interrupt4);
    set_interrupt_gate(5, (unsigned int)interrupt5);
    set_interrupt_gate(6, (unsigned int)interrupt6);
    set_interrupt_gate(7, (unsigned int)interrupt7);
    set_interrupt_gate(8, (unsigned int)interrupt8);
    set_interrupt_gate(9, (unsigned int)interrupt9);
    set_interrupt_gate(10, (unsigned int)interrupt10);
    set_interrupt_gate(11, (unsigned int)interrupt11);
    set_interrupt_gate(12, (unsigned int)interrupt12);
    set_interrupt_gate(13, (unsigned int)interrupt13);
    set_interrupt_gate(14, (unsigned int)interrupt14);
    set_interrupt_gate(15, (unsigned int)interrupt15);

    set_interrupt(); // Load with ASM
}