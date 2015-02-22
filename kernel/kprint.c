/*
 * Copyright (C) 2015 Simon Klein
 *
 */

#include "kernel.h"
#include "vga.h"
#include "uart.h"

void puthexdig(int hex);

void puthexdig(int hex)
{
	char hexletters[] = "0123456789abcdef";
	if(hex > 15)
		return ;
	putchar(hexletters[hex], VGA_GREEN);
	uart_tx(hexletters[hex]);
	return;
}

void kprint(char* string)
{
	int i;
	for(i = 0; *(string + i) != '\0'; i++) {
		putchar(*(string + i), VGA_GREY);
		uart_tx(*(string + i));
	}
	return;
}

void pprint(void* ptr)
{
	int i;
	long adress = (long)ptr;
	kprint("0x");
	for(i = 7; i >= 0; i--)
		puthexdig((adress >> 4*i) & 0xf);
	return;
}

void iprint(int number)
{
	long i;
	for(i = 1000000000; i > 0; i = i / 10){
		puthexdig(number / i);
		number = number % i;
	}
	return;
}

void hexprint(int number)
{
	int i;
	kprint("0x");
	for (i = 7; i>=0; i--)
		puthexdig((number >> 4*i) & 0xf);
	return;
}
