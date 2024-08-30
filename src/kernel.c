#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "drivers/harddrive.h"
#include "lib/filesystem.h"
#include "drivers/speaker.h"
#include "drivers/timer.h"


void main(){
	isr_install();
	
	register_interrupt_handler(33, keyboard_irq);

	__cursor_move = 1;

	char buffer[512];
	ata_read_sector(ATA_PRIMARY_IO, GLOBALDIRCETORYADDRESS, buffer);
	if(buffer[0] != 'D'){
		filesystem_init();
	}



	set_timer_frequency(1000);

	register_interrupt_handler(32, timer_irq);

	asm volatile("sti");


	char* time = number_to_string(__ticks_elapsed);
	for (;;)
	{
		time = number_to_string(__ticks_elapsed);
		write_at(time,0,0);
		memory_free(time);
	}
	
}