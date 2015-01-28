#ifndef KERNEL_H
#define KERNEL_H

#define NULL	(void*)0



extern void _kernel_begin;
extern void _kernel_end;

void _reboot(void);

void kprint(char *);
void pprint(void *);
void iprint(int);

#endif
