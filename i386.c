#include "include/i386.h"
#include "include/kernel.h"

long long gdt_table[GDT_LIMIT];
long long idt_table[IDT_LIMIT];
struct table gdt, idt ;

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
	gdt_table[CS_KERNEL] = gdt_entry(0, GDT_4GB, GDT_SEG_CODE | ACS_RING0, GDT_GRAN_PM | GDT_GRAN_PAGE); 
	gdt_table[DS_KERNEL] = gdt_entry(0, GDT_4GB, GDT_SEG_DATA | ACS_RING0, GDT_GRAN_PM | GDT_GRAN_PAGE);
	gdt_table[CS_USER] = gdt_entry(0, GDT_4GB, GDT_SEG_CODE | ACS_RING3, GDT_GRAN_PM | GDT_GRAN_PAGE);
	gdt_table[DS_USER] = gdt_entry(0, GDT_4GB, GDT_SEG_DATA | ACS_RING3, GDT_GRAN_PM | GDT_GRAN_PAGE);
	gdt.limit = GDT_LIMIT ;
	gdt.entries = (void *)gdt_table;
	load_gdt(gdt);
	load_seg_kernel(DS_KERNEL, 0);
}

long long idt_entry(long offset, int cs, int flags)
{
	long long idt_entry = offset & 0xffff;
        cs = (cs << 3);

	idt_entry |= (cs & 0xffff) << 16;
	idt_entry |= (long long)(flags & 0xffff) << 32;
	idt_entry |= (long long)(offset & 0xffff0000) << 32;
	
	return idt_entry;
}

void handle_interupt(struct i386_state *cpu)
{
	kprint("INT: ");
	iprint(cpu->INT);
	kprint("\nEIP: ");
	pprint((void*) cpu->eip);
	kprint("\n");
}

void load_idt(struct table idt)
{
	short idt_arr[3];
	idt_arr[0] = (idt.limit * 8) - 1;
	idt_arr[1] = (int)idt.entries & 0xffff;
	idt_arr[2] = (int)idt.entries >> 16;

	asm volatile("lidtw %0" :: "m" (idt_arr[0]));
}

void idt_init()
{
	int i;
	// www.lowlevel.eu
	// Master-PIC initialisieren
	outb(0x20, 0x11); // Initialisierungsbefehl fuer den PIC
	outb(0x21, 0x20); // Interruptnummer fuer IRQ 0
	outb(0x21, 0x04); // An IRQ 2 haengt der Slave
	outb(0x21, 0x01); // ICW 4
 
	// Slave-PIC initialisieren
	outb(0xa0, 0x11); // Initialisierungsbefehl fuer den PIC
	outb(0xa1, 0x28); // Interruptnummer fuer IRQ 8
	outb(0xa1, 0x02); // An IRQ 2 haengt der Slave
	outb(0xa1, 0x01); // ICW 4
 
	// Alle IRQs aktivieren (demaskieren)
	outb(0x20, 0x0);
	outb(0xa0, 0x0);
	// end www.lowlevel.eu

	idt_table[0] = 0;
	ISR(0);
	ISR(1);
	ISR(2);
        ISR(3);
        ISR(4);
        ISR(5);
        ISR(6);
        ISR(7);
        ISR(8);
        ISR(9);
        ISR(10);
        ISR(11);
        ISR(12);
        ISR(13);
        ISR(14);
        ISR(15);
        ISR(16);
        ISR(17);
        ISR(18);

	for (i = 19; i < 32; i++)
		idt_table[i + 1] = idt_entry((long)isr1864, CS_KERNEL, ISR_HW);
 	
        ISR(32);
        ISR(33);
        ISR(34);
        ISR(35);
        ISR(36);
        ISR(37);
        ISR(38);
	ISR(39);
        ISR(40);
        ISR(41);
        ISR(42);
        ISR(43);
        ISR(44);
        ISR(45);
        ISR(46);
        ISR(47);
	ISR(48);

	idt.limit = IDT_LIMIT;
	idt.entries = (void *)idt_table;
		
	load_idt(idt);
}

inline void outb(short port, char wert)
{
        asm volatile("outb %0, %1" :: "a"(wert), "d"(port));
}

