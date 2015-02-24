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
#include <uart.h>
#include <ata.h>

extern struct proc *runnable;

void init(struct multiboot *mbs)
{
	int i;
	char sector[512] = {"Hello block\n"};

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

	uart_init(12);
	uart_open();
	ata_init();

	process_init(mbs);

	iprint(write_sector(0, sector));
	iprint(read_sector(1, sector));

	kprint(sector);

	while(1) {
		for (i = 0; i < 100; i++)
			idle();
		kprint("idled for 100 ticks, ");
		iprint(ticks); kprint(" ticks since boot \n");
	}
}


