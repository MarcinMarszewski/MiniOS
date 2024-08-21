#include "string.h"
#include "memory.h"

//zero terminated string
char* number_to_string(int number){
	char* str = (char*)memory_allocate(12);

	char i=0, is_negative=number<0?1:0;
	if(is_negative)
		number = -number;
		
	do{
		str[i++] = (number%10) + '0';
		number/=10;
	}while(number!=0);
	if(is_negative)
		str[i++] = '-';

	str[i] = 0;
	int j;
	for(j=0;j<i/2;j++){
		char temp = str[j];
		str[j] = str[i-j-1];
		str[i-j-1] = temp;
	}
	memory_allocation_sizedown(str, i+1);
	return str;
}