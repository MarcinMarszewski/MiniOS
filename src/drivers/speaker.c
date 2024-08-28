#include "speaker.h"
#include "port.h"

void set_frequency(unsigned short frequency){
	unsigned short divisor = PIT_FREQUENCY / frequency;
	port_byte_out(0x43, 0xB6);
	port_byte_out(0x42, (unsigned char)divisor);
	port_byte_out(0x42, (unsigned char)(divisor >> 8));
}

void speaker_enable(){
	port_byte_out(0x61, port_byte_in(0x61) | 3);
}

void speaker_disable(){
	port_byte_out(0x61, port_byte_in(0x61) & 0xFC);
}