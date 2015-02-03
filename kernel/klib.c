#include <klib.h>

void *memcpy(void *dest, void *src, unsigned long n)
{
	int ints = n / 4;
	int rbytes = n % 4;
	int i;

	for(i = 0; i < ints; i ++)
		*((int *)dest + i) = *((int *)src + i);

	for(i = 0; i < rbytes; i++)
		*((char *)dest + i) = *((char *)src + i);

	return dest;
}

void panic(char *str)
{
	kprint(" \n\nCongratulations: A KERNEL PANIC \n\nWhy ? Because:");
	kprint(str);
	kprint("\n");

	while(1);
}
