#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "drivers/harddrive.h"
#include "lib/filesystem.h"
#include "drivers/speaker.h"


void main(){
	isr_install();
	
	register_interrupt_handler(33, keyboard_irq);

	__cursor_move = 1;

	char buffer[512];
	ata_read_sector(ATA_PRIMARY_IO, GLOBALDIRCETORYADDRESS, buffer);
	if(buffer[0] != 'D'){
		filesystem_init();
	}



	set_frequency(880);
	speaker_enable();

	int i;
	for(i=0;i<1000000;i++){
		asm volatile("nop");
	}
	for(;;){
		asm volatile("nop");
	}
	//speaker_disable();

	/* char buffer[512] = "hello world";

	char* buf = load_buffer(0);
	write_at_cursor(number_to_string((unsigned char)buf[0])); */


	asm volatile("sti");
}