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
	void *page;
	kprint("\f");
	kprint("Hello Hardware \n");

	kprint("gdt_init ... ");
	gdt_init();
	kprint("done \n");

	kprint("idt_init ... ");
	idt_init();
	kprint("done \n");

	kprint("PMM init ... ");
	pmm_init(mbs);
	kprint("done \n");

	vmm_init();

	process_init(mbs);

	kprint("WE GOT TO THE END !, TESTS: \nFinding Free Virtual memory");
	pprint(page = valloc_u());
	kprint("\n");
	pprint(valloc_u());
	kprint("\n");
	pprint(valloc_u());

	*(int*)page = 0x1701;

	if (runnable == NULL)
		panic("NO INIT");
	while(1);
}


