#include "filesystem.h"
#include "../drivers/harddrive.h"

#include "../drivers/screen.h"

unsigned char buffer[512*ALLOCATIONUNITSIZE] = {0};
unsigned char file_allocation_buffer[512];
unsigned char small_buffer[512];

void filesystem_init(){
	buffer[0] = 'D';
	buffer[1] = 0b00000011; //[..,isLast,isFirst]
	buffer[2] = 0; //next allocation
	buffer[3] = 0; //
	ata_write_sector(ATA_PRIMARY_IO, GLOBALDIRCETORYADDRESS, buffer);
}

char* load_buffer(unsigned short address){
	unsigned char i;
	for(i=0;i<ALLOCATIONUNITSIZE;i++){
		ata_read_sector(ATA_PRIMARY_IO, address+i, buffer+(i*512));
	}
	return buffer;
}

void write_buffer(unsigned short address){
	unsigned char i;
	for(i=0;i<ALLOCATIONUNITSIZE;i++){
		ata_write_sector(ATA_PRIMARY_IO, address+i, buffer+(i*512));
	}
}

//0 - could not allocate
unsigned short allocate_unit(unsigned char filetype, unsigned char flags){ 
	unsigned short i;
	for(i=GLOBALDIRCETORYADDRESS+ALLOCATIONUNITSIZE;i<MAXDIRECTORYADDRESS;i+=ALLOCATIONUNITSIZE){
		ata_read_sector(ATA_PRIMARY_IO, i, file_allocation_buffer);
		if(file_allocation_buffer[0] == 0){
			file_allocation_buffer[0] = filetype;
			file_allocation_buffer[1] = flags; //[..,isLast,isFirst]
			file_allocation_buffer[2] = 0;
			file_allocation_buffer[3] = 0; //short - next allocation
			ata_write_sector(ATA_PRIMARY_IO, i, file_allocation_buffer);
			return i;
		}
	}
	return 0;
}

unsigned short find_file_in_dircetory(char* filename, unsigned short searchDir){
	searchDir = searchDir<GLOBALDIRCETORYADDRESS ? GLOBALDIRCETORYADDRESS : searchDir;

	load_buffer(searchDir);

	if(buffer[0] != 'D'){
		return 0;
	}

	while(1){

		int i;
		for(i=1;i<ALLOCATIONUNITSIZE*8;i++){
			int name_search_base=i*64;
			int j;
			for (j = 0; filename[j] != '\0' && filename[j] == buffer[name_search_base + j]; j++){
				
				if (filename[j+1] == '\0' && buffer[name_search_base + j+1] == '\0') {
					unsigned short addr = (buffer[name_search_base + 62]<<8)+buffer[name_search_base + 63];
					return addr;
				}
			}
		}

		if(buffer[1] & 0b00000001){
			return 0;
		}

		load_buffer((buffer[2]<<8)+buffer[3]);
	}
}

void add_file_to_directory(char* filename, unsigned char filetype, unsigned short dir){
	dir = dir<GLOBALDIRCETORYADDRESS ? GLOBALDIRCETORYADDRESS : dir;

	load_buffer(dir);

	if(buffer[0] != 'D'){
		return;
	}

	while(1){

		int i;
		for(i=1;i<ALLOCATIONUNITSIZE*8;i++){
			int name_search_base=i*64;
			if(buffer[name_search_base] == 0){
				int j;
				for(j=0;filename[j] != '\0' && j < 61;j++){
					buffer[name_search_base+j] = filename[j];
				}
				buffer[name_search_base+j] = '\0';
				unsigned short new_allocation = allocate_unit(filetype,0b00000011);
				buffer[name_search_base+62] = new_allocation>>8;
				buffer[name_search_base+63] = new_allocation&0xff;
				write_buffer(dir);
				return;
			}
		}

		if(buffer[1] & 0b00000001){
			unsigned short next_unit = allocate_unit('D',0b00000001);
			
			if(next_unit == 0){
				return;
			}
			buffer[1] = buffer[1] & 0b11111110;
			buffer[2] = next_unit>>8;
			buffer[3] = next_unit&0xff;
			write_buffer(dir);
		}
		dir=(buffer[2]<<8)+buffer[3];
		load_buffer(dir);
	}
}


void delete_file_from_directory(char* filename);

//0 - could not allocate, page size = 4032 bytes
char* file_read(unsigned short file, unsigned short page){
	if(page == 0){
		load_buffer(file);
		return buffer+64;
	}
	ata_read_sector(ATA_PRIMARY_IO, file, small_buffer);
	if(small_buffer[1] & 0b00000001){
		return 0;
	}
	return file_read((small_buffer[2]<<8)+small_buffer[3], page-1);
}

void file_write(unsigned short file, char* data, unsigned short len, unsigned short page, unsigned short offset){
	ata_read_sector(ATA_PRIMARY_IO, file, small_buffer);
	if(small_buffer[0] == 'D'){
		return;
	}

	if(page == 0){
		load_buffer(file);
		int i;
		for(i=0;i<len;i++){
			buffer[64+offset+i] = data[i];
		}
		write_buffer(file);
		return;
	}

	if(small_buffer[1] & 0b00000001){
		unsigned short new_allocation = allocate_unit(small_buffer[0],0b00000001);
		small_buffer[1] = small_buffer[1] & 0b11111110;
		small_buffer[2] = new_allocation>>8;
		small_buffer[3] = new_allocation&0xff;
		ata_write_sector(ATA_PRIMARY_IO, file, small_buffer);
		file_write(new_allocation, data, len, page-1, offset);
		return;
	}

	file_write((small_buffer[2]<<8)+small_buffer[3], data, len, page-1, offset);
}