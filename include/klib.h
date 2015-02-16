#ifndef KLIB_H
#define KLIB_H

#define INIPOOLSIZE 0xffff

#define MEM_MAGIC 0xcc1701E

struct region {
	unsigned int magic;
	unsigned int size;
	struct region *higher;
};

void *kmalloc(unsigned int size);
void *memcpy(void *dest, void *src, unsigned long n);
void kfree(void *);
void panic(char *str);
void klib_init();
#endif
