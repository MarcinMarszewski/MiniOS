#include "screen.h"

char __text_color = 0x0f;

void write_char_at(char c, int x, int y){
	char* vmem = VMEM + 2*(y*SCREEN_WIDTH + x);
	if(vmem > MAX_VMEM_ADDRESS){
		asm volatile("int $5");
	}
	vmem[0] = c;
	vmem[1] = __text_color;
}

void write_at(char* c, int x, int y){
	while(*c){
		write_char_at(*c, x, y);
		c++;
		x++;
	}
}

void get_cursor(int* x, int* y){
	unsigned short pos;
	port_byte_out(0x3d4, 14);
	pos = port_byte_in(0x3d5);
	pos = pos << 8;
	port_byte_out(0x3d4, 15);
	pos |= port_byte_in(0x3d5);
	*x = pos % SCREEN_WIDTH;
	*y = pos / SCREEN_WIDTH;
}

void set_cursor(int x, int y){
	unsigned short pos = y*SCREEN_WIDTH + x;
	port_byte_out(0x3d4, 14);
	port_byte_out(0x3d5, pos >> 8);
	port_byte_out(0x3d4, 15);
	port_byte_out(0x3d5, pos & 0xff);
}

void write_at_cursor(char* c){
	int x, y;
	get_cursor(&x, &y);
	write_at(c, x, y);
}

