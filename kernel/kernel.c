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

#include <ringbuf.h>
#include <vga.h>

extern struct proc *runnable;

void myisr(struct i386_state* cpu)
{
	kprint("test isr fired");
}

void init(struct multiboot *mbs)
{
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
	kprint("done \n");

	kprint("PMM init ... ");
	pmm_init(mbs);
	kprint("done \n");

	vmm_init();

	process_init(mbs);

	register_isr(1, myisr);
	asm volatile("int $0x21 \n\t");


	while(1) {
		for (i = 0; i < 100; i++)
			idle();
		kprint("idled for 100 ticks, ");
		iprint(ticks); kprint(" ticks since boot \n");
	}
}


