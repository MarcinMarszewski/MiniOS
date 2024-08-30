#include "timer.h"

unsigned int __ticks_elapsed = 0;

void set_timer_frequency(unsigned int frequency){
	unsigned short divisor = PIT_FREQUENCY / frequency;
	port_byte_out(0x43, 0b00110100);
	port_byte_out(0x40, (unsigned char)divisor & 0xFF);
	port_byte_out(0x40, (unsigned char)(divisor >> 8) & 0xFF);
}

void timer_irq(registers_t r){
	__ticks_elapsed++;
}