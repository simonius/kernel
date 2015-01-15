#include "include/kernel.h"
#include "include/i386.h"

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

	asm volatile ("int $0x1");
	asm volatile ("int $0x2");
	asm volatile ("int $0x20");
}

