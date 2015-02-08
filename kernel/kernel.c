#include <kernel.h>
#include <i386.h>
#include <proc.h>
#include <mm.h>
#include <multiboot.h>

void init(struct multiboot *mbs)
{
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
		

	process_init(mbs);

	kprint("WE GOT TO THE END !");
	while(1);
}


