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
#define ACS_TSS			0b00001001

#define GDT_GRAN_PAGE		0b10000000
#define GDT_GRAN_PM		0b01000000

#define GDT_SEG_CODE	ACS_PRSNT | ACS_MSEG | ACS_CODE | ACS_WRITE
#define GDT_SEG_DATA	ACS_PRSNT | ACS_MSEG | ACS_DATA | ACS_WRITE	
#define GDT_TSS		ACS_PRSNT | ACS_RING3 | ACS_TSS // RING0 ?

#define GDT_LIMIT 6
#define GDT_4GB 0xffffffff
#define CS_KERNEL	1
#define DS_KERNEL	2
#define CS_USER		3
#define DS_USER		4
#define TSS		5

#define ISR_TYP_HW	0x600
#define ISR_TYP_Trap	0x700
#define ISR_TYP_Task	0x500
#define ISR_32		0x800
#define ISR_USER	0x6000
#define ISR_KERNEL	0x0
#define ISR_P		0x8000	

#define ISR_Trap 	ISR_TYP_Trap | ISR_32 | ISR_USER | ISR_P
#define ISR_HW 		ISR_TYP_HW | ISR_32 | ISR_USER | ISR_P

#define IDT_LIMIT	50 
#define ISR(nr)         idt_table[nr]  = idt_entry( (long) isr##nr , CS_KERNEL, ISR_HW)
#define ISR_H(nr)	void isr##nr (void);


struct  table {
	long long *entries;
	int limit;
}; /* structure for GDT and IDT*/

struct i386_state {
	
	int edi;
	int esi;
	int ebp;
	int esp22;
	int ebx;
	int edx;
	int ecx;
	int eax;	
	
	int INT;
	int error;

	//saved by CPU
	int eip;
	int cs;
	int eflags;
	int esp;
	int ss;
};

long long gdt_entry(long base, long limit, char acces, char flags);
void load_gdt(struct table);
void gdt_init();
void load_seg_kernel(int ds, char ring);

long long idt_entry(long offset, int cs, int flags);
void load_idt(struct table);
void idt_init();
struct i386_state *handle_interupt(struct i386_state *);
void outb(short port, char wert);

ISR_H(0)
ISR_H(1)
ISR_H(2)
ISR_H(3)
ISR_H(4)
ISR_H(5)
ISR_H(6)
ISR_H(7)
ISR_H(8)
ISR_H(9)
ISR_H(10)
ISR_H(11)
ISR_H(12)
ISR_H(13)
ISR_H(14)
ISR_H(15)
ISR_H(16)
ISR_H(17)
ISR_H(18)
ISR_H(1864)
ISR_H(32)
ISR_H(33)
ISR_H(34)
ISR_H(35)
ISR_H(36)
ISR_H(37)
ISR_H(38)
ISR_H(39)
ISR_H(40)
ISR_H(41)
ISR_H(42)
ISR_H(43)
ISR_H(44)
ISR_H(45)
ISR_H(46)
ISR_H(47)
ISR_H(48)



#endif

