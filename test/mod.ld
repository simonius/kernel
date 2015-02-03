ENTRY(main) /*entry point*/

SECTIONS
{
	. = 0x200000;
	
	.text :{
		*(.text)
	}
	.data  : {
		*(.data)
	}
	.rodata : {
		*(.rodata)
	}
	.bss : {
		*(.bss)
	}

}
