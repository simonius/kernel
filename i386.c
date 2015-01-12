#include "include/i386.h"
#include "include/kernel.h"

long long gdt_table[GDT_LIMIT];
struct table gdt ;

void load_seg_kernel(int ds, char ring)
{
	ds = (ds << 3) | (ring & 0x2);
	asm volatile(
			"mov %%ax, %%ds \n\t"
			"mov %%ax, %%es \n\t"
			"mov %%ax, %%fs \n\t"
			"mov %%ax, %%gs \n\t"
			"mov %%ax, %%ss \n\t"
			:: "a"(ds));
	asm volatile( 	"ljmp $0x08, $1f  \n\t" /*numeric laberl forwar, f suffix*/
			"1: \n\t");

	return;
	

}

void load_gdt(struct table gdt)
{
	short gdt_arr[3];
	gdt_arr[0] = (gdt.limit * 8) - 1;
	gdt_arr[1] = (int)gdt.entries & 0xffff;
	gdt_arr[2] = (int)gdt.entries >> 16;
	asm volatile("lgdtw %0":: "m" (gdt_arr[0])); /* not *exactly* correct*/
}

long long gdt_entry(long base, long limit, char acces, char flags)
{
	long long gdt_entry = limit & 0xffff;
	gdt_entry |= (base & 0xffffff) << 16;
	gdt_entry |= ((long long)acces & 0xff) << 40;
	gdt_entry |= ((long long)limit & 0x0f0000) << 32;
	gdt_entry |= ((long long)flags & 0xf0) << 48;
	gdt_entry |= ((long long)(base  & 0xff000000)) << 32;
	return gdt_entry;
}

void gdt_init()
{
	gdt_table[0] = gdt_entry(0, 0, 0, 0);
	gdt_table[1] = gdt_entry(0, GDT_4GB, GDT_SEG_CODE | ACS_RING0, GDT_GRAN_PM | GDT_GRAN_PAGE); 
	gdt_table[2] = gdt_entry(0, GDT_4GB, GDT_SEG_DATA | ACS_RING0, GDT_GRAN_PM | GDT_GRAN_PAGE);
	gdt_table[3] = gdt_entry(0, GDT_4GB, GDT_SEG_CODE | ACS_RING3, GDT_GRAN_PM | GDT_GRAN_PAGE);
	gdt_table[4] = gdt_entry(0, GDT_4GB, GDT_SEG_DATA | ACS_RING3, GDT_GRAN_PM | GDT_GRAN_PAGE);
	gdt.limit = GDT_LIMIT ;
	gdt.entries = (void *)gdt_table;
	load_gdt(gdt);
	load_seg_kernel(2, 0);
}
