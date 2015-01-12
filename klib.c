#include "include/klib.h"

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
