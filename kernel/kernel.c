/*
 * Copyright (C) 2015 Simon Klein
 *
 */

#include <kernel.h>
#include <klib.h>
#include <i386.h>
#include <proc.h>
#include <mm.h>
#include <multiboot.h>

extern struct proc *runnable;

void init(struct multiboot *mbs)
{
	int *ptrs[250];
	int i;

	kprint("\f");
	kprint("Hello Hardware \n");

	kprint("gdt_init ... ");
	gdt_init();
	kprint("done \n");

	kprint("idt_init ... ");
	idt_init();
	kprint("done \n");

	kprint("klib_init ...");
	klib_init();
	kprint("done");

	kprint("PMM init ... ");
	pmm_init(mbs);
	kprint("done \n");

	vmm_init();

	process_init(mbs);



	kprint("testing klib");

	for (i = 0; i < 100; i++)
		ptrs[i] = kmalloc(4);

	for (i = 0; i < 100; i++)
		*(ptrs[i]) = 0x1864;

	for (i = 0; i < 100; i++)
		kfree(ptrs[i]);

	if (runnable == NULL)
		panic("NO INIT");
	while(1);
}


