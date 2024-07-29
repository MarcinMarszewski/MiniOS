#include "port.h"

void port_byte_out (unsigned short port, unsigned char data) {
    __asm__ __volatile__("out %%al, %%dx" : : "a" (data), "d" (port));
}