#ifndef PROC_H
#define PROC_H

#include "i386.h"
#include "mm.h"

#define TASK_LIMIT 16
#define PROCESS_VALID 0x001
#define PROCESS_RUN 0x002

#include <multiboot.h>

struct process {
	int ticks;
	int flags;
	struct i386_state *cpu;
	void *thread_state;
	void *kernel_stack;
	void *user_stack;
	struct process *next;
	struct user_pagedir *pgtbl;
};

extern struct process *curr_task;
extern struct process tasks[];

void schedule();
int create_process(void *entry);
void process_init(struct multiboot *mbs);

void swtch(void **old, void *new);
void swtch_user(void **old_sp, struct i386_state *user);
void proc_restart(struct process *task);
void context_switch(struct process *old, struct process *new);
#endif
