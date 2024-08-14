#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "lib/memory.h"
#include "lib/string.h"

void scancode_w(){
	int x, y;
	get_cursor(&x, &y);
	set_cursor(x, y-1 < 0 ? 0 : y-1);
}

void scancode_s(){
	int x, y;
	get_cursor(&x, &y);
	set_cursor(x, y+1 >= SCREEN_HEIGHT ? SCREEN_HEIGHT-1 : y+1);
}

void scancode_a(){
	int x, y;
	get_cursor(&x, &y);
	set_cursor(x-1 < 0 ? 0 : x-1, y);
}

void scancode_d(){
	int x, y;
	get_cursor(&x, &y);
	set_cursor(x+1 >= SCREEN_WIDTH ? SCREEN_WIDTH-1 : x+1, y);
}

void scancode_q(){
	__text_color = 0xff;
	write_at_cursor(" ");
}

void scancode_e(){
	__text_color = 0x00;
	write_at_cursor(" ");
}

void main(){
	isr_install();
	
	
	define_key_interrupt(0x11, scancode_w);
	define_key_interrupt(0x1f, scancode_s);
	define_key_interrupt(0x1e, scancode_a);
	define_key_interrupt(0x20, scancode_d);
	define_key_interrupt(0x10, scancode_q);
	define_key_interrupt(0x12, scancode_e);

	register_interrupt_handler(33, keyboard_irq);

	__cursor_move = 0;

	char* loads_of_memory = memory_allocate(1024*1024*4);



	char* str1 = number_to_string(123);
	char* str2 = number_to_string(456);
	char* str3 = number_to_string(789);
	char* str4 = number_to_string(789);
	char* str5 = number_to_string(789);

	write_at(number_to_string(loads_of_memory),0,3);

	write_at(str1, 0, 0);
	write_at(str2, 0, 1);
	write_at(str3, 0, 2);

	memory_free(str1);
	memory_free(str2);
	memory_free(str3);
	memory_free(str4);
	memory_free(str5);

	char* str6 = number_to_string(123);

	write_at(number_to_string(str4),0,4);
	write_at(number_to_string(str5),0,5);
	write_at(number_to_string(str6),0,6);

	char* loads_of_memory2 = memory_allocate(1024*32);

	memory_free(loads_of_memory);
	memory_free(loads_of_memory2);
	memory_free(str6);

	write_at(number_to_string(memory_allocate(4)),0,0);


	asm volatile("sti");
}