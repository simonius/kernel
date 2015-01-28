#ifndef MULTIBOOT_H
#define MULTIBOOT_H

struct mb_mod {		//structure of a multiboot module
	unsigned int 	mod_start;
	unsigned int 	mod_end;
	char		*mod_name;
	unsigned int	reserved;
};

struct sec_table { // TODO section table structure

};

struct mb_syms {	// multiboot elf symbol table
	unsigned int 	count;
	unsigned int 	size;
	struct sec_tbl	*sections;
	char		**strings;
};

struct mb_mmap {
	unsigned int 	size;
	unsigned long long baseaddr;
	unsigned long long length;
	unsigned int	type;
};

struct multiboot {
	unsigned int 	mbs_flags;
	unsigned int 	mbs_mem_lower;
	unsigned int 	mbs_mem_upper;
	unsigned int 	mbs_bootdevice;
	char 		*mbs_cmdline;
	unsigned int 	mbs_mods_count;
	struct mb_mod 	*mbs_mods_addr;
	struct mb_syms	mbs_syms;
	unsigned int	mbs_mmap_length;
	struct mb_mmap	*mbs_mmap;	
};

#endif
