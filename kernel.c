#include "include/kernel.h"
#include "include/i386.h"

void init(void)
{

	kprint("gdt_init ... ");
	gdt_init();
	kprint("done");
	
}

