#include "filesystem.h"
#include "../drivers/harddrive.h"

#include "../drivers/screen.h"

unsigned char zero_buffer[512] = {0};

unsigned char segment_allocation_buffer[512] = {0};
unsigned char file_read_buffer[512] = {0};

// 0x0001 - empty pointer
unsigned short allocate_unit(unsigned short previousSegment, unsigned short nextSegment){
	if(nextSegment < GLOBALDIRCETORYADDRESS || nextSegment > MAXDIRECTORYADDRESS){
		nextSegment = 1;
	} 
	if(previousSegment < GLOBALDIRCETORYADDRESS || previousSegment > MAXDIRECTORYADDRESS){
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

//TODO: updating file descriptor, if descriptor == 0 dont update
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
// TODO: exclusive file access, max name len check
file* create_file_in_directory(file* dir, char* name, unsigned char name_len, unsigned char filetype){
	if(dir->filetype != 0){
		return 0;
	}
	unsigned short new_unit = allocate_unit(1, 1);
	if(new_unit == 0){
		return 0;
	}

	dir->currentSegment = dir->lastSegment;
	dir->currentSegmentOffset = dir->lastSegmentOffset;

	descriptor* new_descriptor = (descriptor*)memory_allocate(sizeof(descriptor));
	new_descriptor->segment = dir->currentSegment;
	new_descriptor->offset = dir->currentSegmentOffset;
	new_descriptor->length = DESCRIPTOR_SIZE + name_len;

	char descriptor_buffer[DESCRIPTOR_SIZE] = {0};			// for descriptor structure just read this code
	descriptor_buffer[0] = name_len + DESCRIPTOR_SIZE;
	descriptor_buffer[1] = 0;				//last segment offset
	descriptor_buffer[2] = new_unit>>8;
	descriptor_buffer[3] = new_unit&0xff;	//first segment
	descriptor_buffer[4] = new_unit>>8; 	//last segment
	descriptor_buffer[5] = new_unit&0xff;
	descriptor_buffer[6] = filetype;

	write_to_file(dir, descriptor_buffer, 1);
	write_to_file(dir, name, name_len);
	write_to_file(dir, descriptor_buffer+1,DESCRIPTOR_SIZE-1);

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

file* open_file_in_directory(file* dir, char* name, unsigned char name_len){
	if(dir->filetype != 0){
		return 0;
	}

	dir->currentSegment = dir->firstSegment;
	dir->currentSegmentOffset = 0;

	unsigned char descriptor_legth = 0;

	descriptor* new_descriptor = (descriptor*)memory_allocate(sizeof(descriptor));
	new_descriptor->length = name_len + DESCRIPTOR_SIZE;
	while (dir->currentSegment != dir->lastSegment || dir->currentSegmentOffset <= dir->lastSegmentOffset)
	{
		new_descriptor->segment = dir->currentSegment;
		new_descriptor->offset = dir->currentSegmentOffset;
		read_from_file(dir, &descriptor_legth, 1);
		if(descriptor_legth == 0){
			return 0;
		}

		if(descriptor_legth != name_len + DESCRIPTOR_SIZE){
			read_from_file(dir, 0, descriptor_legth-1);
		} else {
			unsigned char* descriptor_compare = (unsigned char*)memory_allocate(name_len + DESCRIPTOR_SIZE-1);
			read_from_file(dir, descriptor_compare, descriptor_legth-1);
			if(memory_compare(descriptor_compare, name, name_len)){
				file* new_file = (file*)memory_allocate(sizeof(file));
				new_file->filetype = descriptor_compare[name_len + DESCRIPTOR_SIZE-2];
				new_file->firstSegment = (descriptor_compare[name_len + DESCRIPTOR_SIZE-4]<<8)+descriptor_compare[name_len + DESCRIPTOR_SIZE-3];
				new_file->lastSegment = (descriptor_compare[name_len + DESCRIPTOR_SIZE-6]<<8)+descriptor_compare[name_len + DESCRIPTOR_SIZE-5];
				new_file->lastSegmentOffset = descriptor_compare[name_len + DESCRIPTOR_SIZE-7];
				new_file->currentSegment = new_file->firstSegment;
				new_file->currentSegmentOffset = 0;
				new_file->fileDescriptor = (new_descriptor);
				return new_file;
			}
		}
	}
	memory_free(new_descriptor);
	return 0;
}

void clear_file_descriptor(descriptor* d){
	file f;
	f.firstSegment = d->segment;
	f.currentSegment = d->segment;
	f.currentSegmentOffset = d->offset;
	f.lastSegment = 0;
	f.lastSegmentOffset = 0;
	f.filetype = 1;
	f.fileDescriptor = 0;

	unsigned char descriptor_length = 0;
	read_from_file(&f, &descriptor_length, 1);
	write_to_file(&f, zero_buffer, descriptor_length-1);
}

void delete_file_data(file* f){
	f->currentSegment = f->firstSegment;
	unsigned short nextSegment = 0;
	while(f->currentSegment >= GLOBALDIRCETORYADDRESS && f->currentSegment <= MAXDIRECTORYADDRESS){
		ata_read_sector(ATA_PRIMARY_IO, f->currentSegment, file_read_buffer);
		nextSegment = (file_read_buffer[510]<<8)+file_read_buffer[511];
		ata_write_sector(ATA_PRIMARY_IO, f->currentSegment, zero_buffer);
		f->currentSegment = nextSegment;
	}
}

void remove_file(file* f){
	clear_file_descriptor(f->fileDescriptor);
	delete_file_data(f);
	memory_free(f->fileDescriptor);
	memory_free(f);
}