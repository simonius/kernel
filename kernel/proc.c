/*
 * Copyright (C) 2015 Simon Klein
 *
 */

/* proc.c - scheduling and other process managment*/

#include <proc.h>
#include <klib.h>
#include <kernel.h>
#include <mm.h>
#include <multiboot.h>
#include <elf.h>
#include <i386.h>

struct process *curr_task = NULL;
struct process *runnable = NULL;
struct process tasks[TASK_LIMIT];




void print_task(struct process *task)
{
	kprint("Task: \n Kernel Stack:"); pprint(task->kernel_stack);
	kprint("User Stack:"); pprint(task->user_stack);
	kprint("\nCPU State:"); state_print(task->cpu);
	kprint("\nFlags:");
	if (task->flags & PROCESS_RUN)
		kprint("RUNNABLE");
	if (task->flags & PROCESS_VALID)
		kprint("VALID");
	return;
}

void add_runlist(struct process *proc)
{
	struct process **ptr = &runnable;
	while (*ptr != NULL)
		ptr = &((*ptr)->next);
	*ptr = proc;
	proc->next = NULL;
}

void schedule()
{
	struct process *old, *new;
	if (curr_task->ticks-- < 1){
		curr_task->ticks = 3;
		add_runlist(curr_task);	//rewrite scheduler
		old = curr_task;
		if (runnable == NULL)
			panic("no runnable process");
		curr_task = runnable;
		new = curr_task;
	}

	if (!(curr_task->flags & PROCESS_RUN)){
		old = curr_task;
		if (runnable == NULL)
			panic("no runnable process");
		curr_task = runnable;
		new = curr_task;
	}

	if (new != old)
		context_switch(old, new);
}


int create_process(void *entry)
{
	int i, j;
	for (i = 0; i < TASK_LIMIT; i++)
		if (0 == (tasks[i].flags & PROCESS_VALID))
			break;

	kprint("Creating Process: ");iprint(i); kprint("\n");
	tasks[i].flags = PROCESS_VALID | PROCESS_RUN;

	tasks[i].kernel_stack = ppage_alloc();
	tasks[i].user_stack = ppage_alloc();

	tasks[i].cpu = &(tasks[i].kernel_stack[4095]) - sizeof(struct i386_state);

	for (j = 0; j < sizeof(struct i386_state); j++)
		*((char*)tasks[i].cpu + j) = 0;

	tasks[i].cpu->esp = &(tasks[i].user_stack[4095]);
	tasks[i].cpu->eflags = 0x206;
	tasks[i].cpu->cs = 0x18 | 0x03;
	tasks[i].cpu->ss = 0x20 | 0x03;
	tasks[i].cpu->eip = entry;
	tasks[i].next = NULL;
	tasks[i].ticks = 10;
	return i;
}

void process_init(struct multiboot *mbs)
{
	int PID;
	tasks[0].flags = PROCESS_VALID;
	curr_task = &tasks[0];

	for (PID = 1; PID < TASK_LIMIT; PID++)
		tasks[PID].flags = 0;

}

void proc_unblock(struct process **waiting)
{
	struct process **ptr;
	ptr = &runnable;
	while (*ptr != NULL)
		ptr = &((*ptr)->next);

	*ptr = *waiting;
	waiting = &((*waiting)->next);
	(*ptr)->next = NULL;
	(*ptr)->flags |= PROCESS_RUN;
}

void proc_block(struct process **waiting)
{
	struct process **ptr, *old, *new;
	ptr = waiting;
	while (*ptr != NULL)
		ptr = &((*ptr)->next);

	*ptr = curr_task;
	(*ptr)->next = NULL;
	curr_task->flags &= ~PROCESS_RUN;
	if (runnable == NULL)
		panic("no runnable process");

	new = runnable;
	old = curr_task;
	curr_task = new;
	context_switch(old, new);
}

void context_switch(struct process *old, struct process *new)
{
	change_userptbl(old->pgtbl, new->pgtbl);
	if (new->thread_state == NULL)
		swtch_user(&(old->thread_state), new->cpu);
	else
		swtch(&(old->thread_state), new->thread_state);
	return;
}
