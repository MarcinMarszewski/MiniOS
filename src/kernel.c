#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "drivers/harddrive.h"
#include "lib/filesystem.h"


void main(){
	isr_install();
	
	register_interrupt_handler(33, keyboard_irq);

	__cursor_move = 1;

	char buffer[512];
	ata_read_sector(ATA_PRIMARY_IO, GLOBALDIRCETORYADDRESS, buffer);
	if(buffer[0] != 'D'){
		filesystem_init();
	}

	add_file_to_directory("test", 'D', GLOBALDIRCETORYADDRESS);
	unsigned short testdir = find_file_in_dircetory("test", GLOBALDIRCETORYADDRESS);

	add_file_to_directory("test2", 'F', testdir);
	unsigned short testfile = find_file_in_dircetory("test2", testdir);
	file_write(testfile, "hello world", 11, 0, 0);
	file_write(testfile, "hello world", 11, 1, 0);

	unsigned short toDelete = delete_file_from_directory("test2", testdir);
	write_at_cursor(number_to_string(toDelete));
	delete_file(toDelete);

	add_file_to_directory("test3", 'F', testdir);

	/* char buffer[512] = "hello world";

	char* buf = load_buffer(0);
	write_at_cursor(number_to_string((unsigned char)buf[0])); */


	asm volatile("sti");
}