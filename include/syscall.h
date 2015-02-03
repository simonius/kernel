#ifndef SYSCALL_H
#define SYSCALL_H

#include<i386.h> 

#define EXIT	0
#define WRITE	1


void syscall(struct i386_state *cpu);

#endif
