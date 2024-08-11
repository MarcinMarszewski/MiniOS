#ifndef SCREEN_H
#define SCREEN_H

#define VMEM (char*)0xb8000
#define MAX_VMEM_ADDRESS 0xb8f9f

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define WHITE_ON_RED 0x4f
#define WHITE_ON_BLACK 0x0f

extern char __text_color;
extern char __cursor_move;

void write_at(char* c, int x, int y);
void write_char_at(char c, int x, int y);
void write_at_cursor(char* c);
void get_cursor(int* x, int* y);
void set_cursor(int x, int y);

#endif