ENTRY(_start) /*entry point*/

SECTIONS
{
	. = 0x100000;
	
	_kernel_begin = ABSOLUTE(.);

	.text : {
		*(.multiboot)
		*(.text)
	}
	.data ALIGN(4096) : {
		*(.data)
	}
	.rodata ALIGN(4096) : {
		*(.rodata)
	}
	.bss ALIGN(4096) : {
		*(.bss)
	}
	. = ALIGN(4096);
	_kernel_end = ABSOLUTE(.);
	
}
