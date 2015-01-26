/* proc.c - scheduling and other process managment*/

#include "include/proc.h"
#include "include/klib.h"
#include "include/kernel.h"


struct process *curr_task = NULL;
struct process *runnable = NULL;
struct process tasks[TASK_LIMIT];

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


struct process* create_process(unsigned int entry)
{
	int i, j;
	for (i = 0; i < TASK_LIMIT; i++)
		if (0 == (tasks[i].flags & PROCESS_VALID))
			break;
	
	kprint("Creating Process: ");iprint(i); kprint("\n");
	tasks[i].flags = PROCESS_VALID | PROCESS_RUN;
	tasks[i].cpu = &(tasks[i].kernel_stack[4095]) - sizeof(struct i386_state);

	for (j = 0; j < sizeof(struct i386_state); j++)	
		*((char*)tasks[i].cpu + j) = 0;

	tasks[i].cpu->esp = &(tasks[i].user_stack[4095]);
	tasks[i].cpu->eflags = 0x206;
	tasks[i].cpu->cs = 0x18 | 0x03;
	tasks[i].cpu->ss = 0x20 | 0x03;
	tasks[i].cpu->eip = entry;
	tasks[i].next_runnable = NULL;
	return &tasks[i];
	
}

void task_a()
{
	int i;
	while(1){
		kprint("A");
		for(i = 0 ; i < 10000000; i++); 


	}
}

void task_b()
{
	int i, j;
	for (j = 0; j < 10; j++){
		kprint("B");
		for(i = 0; i < 10000000; i++);
	}

	asm volatile("cli; hlt");

}

void process_init()
{
	int i;
	tasks[0].flags = PROCESS_VALID;
 
	for (i = 1; i < TASK_LIMIT; i++)
		tasks[i].flags = 0;

	runnable = create_process((unsigned int)task_a);
	runnable->next_runnable = create_process((unsigned int)task_b);
	runnable->next_runnable->next_runnable = runnable;
	curr_task = &tasks[0];
}
