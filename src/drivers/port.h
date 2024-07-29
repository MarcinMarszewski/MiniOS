#ifndef PORT_H
#define PORT_H

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)

void port_byte_out (unsigned short port, unsigned char data);


#endif