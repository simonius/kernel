#include <ata.h>
#include <proc.h>
#include <i386.h>
#include <klib.h>
#include <kernel.h>

#define ATA0_DATA 	0x1F0
#define ATA0_ERROR_REG 	0x1f1
#define ATA0_SEC_COUNT	0x1f2
#define ATA0_SEC_NUM	0x1f3
#define ATA0_LBA_L	0x1f3
#define ATA0_CYL_L	0x1f4
#define ATA0_LBA_M	0x1f4
#define ATA0_CYL_H	0x1f5
#define ATA0_LBA_H	0x1f5
#define ATA0_DEV	0x1f6 //TODO last 4 LBA bytes
#define ATA0_STATUS	0x1f7
#define ATA0_COMMAND	0x1f7
#define ATA0_DEV_CTRL	0x3f6

#define NO_ERROR	0x00
#define ATA_PRIM	0xe0
#define ATA_SEC		0xf0
#define ATA_BUSY	0x80
#define ATA_READY	0x40
#define ATA_DREQ	0x08
#define ATA_FLAG	0xe9
#define IRQ_DISABLE	0x02

#define READ 		0x20
#define WRITE 		0x30
#define IDENT_DRV	0xEC

#define SECTOR_SIZE	512

#define TIMEOUT		10

#define _ATA_READ	0x01
#define _ATA_WRITE	0x02
#define ATA0_IRQ	0x0e


struct process *waiting = 0;
unsigned int ata_flags = 0;
short ata_flags_reg();

void ata_init()
{
	unsigned int i = 0;

	outb(ATA0_DEV, ATA_PRIM);

	while (ATA_READY != ata_flags_reg())
		if (i++ > TIMEOUT)
			panic("ATA not READY");

	ata_flags = 0;
	waiting = NULL;

	if (register_isr(ATA0_IRQ, ata_int))
		panic("could not get irq");

	outb(ATA0_DEV_CTRL, 0);
}

void ata_int(struct i386_state *cpu)
{
	proc_unblock(&waiting);
}

int ata_ready()
{
	char status = 0;

	outb(ATA0_DEV, ATA_PRIM);
	status = ata_flags_reg();
	if (status == ATA_READY)
		return 1;
	else
		return 0;
}


void write_cmd_param(char device, unsigned int lba, char cmd)
{
	outb(ATA0_DEV, device);
	outb(ATA0_DEV_CTRL, 0);
	outb(ATA0_SEC_COUNT, 1);
	outb(ATA0_LBA_L, lba & 0xff);
	outb(ATA0_LBA_M, (lba >> 8) & 0xff);
	outb(ATA0_LBA_H, (lba >> 16) & 0xff);
	outb(ATA0_COMMAND, cmd);
}

int read_sector(unsigned int sector, void *buf)
{
	unsigned int i = 0, j = 0;

	while (!ata_ready()) {
		proc_block(&waiting);
		if (i++ > TIMEOUT)
			return 0;
	}

	write_cmd_param(ATA_PRIM, sector, READ);

	for (i = 0; i < SECTOR_SIZE; i+=2) {
		for (j = 0; ata_flags_reg() != (ATA_READY | ATA_DREQ); j++)
			if (j > TIMEOUT)
				return i;
		asm volatile("inw %%dx, %%ax \n\t" : "=a"(*(short*)(buf + i)): "d"(ATA0_DATA):);
	}

	return i;
}

short ata_flags_reg()
{
	char ret = inb(ATA0_STATUS) & ATA_FLAG;
	return ret;
}

int write_sector(unsigned int sector, void *buf)
{
	unsigned int i = 0, j = 0;

	while (!ata_ready()) {
		proc_block(&waiting);
		if (i++ > TIMEOUT)
			return 0;
	}

	write_cmd_param(ATA_PRIM, sector, WRITE);

	for (i = 0; i < SECTOR_SIZE ; i += 2) {
		for (j = 0; ata_flags_reg() != (ATA_READY | ATA_DREQ); j++)
			if (j > TIMEOUT)
				return i;

		asm volatile(" outw %%ax, %%dx \n\t" :: "a"(*(short*)(buf + i)), "d"(ATA0_DATA):);
	}

	return i;
}
