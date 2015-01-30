#include <kernel.h>
#include <i386.h>
#include <proc.h>
#include <mm.h>
#include <multiboot.h>

void init(struct multiboot *mbs)
{
	void *pmpage[8];
	int i;

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
		

	process_init();	
	asm volatile("sti");
	while(1);
}


