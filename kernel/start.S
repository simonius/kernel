#define MB_MAGIC 0x1badb002
#define MB_FLAGS 0x2 //get mem info
#define MB_CHECKSUM - (MB_MAGIC + MB_FLAGS)

.extern init
.extern lowpagedir

.section .multiboot
.align 4

.int MB_MAGIC
.int MB_FLAGS
.int MB_CHECKSUM

.section .text
.global _loader
.global _reboot
.global pagedir

_loader:
	cli
	mov $kernel_stack, %esp

	mov $0xB8000, %eax
	movb $65, (%eax)

	mov %cr4, %eax // enable PSE
	or $0x10, %eax
	mov %eax, %cr4

	mov $0x83, %eax // write pagetables
	mov %eax, lowpagedir

	mov $lowpagedir, %ecx
	add $0xc00, %ecx
	mov %eax, (%ecx)

	mov $lowpagedir, %ecx
	add $0xffc, %ecx // pagedir as last entry into itself
	mov $lowpagedir, %eax
	and $0xfffff000, %eax
	or $0x03, %eax
	mov %eax, (%ecx)

	mov $lowpagedir, %eax
	mov %eax, %cr3 // set pagedir adress

	mov %cr0, %eax //enable paging
	or $0x80000000, %eax
	mov %eax, %cr0

	mov $_higher_half, %eax
	jmp *%eax

_higher_half:
	push %ebp
	call init

_stop:
	cli
	hlt
	jmp _stop

_reboot:
	int $0xee




.section .bss

.align 4096
pagedir:
.space 4096

.space 8192
kernel_stack: // kernel stack, upper end
