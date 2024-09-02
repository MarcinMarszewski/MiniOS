#include "filesystem.h"
#include "../drivers/harddrive.h"

#include "../drivers/screen.h"

unsigned char segment_allocation_buffer[512] = {0};
unsigned char file_read_buffer[512] = {0};

// 0x0001 - empty pointer
unsigned short allocate_unit(unsigned short previousSegment, unsigned short nextSegment){
	if(nextSegment == 0){
		nextSegment = 1;
	} 
	if(previousSegment == 0){
		previousSegment = 1;
	}

	unsigned short i;
	for(i=GLOBALDIRCETORYADDRESS;i<MAXDIRECTORYADDRESS;i++){
		ata_read_sector(ATA_PRIMARY_IO, i, segment_allocation_buffer);
		if(segment_allocation_buffer[511] == 0){
			segment_allocation_buffer[508] = previousSegment>>8;
			segment_allocation_buffer[509] = previousSegment&0xff;
			segment_allocation_buffer[510] = nextSegment>>8;
			segment_allocation_buffer[511] = nextSegment&0xff;
			ata_write_sector(ATA_PRIMARY_IO, i, segment_allocation_buffer);
			return i;
		}
	}
	return 0;
}

// data segment structure:
// [data:508][previous:2][next:2]
// 0x0001 - empty pointer

//TODO: updating file descriptor
void write_to_file(file* f, char* data, unsigned short len){
	ata_read_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
	unsigned short i;
	for(i=0;i<len;i++){
		if((f->currentSegmentOffset+i)%512 == 508){
			unsigned short nextSegment = (file_read_buffer[510]<<8)+file_read_buffer[511];
			if(nextSegment == 1){
				unsigned short newSegment = allocate_unit(f->currentSegment, 1);
				file_read_buffer[510] = newSegment>>8;
				file_read_buffer[511] = newSegment&0xff;

				ata_write_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
				ata_read_sector(ATA_PRIMARY_IO, newSegment, file_read_buffer);

				file_read_buffer[508] = f->currentSegment>>8;
				file_read_buffer[509] = f->currentSegment&0xff;
				
				f->lastSegment = newSegment;
				f->currentSegment = newSegment;
			} else {
				ata_write_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
				ata_read_sector(ATA_PRIMARY_IO, nextSegment, file_read_buffer);
				f->currentSegment = nextSegment;
			}
			f->currentSegmentOffset = 0;
		}
		file_read_buffer[(f->currentSegmentOffset+i)%512] = data[i];
	}

	if(f->currentSegment == f->lastSegment && (f->currentSegmentOffset + i) % 512 > f->lastSegmentOffset){
		f->lastSegmentOffset = (f->currentSegmentOffset+i)%512;
	}

	f->currentSegmentOffset = (f->currentSegmentOffset+i)%512;
	ata_write_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
}

// char* data should be allocated by the caller
// returns the number of bytes read
unsigned short read_from_file(file* f, char* data, unsigned short len){
	ata_read_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
	unsigned short i;
	for(i=0;i<len;i++){
		if(f->currentSegment == f->lastSegment && (f->currentSegmentOffset + i) % 512 > f->lastSegmentOffset){
			return i;
		}

		if((f->currentSegmentOffset+i)%512 == 508){
			unsigned short nextSegment = (file_read_buffer[510]<<8)+file_read_buffer[511];
			if(nextSegment == 1){
				return i;
			} else {
				ata_write_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
				ata_read_sector(ATA_PRIMARY_IO, nextSegment, file_read_buffer);
				f->currentSegment = nextSegment;
			}
			f->currentSegmentOffset = 0;
		}
		data[i] = file_read_buffer[(f->currentSegmentOffset+i)%512];
	}
	f->currentSegmentOffset = (f->currentSegmentOffset+i)%512;
	return i;
}

#define DESCRIPTOR_SIZE 7
// filetype: 0 - directory, 1 - file
// TODO: optimise number of writes
file* create_file_in_directory(file* dir, char* name, char* name_len, unsigned char filetype){
	unsigned short new_unit = allocate_unit(1, 1);
	if(new_unit == 0){
		return 0;
	}

	descriptor* new_descriptor = (descriptor*)memory_allocate(sizeof(descriptor));
	new_descriptor->segment = dir->currentSegment;
	new_descriptor->offset = dir->currentSegmentOffset;
	new_descriptor->length = DESCRIPTOR_SIZE + name_len;

	char descriptor_buffer[DESCRIPTOR_SIZE] = {0};
	descriptor_buffer[0] = name_len + DESCRIPTOR_SIZE;
	descriptor_buffer[1] = filetype;
	descriptor_buffer[2] = new_unit>>8;
	descriptor_buffer[3] = new_unit&0xff;
	descriptor_buffer[4] = new_unit>>8;
	descriptor_buffer[5] = new_unit&0xff;
	descriptor_buffer[6] = 0;

	write_to_file(dir, descriptor_buffer, DESCRIPTOR_SIZE-1);
	write_to_file(dir, name, name_len);
	write_to_file(dir, descriptor_buffer+DESCRIPTOR_SIZE-1,1);

	file* new_file = (file*)memory_allocate(sizeof(file));
	new_file->filetype = filetype;
	new_file->firstSegment = new_unit;
	new_file->lastSegment = new_unit;
	new_file->lastSegmentOffset = 0;
	new_file->currentSegment = new_unit;
	new_file->currentSegmentOffset = 0;
	new_file->fileDescriptor = new_descriptor;

	return new_file;
}