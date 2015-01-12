	.file	"i386.c"
	.text
	.globl	load_seg_kernel
	.align	16, 0x90
	.type	load_seg_kernel,@function
load_seg_kernel:                        # @load_seg_kernel
# BB#0:
	movl	4(%esp), %ecx
	shll	$3, %ecx
	movzbl	8(%esp), %eax
	andl	$2, %eax
	orl	%ecx, %eax
	#APP
	mov %ax, %ds 
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss 
	
	#NO_APP
	#APP
	ljmp $0x08, $1f  
	1: 
	
	#NO_APP
	ret
.Ltmp0:
	.size	load_seg_kernel, .Ltmp0-load_seg_kernel

	.globl	load_gdt
	.align	16, 0x90
	.type	load_gdt,@function
load_gdt:                               # @load_gdt
# BB#0:
	subl	$8, %esp
	movl	12(%esp), %eax
	movl	16(%esp), %ecx
	leal	65535(,%ecx,8), %ecx
	movw	%cx, 2(%esp)
	movw	%ax, 4(%esp)
	shrl	$16, %eax
	movw	%ax, 6(%esp)
	#APP
	lgdtw 2(%esp)
	#NO_APP
	addl	$8, %esp
	ret
.Ltmp1:
	.size	load_gdt, .Ltmp1-load_gdt

	.globl	gdt_entry
	.align	16, 0x90
	.type	gdt_entry,@function
gdt_entry:                              # @gdt_entry
# BB#0:
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	movl	16(%esp), %edx
	movl	20(%esp), %ecx
	movzwl	%cx, %eax
	movl	%edx, %esi
	shll	$16, %esi
	orl	%esi, %eax
	sarl	$31, %esi
	movsbl	24(%esp), %edi
	shll	$8, %edi
	andl	$983040, %ecx           # imm = 0xF0000
	movsbl	28(%esp), %ebx
	shll	$16, %ebx
	andl	$-16777216, %edx        # imm = 0xFFFFFFFFFF000000
	orl	%esi, %edx
	orl	%edi, %edx
	orl	%ecx, %edx
	orl	%ebx, %edx
	popl	%esi
	popl	%edi
	popl	%ebx
	ret
.Ltmp2:
	.size	gdt_entry, .Ltmp2-gdt_entry

	.globl	gdt_init
	.align	16, 0x90
	.type	gdt_init,@function
gdt_init:                               # @gdt_init
# BB#0:
	subl	$6, %esp
	movl	$0, gdt_table+4
	movl	$0, gdt_table
	movl	$gdt_table, %eax
	movl	$-26112, gdt_table+12   # imm = 0xFFFFFFFFFFFF9A00
	movl	$65535, gdt_table+8     # imm = 0xFFFF
	movl	$-28160, gdt_table+20   # imm = 0xFFFFFFFFFFFF9200
	movl	$65535, gdt_table+16    # imm = 0xFFFF
	movl	$-1536, gdt_table+28    # imm = 0xFFFFFFFFFFFFFA00
	movl	$65535, gdt_table+24    # imm = 0xFFFF
	movl	$-3584, gdt_table+36    # imm = 0xFFFFFFFFFFFFF200
	movl	$65535, gdt_table+32    # imm = 0xFFFF
	movl	$6, gdt+4
	movl	$gdt_table, gdt
	movw	$47, (%esp)
	movw	%ax, 2(%esp)
	shrl	$16, %eax
	movw	%ax, 4(%esp)
	#APP
	lgdtw (%esp)
	#NO_APP
	movl	$16, %eax
	#APP
	mov %ax, %ds 
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss 
	
	#NO_APP
	#APP
	ljmp $0x08, $1f  
	1: 
	
	#NO_APP
	addl	$6, %esp
	ret
.Ltmp3:
	.size	gdt_init, .Ltmp3-gdt_init

	.type	gdt_table,@object       # @gdt_table
	.comm	gdt_table,48,8
	.type	gdt,@object             # @gdt
	.comm	gdt,8,4

	.ident	"Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)"
	.section	".note.GNU-stack","",@progbits
