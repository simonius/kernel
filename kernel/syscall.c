/*
 * Copyright (C) 2015 Simon Klein
 *
 */

#include <syscall.h>
#include <i386.h>
#include <vga.h>
#include <kernel.h>
#include <proc.h>

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
	proc_restart(curr_task);
}
