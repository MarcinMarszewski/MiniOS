#include "screen.h"

char __text_color = 0x0f;
char __cursor_move = 1;
char __autoscroll = 1;

unsigned char cursor_x = 0;
unsigned char cursor_y = 0;

void write_char_at(char c, int x, int y){
	char* vmem = VMEM + 2*(y*SCREEN_WIDTH + x);
	if(vmem > (char *)MAX_VMEM_ADDRESS){
		asm volatile("int $5");
	}
	vmem[0] = c;
	vmem[1] = __text_color;

	if(__cursor_move)
		set_cursor((x+1)%SCREEN_WIDTH, y + x/SCREEN_WIDTH);
}

void write_at(char* c, int x, int y){
	if(y>=SCREEN_HEIGHT){
		return;
	}
	if(x>=SCREEN_WIDTH){
		return;
	}

	while(*c){
		if(*c=='\n'){
			x = 0;
			y++;
			c++;
		} else {
			write_char_at(*c++, x++, y);
		}
		
		if(x>=SCREEN_WIDTH){
			x = 0;
			y++;
		}

		if(y>=SCREEN_HEIGHT){
			if(__autoscroll){
				memory_copy(VMEM + 2*SCREEN_WIDTH, VMEM, 2*SCREEN_WIDTH*(SCREEN_HEIGHT-1));
				memory_set(VMEM + 2*SCREEN_WIDTH*(SCREEN_HEIGHT-1), 0, 2*SCREEN_WIDTH);
				x=0;
				y=SCREEN_HEIGHT-1;
				set_cursor(x, y);
			} else {
				return;
			}
		}
	}
}

void get_cursor(int* x, int* y){
	*x = cursor_x;
	*y = cursor_y;
}

void set_cursor(int x, int y){
	unsigned short pos = y*SCREEN_WIDTH + x;
	port_byte_out(0x3d4, 14);
	port_byte_out(0x3d5, pos >> 8);
	port_byte_out(0x3d4, 15);
	port_byte_out(0x3d5, pos & 0xff);
	cursor_x = x;
	cursor_y = y;
}

void write_at_cursor(char* c){
	write_at(c, cursor_x, cursor_y);
}

void delete_char(){
	if(cursor_x>=0 || cursor_y>=0){
		if(cursor_x ==0){
			cursor_y--;
			cursor_x=SCREEN_WIDTH-1;
		}else{
			cursor_x--;
		}

		__cursor_move =0;
		write_char_at(" ", cursor_x, cursor_y);
		__cursor_move=1;
	}
}