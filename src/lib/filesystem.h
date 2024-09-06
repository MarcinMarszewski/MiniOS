#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define GLOBALDIRCETORYADDRESS 128
#define MAXDIRECTORYADDRESS 1024*64


typedef struct descriptor{
	unsigned short segment;
	unsigned short offset;
	unsigned short length;
} descriptor;

typedef struct file{
	unsigned char filetype;

	unsigned short firstSegment;
	unsigned short lastSegment;
	unsigned short lastSegmentOffset;

	unsigned short currentSegment;
	unsigned short currentSegmentOffset;

	struct descriptor* fileDescriptor;
} file;
//should a file descriptor be its own type? probably

unsigned short allocate_unit(unsigned short previousSegment, unsigned short nextSegment);

void write_to_file(file* f, char* data, unsigned short len);

unsigned short read_from_file(file* f, char* data, unsigned short len);

file* create_file_in_directory(file* dir, char* name, unsigned char name_len, unsigned char filetype);

file* open_file_in_directory(file* dir, char* name, unsigned char name_len);


void clear_file_descriptor(descriptor* d);

void delete_file_data(file* f);

void remove_file(file* f);

#endif