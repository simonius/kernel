#ifndef I386_H
#define I386_H

#define ACS_PRSNT		0b10000000

#define ACS_RING0		0b0
#define ACS_RING1		0b00100000
#define ACS_RING2		0b01000000
#define ACS_RING3		0b01100000

#define ACS_MSEG		0b00010000
#define ACS_CODE		0b00001000
#define ACS_DATA		0b0
#define ACS_WRITE		0b00000010

#define GDT_GRAN_PAGE		0b10000000
#define GDT_GRAN_PM		0b01000000

#define GDT_SEG_CODE	ACS_PRSNT | ACS_MSEG | ACS_CODE | ACS_WRITE
#define GDT_SEG_DATA	ACS_PRSNT | ACS_MSEG | ACS_DATA | ACS_WRITE	

#define GDT_LIMIT 6
#define GDT_4GB 0xffffffff

struct table {
	long long *entries;
	int limit;
}; /* structure for GDT and IDT*/

long long gdt_entry(long base, long limit, char acces, char flags);
void load_gdt(struct table);
void gdt_init();
void load_seg_kernel(int ds, char ring);

#endif

