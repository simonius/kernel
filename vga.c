#include "include/vga.h"

void clear()
{
	int i;
	for(i = 0; i < 1000; i++)
		*((int *) VGA_FB + i) = 0;
	return;
}

int scroll(int pos)
{
	int i;
	char *fb = (char*)VGA_FB;

	for(i = 160; i < 2000; i++)
		fb[i - 160] = fb[i];


	return pos - 160;
}

void putchar(char c, char color)
{
	static int pos = 0;
	char *fb = (char*)VGA_FB;
	
	if(pos >= 2000)
		pos = scroll(pos);
	

	*(fb + 2 * pos) = c;
	*(fb + 2 * pos + 1) = color;
	pos ++;
}


