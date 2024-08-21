#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "drivers/harddrive.h"
#include "lib/filesystem.h"

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
	
	/* define_key_interrupt(0x11, scancode_w);
	define_key_interrupt(0x1f, scancode_s);
	define_key_interrupt(0x1e, scancode_a);
	define_key_interrupt(0x20, scancode_d);
	define_key_interrupt(0x10, scancode_q);
	define_key_interrupt(0x12, scancode_e);

	register_interrupt_handler(33, keyboard_irq);

	__cursor_move = 0; */


	filesystem_init();

	int i;
	add_file_to_directory("file1", 'F', 0);

	unsigned short file = find_file_in_dircetory("file1", GLOBALDIRCETORYADDRESS);
	
	file_write(file, "hello world", 11, 0, 0);
	file_write(file, "hello world", 11, 0, 11);
	file_write(file, "hello world", 11, 1, 0);
	write_at_cursor(file_read(file, 0));
	write_at_cursor(file_read(file, 1));
	/* char buffer[512] = "hello world";

	char* buf = load_buffer(0);
	write_at_cursor(number_to_string((unsigned char)buf[0])); */


	asm volatile("sti");
}