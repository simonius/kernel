#define MB_MAGIC 0x1badb002
#define MB_FLAGS 0x0
#define MB_CHECKSUM - (MB_MAGIC + MB_FLAGS)

.extern init

.section .multiboot
.align 4

.int MB_MAGIC
.int MB_FLAGS
.int MB_CHECKSUM

.section .text
.global _start
.global _reboot
_start:
	cli
	mov $kernel_stack, %esp 
	call init

_stop:
	cli
	hlt
	jmp _stop

_reboot:
	int $0xee

.section .bss
.space 8192
kernel_stack: // kernel stack, upper end
