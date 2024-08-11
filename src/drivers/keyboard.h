#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/isr.h"
#include "port.h"

void keyboard_irq(registers_t r);

void define_key_interrupt(unsigned char scancode, void (*handler)(void));

#endif