#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "drivers/harddrive.h"
#include "lib/filesystem.h"
#include "drivers/speaker.h"
#include "drivers/timer.h"

extern void disk_load();

void main(){
	isr_install();
	
	register_interrupt_handler(33, keyboard_irq);

	__cursor_move = 1;
	__autoscroll = 1;

	/* unsigned short unit_one = allocate_unit(1,1);
	
	file root_dir;
	root_dir.firstSegment = unit_one;
	root_dir.lastSegment = unit_one;
	root_dir.currentSegment = unit_one;
	root_dir.currentSegmentOffset = 0;
	root_dir.fileDescriptor = 0;
	root_dir.filetype = 0; */

	/*file* new_file = create_file_in_directory(&f, "test", 4, 1);
	file* new_file2 = create_file_in_directory(&f, "test2", 5, 1);

	write_to_file(new_file, "Hello, World1", 13);
	write_to_file(new_file2, "Hello, World2", 13);

	

	file* opened_file = open_file_in_directory(&f, "test2", 5);

	delete_file_data(new_file);
	clear_file_descriptor(new_file->fileDescriptor);
	

	file* opened_file2 = open_file_in_directory(&f, "test2", 5);

	int i;
	for(i = 0; i < 100; i++){
		write_to_file(opened_file, "wellno", 6);
	} */

	set_timer_frequency(1000);

	init_terminal();

	register_interrupt_handler(32, timer_irq);

	asm volatile("sti");

	write_at_cursor("Hello, World!\n");
	write_at_cursor("This is a test of the terminal.");
	delete_char();
	delete_char();
	delete_char();

	/* char* time = number_to_string(__ticks_elapsed);
	for (;;)
	{
		time = number_to_string(__ticks_elapsed);
		write_at(time,0,0);
		memory_free(time);
	} */
	
}