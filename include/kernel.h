#ifndef KERNEL_H
#define KERNEL_H



extern void _reboot(void);
void init(void);

void kprint(char *);
void pprint(void *);
void iprint(int);

#endif
