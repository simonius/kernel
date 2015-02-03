#include <syscall.h>
#include <i386.h>
#include <vga.h>

void syscall(struct i386_state *cpu)
{
	int i;
	switch(cpu->eax){
	case EXIT:
		kprint("IMPLEMENT EXIT \n");
	break;
	case WRITE:
		for (i = 0; i < cpu->edx; i++)
			putchar(*((char*)cpu->ecx + i), VGA_GREY);
	break;
	default:
		kprint("IMPLEMENT SYSCALL #");
		iprint(cpu->INT);
	}
}
