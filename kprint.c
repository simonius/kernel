#include "include/kernel.h"
#include "include/vga.h"

void puthex(char hex)
{
	char hexletters[] = "0123456789abcdef";
	if(hex > 15)
		return ;
	putchar(hexletters[hex], VGA_GREEN);
	return;
}

void kprint(char* string)
{
	int i;
	for(i = 0; *(string + i) != '\0'; i++)
		putchar(*(string + i), VGA_GREY);	
	return;
}

void pprint(void* ptr)
{
	int i;
	long adress = (long)ptr;
	kprint("0x");
	for(i = 7; i >= 0; i--)
		puthex((adress >> 4*i) & 0xf);
	return;
}

void iprint(int number)
{
	long i;
	for(i = 1000000000; i > 0; i = i / 10){
		puthex(number / i);
		number = number % i;
	}
	return;
}
