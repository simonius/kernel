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

void schedule()
{
	if ((curr_task->flags & PROCESS_RUN) == 0){
		curr_task = runnable ;
		return;
 	}

	if (curr_task->next_runnable != NULL){
		curr_task = curr_task->next_runnable;
		return;
	}

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
	tasks[i].next_runnable = NULL;
	return i;
}

void process_init(struct multiboot *mbs)
{
	int PID;
	tasks[0].flags = PROCESS_VALID;
	curr_task = &tasks[0];

	for (PID = 1; PID < TASK_LIMIT; PID++)
		tasks[PID].flags = 0;

	if (mbs->mbs_mods_count < 1)
		panic("No Init Module");

	iprint(mbs->mbs_mods_count);
	kprint(" Multiboot Modules, using one as init: ");
	kprint(mbs->mbs_mods_addr->mod_name);
	kprint("\n");

	if (load_elf_module(mbs->mbs_mods_addr->mod_start, &PID))
		panic("Bad init Module");

	runnable = &tasks[PID];
}
