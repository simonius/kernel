#ifndef ELF_H
#define ELF_H

#define ELF_MAGIC	(unsigned int)(0x7F | 'E' << 8 | 'L' << 16 | 'F' << 24)
#define ELF_32		1
#define ELF_LE		1
#define ELF_X86		0x03
#define PT_LOAD		1
#define PT_DYNAMIC	2

struct elf_hdr{
	unsigned int 	magic;
	unsigned char	class;
	unsigned char 	endianess;
	unsigned char	version;
	unsigned char	abi;
	unsigned char	abi_version;
	unsigned char 	pad[7];
	unsigned short	typ;
	unsigned short	machine;
	unsigned int	version_2;
	void		*entry;
	unsigned int	phoff;
	unsigned int	shoff;
	unsigned int	flags;
	unsigned short	hsize;
	unsigned short	phentsize;
	unsigned short	phnum;
	unsigned short	shentsize;
	unsigned short	shnum;
	unsigned short	shstrndex;
};

struct elf_phdr{
	unsigned int type;
	unsigned int offset;
	unsigned int vaddr;
	unsigned int paddr;
	unsigned int filesize;
	unsigned int memsize;
	unsigned int flags;
	unsigned int alignment;
};

int load_elf_module(void *module, int *PID);
#endif //ELF_H
