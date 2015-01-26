#ifndef PROC_H
#define PROC_H
#include "i386.h"

#define TASK_LIMIT 16
#define PROCESS_VALID 0x001
#define PROCESS_RUN 0x002
struct process {
	int flags;
	struct i386_state *cpu;
	char kernel_stack[4096];
	char user_stack[4096];
	struct process *next_runnable;
};

extern struct process *curr_task;
extern struct process tasks[];

void schedule();
struct process* create_process(unsigned int entry);
void process_init();

#endif
