ENTRY(loader) /*entry point*/

SECTIONS
{
	. = 0xC0100000;
	loader = _loader - 0xC0000000;
	lowpagedir = pagedir -0xC0000000;
	_kernel_begin = .;

	.text ALIGN(0x1000) : AT(ADDR(.text) - 0xC0000000){
		*(.multiboot)
		*(.text)
	}
	.data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000){
		*(.data)
	}
	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000){
		*(.rodata*)
	}
	.bss ALIGN(0x1000) : AT(ADDR(.bss) - 0xC0000000){
		*(.bss)
	}	
	_kernel_end = .;
}
