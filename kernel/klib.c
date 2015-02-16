/*
 * Copyright (C) 2015 Simon Klein
 *
 */

#include <klib.h>
#include <kernel.h>
#include <mm.h>

struct region *adress = NULL;
char init_m_pool[INIPOOLSIZE];

void add_free_region(void *begin, unsigned int size);
void concat(struct region **start);

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

void *kmalloc(unsigned int size)
{
	struct region **ptr = &adress, *new, *ret;
	void *free;

	size = size + sizeof(struct region);

	while (*ptr != NULL){
		if ((*ptr)->size >= size)
			break;
		ptr = &((*ptr)->higher);
	}

	if (*ptr == NULL){
		free = valloc((size >> 10) + 1);
		if (free == NULL)
			panic("not enough free virtual pages");
		add_free_region(free, ((size >> 10) + 1) << 10);
	}

	ptr = &adress;

	while (*ptr != NULL){
		if ((*ptr)->size >= size)
			break;
		ptr = &((*ptr)->higher);
	}

	if (*ptr == NULL)
		panic("malloc failed on second try !!!");


	if ((*ptr)->size >= (size + sizeof(struct region) + 4)) {
		new = (void*)*ptr + size;
		new->magic = (*ptr)->magic;
		new->size = (*ptr)->size - size;
		new->higher = (*ptr)->higher;
		ret = (*ptr) + 1;
		(*ptr)->size = size;
		*ptr = new;
	} else {
		ret = *ptr + 1;
		*ptr = (*ptr)->higher;
	}

	return ret;
}

void kfree(void *ptr)
{
	struct region *reg = ptr - sizeof(struct region);
	if (reg->magic != MEM_MAGIC)
		panic("INVALID KFREE");

	add_free_region(reg, reg->size);
	concat(&adress);
}


void add_free_region(void *begin, unsigned int size)
{
	struct region **ptr = &adress;
	struct region *reg = begin;
	reg->magic = MEM_MAGIC;
	reg->size = size;

	while (*ptr < reg && *ptr != NULL)
		ptr = &((*ptr)->higher);

	reg->higher = *ptr;
	*ptr = reg;
}

void concat(struct region **start)
{
	struct region **ptr;
start:
	ptr = start;
	while ((*ptr)->higher != NULL) {
		if (((void*)(*ptr) + (*ptr)->size) == (*ptr)->higher){
			(*ptr)->size += (*ptr)->higher->size;
			(*ptr)->higher = (*ptr)->higher->higher;
			goto start;
		}
	ptr = &((*ptr)->higher);
	}
}


void klib_init()
{
	add_free_region(init_m_pool, sizeof(init_m_pool));
}

void debug_print_struct()
{
	struct region **ptr = &adress;
	while (*ptr != NULL) {
		kprint("("); pprint(*ptr);kprint(".size = "); hexprint((*ptr)->size);kprint(" )->");
		ptr = &((*ptr)->higher);
	}
	kprint("NULL");
}
