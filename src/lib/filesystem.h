#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define GLOBALDIRCETORYADDRESS 128
#define ALLOCATIONUNITSIZE 8

#define MAXDIRECTORYADDRESS 1024*64

void filesystem_init();

unsigned short find_file_in_dircetory(char* filename, unsigned short searchDir);
void add_file_to_directory(char* filename, unsigned char filetype, unsigned short dir);

char* file_read(unsigned short file, unsigned short page);

void file_write(unsigned short file, char* data, unsigned short len, unsigned short page, unsigned short offset);

#endif