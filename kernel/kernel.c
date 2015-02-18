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

unsigned long long ticks = 0;

void init(struct multiboot *mbs)
{
	int i;

	struct ringbuf buf;
	char data[100];
	char *test = {"Hello from ringbuf \n"};
	char a;

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


	ring_init(&buf, sizeof(data), data);
	for (i = 0; test[i] != '\0'; i++)
		ring_write(&buf, &test[i]);

	while(ring_ne(&buf)) {
		ring_read(&buf, &a);
		putchar(a, VGA_BLUE);
	}



	while(1) {
		for (i = 0; i < 100; i++)
			idle();
		kprint("idled for 100 ticks, ");
		iprint(ticks); kprint(" ticks since boot \n");
	}
}


