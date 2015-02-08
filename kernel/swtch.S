/* function that saves the state of one thread and restarts another 	*
 * switch(void **old_sp, void *new_sp) 					*/

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
