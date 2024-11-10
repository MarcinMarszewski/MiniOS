#include "terminal.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"

void key_a(){
	write_at_cursor("a");
}

void key_b(){
	write_at_cursor("b");
}

void key_c(){
	write_at_cursor("c");
}

void key_d(){
	write_at_cursor("d");
}

void key_e(){
	write_at_cursor("e");
}

void key_f(){
	write_at_cursor("f");
}

void key_g(){
	write_at_cursor("g");
}

void key_h(){
	write_at_cursor("h");
}

void key_i(){
	write_at_cursor("i");
}

void key_j(){
	write_at_cursor("j");
}

void key_k(){
	write_at_cursor("k");
}

void key_l(){
	write_at_cursor("l");
}

void key_m(){
	write_at_cursor("m");
}

void key_n(){
	write_at_cursor("n");
}

void key_o(){
	write_at_cursor("o");
}

void key_p(){
	write_at_cursor("p");
}

void key_r(){
	write_at_cursor("r");
}

void key_s(){
	write_at_cursor("s");
}

void key_t(){
	write_at_cursor("t");
}

void key_u(){
	write_at_cursor("u");
}

void key_v(){
	write_at_cursor("v");
}

void key_w(){
	write_at_cursor("w");
}

void key_x(){
	write_at_cursor("x");
}

void key_y(){
	write_at_cursor("y");
}

void key_z(){
	write_at_cursor("z");
}

void key_enter(){
	write_at_cursor("\n");
}

void key_space(){
	write_at_cursor(" ");
}

void key_backspace(){
	delete_char();
}

void init_terminal(){
	__cursor_move = 1;
	
	define_key_interrupt(0x1E, key_a);
	define_key_interrupt(0x30, key_b);
	define_key_interrupt(0x2E, key_c);
	define_key_interrupt(0x20, key_d);
	define_key_interrupt(0x12, key_e);
	define_key_interrupt(0x21, key_f);
	define_key_interrupt(0x22, key_g);
	define_key_interrupt(0x23, key_h);
	define_key_interrupt(0x17, key_i);
	define_key_interrupt(0x24, key_j);
	define_key_interrupt(0x25, key_k);
	define_key_interrupt(0x26, key_l);
	define_key_interrupt(0x32, key_m);
	define_key_interrupt(0x31, key_n);
	define_key_interrupt(0x18, key_o);
	define_key_interrupt(0x19, key_p);
	define_key_interrupt(0x13, key_r);
	define_key_interrupt(0x1F, key_s);
	define_key_interrupt(0x14, key_t);
	define_key_interrupt(0x16, key_u);
	define_key_interrupt(0x2F, key_v);
	define_key_interrupt(0x11, key_w);
	define_key_interrupt(0x2D, key_x);
	define_key_interrupt(0x15, key_y);
	define_key_interrupt(0x2C, key_z);
	define_key_interrupt(0x1C, key_enter);
	define_key_interrupt(0x39, key_space);
	define_key_interrupt(0x0E, key_backspace);

	register_interrupt_handler(33, keyboard_irq);
}