#define ISR(nr) 	.global isr##nr ; isr##nr: pushl $0 ; pushl $##nr ; jmp master_isr
#define ISR_ERROR(nr)	.global isr##nr ;isr##nr: pushl $##nr ; jmp master_isr

.extern handle_interupt
//exception
ISR(0)
ISR(1)
ISR(2)
ISR(3)
ISR(4)
ISR(5)
ISR(6)
ISR(7)
ISR_ERROR(8)
ISR(9)
ISR_ERROR(10)
ISR_ERROR(11)
ISR_ERROR(12)
ISR_ERROR(13)
ISR_ERROR(14)
ISR(15)
ISR(16)
ISR_ERROR(17)
ISR(18)

//IRQs
ISR(32)
ISR(33)
ISR(34)
ISR(35)
ISR(36)
ISR(37)
ISR(38)
ISR(39)
ISR(40)
ISR(41)
ISR(42)
ISR(43)
ISR(44)
ISR(45)
ISR(46)
ISR(47)

//Syscalls
ISR(48)

//Fail
ISR(1864)

master_isr:

	pusha
	
	mov $0x10, %eax //load kernel data segments
	mov %eax, %ds
	mov %eax, %es


	push %esp // put struct ptr on stack 
	call handle_interupt
	mov %eax, %esp //copy ptr to i386_state in SP;
		
	mov $0x23, %eax
	mov %eax, %ds
	mov %eax, %es	

	popa

	add $8, %esp
	
	iret


