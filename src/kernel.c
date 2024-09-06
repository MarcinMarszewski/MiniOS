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

	unsigned short unit_one = allocate_unit(1,1);
	
	file f;
	f.firstSegment = unit_one;
	f.lastSegment = unit_one;
	f.currentSegment = unit_one;
	f.currentSegmentOffset = 0;
	f.fileDescriptor = 0;
	f.filetype = 0;

	file* new_file = create_file_in_directory(&f, "test", 4, 1);
	file* new_file2 = create_file_in_directory(&f, "test2", 5, 1);

	write_to_file(new_file, "Hello, World1", 13);
	write_to_file(new_file2, "Hello, World2", 13);

	

	file* opened_file = open_file_in_directory(&f, "test2", 5);

	int i;
	for(i = 0; i < 40; i++){
		write_to_file(opened_file, "Hello, World2", 13);
	}

	delete_file_data(new_file);
	clear_file_descriptor(new_file->fileDescriptor);
	

	file* opened_file2 = open_file_in_directory(&f, "test2", 5);

	write_to_file(opened_file2, "wellno", 6);

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