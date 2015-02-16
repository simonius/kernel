/*
 * Copyright (C) 2015 Simon Klein
 *
 */


/* function that saves the state of one thread and restarts another 	*
 * swtch(void **old_sp, void *new_sp) 					*/

.global swtch

swtch:
	mov 4(%esp), %eax //new_sp
	mov 8(%esp), %ecx //old_sp saving adress

	push %ebx
	push %ebp
	push %edi
	push %esi

	mov %esp, (%ecx)
	mov %eax, %esp

	pop %esi
	pop %edi
	pop %ebp
	pop %ebx

	ret



/* function that saves the state of a kernel thread and restarts a process in User Space	*/
/* swtch_user(void **old_sp, struct i386_state *user)						*/

.global swtch_user

swtch_user:
	mov 4(%esp), %eax //user process state
	mov 8(%esp), %ecx //kernel sp save adress

	push %ebx
	push %ebp
	push %edi
	push %esi

	mov %esp, (%ecx)
	mov %eax, %esp

	mov $0x23, %eax
	mov %eax, %ds
	mov %eax, %es

	popa

	add $8, %esp

	iret
