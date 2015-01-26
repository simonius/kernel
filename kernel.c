#include "include/kernel.h"
#include "include/i386.h"
#include "include/proc.h"

void init(void)
{
	kprint("\f");
	kprint("Hello Hardware \n");
	kprint("gdt_init ... ");
	gdt_init();
	kprint("done \n");

	kprint("idt_init ... ");
	idt_init();
	kprint("done \n");
	process_init();	
	asm volatile("sti");
	while(1);
}


