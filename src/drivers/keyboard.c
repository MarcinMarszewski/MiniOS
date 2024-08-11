#include "keyboard.h"

void (*key_interrupts[256])(void);

void keyboard_irq(registers_t r){
	unsigned char scancode = port_byte_in(0x60);

	if(key_interrupts[scancode] != 0){
		key_interrupts[scancode]();
	}
}

void define_key_interrupt(unsigned char scancode, void (*handler)(void)){
	key_interrupts[scancode] = handler;
}