#ifndef ATA_H
#define ATA_H

#include <i386.h>

void ata_init();


void ata_int(struct i386_state *cpu);

int read_sector(unsigned int sector, void *buf);
int write_sector(unsigned int sector, void *buf);


#endif
