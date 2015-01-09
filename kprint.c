#include "include/kernel.h"
#include "include/vga.h"


void kprint(char* string)
{
	int i;
	for(i = 0; *(string + i) != '\0'; i++)
		putchar(*(string + i), VGA_GREY);	
}
