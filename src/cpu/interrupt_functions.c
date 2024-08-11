#include "interrupt_functions.h"
#include "../drivers/screen.h"

void empty_interrupt(){
	asm volatile("cli");
	__text_color = WHITE_ON_RED;
	write_at("Interrupt!", 0, 0);
	__text_color = WHITE_ON_BLACK;
}

void out_of_bounds_interrupt(){
	asm volatile("cli");
	__text_color = WHITE_ON_RED;
	write_at("Out of bounds!", 0, 0);
	__text_color = WHITE_ON_BLACK;
}