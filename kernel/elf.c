#include <elf.h>
#include <proc.h>
#include <klib.h>
#include <kernel.h>
#include <mm.h>
int load_elf_module(void *module, int *PID) //FIXME : check on every magic/size/adrees etc.
{
	struct elf_hdr *hdr = module;
	struct elf_phdr *phdr = NULL;
	int i;

	if (hdr->magic != (ELF_MAGIC)){
		kprint("BAD ELF MAGIC:");
		pprint(hdr->magic);
		kprint("at:");
		pprint(hdr);
		return -1;
	}

	phdr = hdr->phoff + module;
	while (phdr < (hdr->phentsize * hdr->phnum + module + hdr->phoff)){
		memcpy(phdr->paddr, phdr->offset + module, phdr->filesize);
		for (i = 0; i < phdr->memsize; i++)
			ppage_reserve(phdr->paddr + i);

	phdr = (void *)phdr + hdr->phentsize;
	}

	*PID = create_process(hdr->entry);
	return 0;
}
