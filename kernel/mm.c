/*
 * Copyright (C) 2015 Simon Klein
 *
 */

#include <mm.h>
#include <kernel.h>
#include <multiboot.h>

unsigned int pm_bitmap[(BITMAPEND / PAGESIZE)/32];
extern struct page_directory lowpagedir;


int ppage_reserve(void *page)
{
	if (BITMAP_I(page) > (sizeof(pm_bitmap) / 4))
		return -2;

	if (pm_bitmap[BITMAP_I(page)] & (1<< BITMAP_J(page)))
		return -1;

	pm_bitmap[BITMAP_I(page)] |= 1 << BITMAP_J(page);
	return 0;
}

void pmm_init(struct multiboot *mbs)
{
	int i;
	void *page;
	struct mb_mmap *mmap, *mmap_end;
	struct mb_mod *mod;

	kprint(" allocating bitmap for "); iprint(BITMAPEND);
	kprint(" Bytes of size "); iprint(sizeof(pm_bitmap));
	kprint(" Bytes\n");

	for (i = 0; i < (sizeof(pm_bitmap) / 4); pm_bitmap[i] = ~0, i++);

	mmap = mbs->mbs_mmap;
	mod = mbs->mbs_mods_addr;
	mmap_end = (void *)mmap + mbs->mbs_mmap_length;

	while(mmap < mmap_end) {
		kprint("BEGIN: ");hexprint(mmap->baseaddr);
		kprint("END: ");hexprint(mmap->length + mmap->baseaddr);

		if (mmap->type == 1){
			kprint(" FREE \n");
			for (i = 0; i < mmap->length; i++)
				if(ppage_free(mmap->baseaddr + (void *)i) < -1)
					break;
		}else{
			kprint(" USED \n");
		}
		mmap = (void*)mmap + mmap->size + 4;
	}

	/* FIXME: MULTIBOOT MODULES ARE UNSUPPORTET !!!
	for (i = 0; i < mbs->mbs_mods_count; i++) {
		ppage_reserve(mod + i);
		for (page = (void*)mod->mod_start; page < (void*)mod->mod_end; page++)
			ppage_reserve(page);
	}
	*/


	//ppage_reserve(mbs);
	//ppage_reserve(mmap);

	page = VTOP(&_kernel_begin);
	while(page < VTOP(&_kernel_end))
		ppage_reserve(page++);

	pm_bitmap[0] |= 0x1; //allocate page 0 for NULL ptr (*fix*)
}

void* ppage_alloc()
{
	int i = 0, j;
	while (pm_bitmap[i] == ~(0)) {
		if (!(++i < (sizeof(pm_bitmap) / 4)))
			return NULL;
	}

	for (j = 0; j < 32; j++)
		if (!(pm_bitmap[i] & (1 << j)))
			break;

	pm_bitmap[i] |= (1 << j);

	return (void *)((i * 32 + j) * PAGESIZE);
}

void* clppage_alloc()
{
	void *ret;
	int i = 0, j;
        while (pm_bitmap[i] == ~(0)) {
                if (!(++i < (sizeof(pm_bitmap) / 4)))
                        return NULL;
        }

        for (j = 0; j < 32; j++)
                if (!(pm_bitmap[i] & (1 << j)))
                        break;

        pm_bitmap[i] |= (1 << j);

        ret = (void *)((i * 32 + j) * PAGESIZE);
	for (i = 0; i < 1024; i++)
		*((int*)ret + i) = 0;

	return ret;
}

int ppage_free(void *page)
{
	if (BITMAP_I(page) > sizeof(pm_bitmap) / 4)
		return -2;


	pm_bitmap[BITMAP_I(page)] &= ~(1<<BITMAP_J(page));
	return 0;
}

void vmm_init()
{
	void *page;
	struct page_table *table;
	int i;

	set_pagedir_entry(0x0, 0x0, 0x0);
	for (page = 0x0; page < KERNEL_BASE; page += 4 K)
		set_pagedir_entry(page, 0x0, 0x0);

	table = valloc();
	for (i = 0; i < 1024; i++) {
		if ((page = 4 K * i + KERNEL_BASE) < &_kernel_end)
			table->entries[i] = page_table_entry(VTOP(page), PAGE_W | PAGE_VALID);
		else
			table->entries[i] = 0;
	}

	set_pagedir_entry(KERNEL_BASE, (void *)(get_table_entry(table) & 0xFFFFF000), PAGE_VALID | PAGE_W);
	reload_pdir(&lowpagedir);
}

void reload_pdir(struct page_directory *dir)
{
	asm volatile (	"mov  %0, %%cr3 \n\t"
			"mov %%cr0, %%eax \n\t"
			"or $0x80000000, %%eax \n\t"
			"mov %%eax, %%cr0 \n\t"
			:: "r"(dir): "eax");
}

unsigned int page_dir_entry(void *page_table, unsigned int flags)
{
	return ((unsigned int)page_table & 0xfffff000) | (flags & 0x7);
}

unsigned int page_table_entry(void *physical, unsigned int flags)
{
	return ((unsigned int)physical & 0xfffff000) | (flags & 0x7);
}


void page_dir_init(struct page_directory *dir)
{
	int i;
	for (i = 0; i < 1024; i++)
		dir->tables[i] = 0;

	dir->tables[1023] = page_dir_entry(dir, PAGE_VALID | PAGE_W);
}

void set_pagedir_entry(void *adress, void *table, int flags)
{
	int entry = PDIR_ENTRY(adress);
	struct page_directory *dir = (struct page_directory *)PAGEDIR_BASE;
	dir->tables[entry] = page_dir_entry(table, flags);
}

void set_table_entry(void *vadress, void *phy, int flags)
{
	struct page_table *tbl = ((struct page_table *)PAGETABLE_BASE) + PDIR_ENTRY(vadress);
	tbl->entries[PTBL_ENTRY(vadress)] = page_table_entry(phy, flags);
	reload_pdir(&lowpagedir); // FIXME: i486 invlpgt
}

unsigned int get_table_entry(void *adress)
{
	struct page_table *tbl = ((struct page_table *)PAGETABLE_BASE) + PDIR_ENTRY(adress);
	return tbl->entries[PTBL_ENTRY(adress)];
}

unsigned int get_pagedir_entry(void *adress)
{
	int entry = PDIR_ENTRY(adress);
	struct page_directory *dir = (struct page_directory *)PAGEDIR_BASE;
	return dir->tables[entry];
}

int map(void *virtual, void *physical, int flags)
{
	if (!(get_pagedir_entry(virtual) & PAGE_VALID))
		set_pagedir_entry(virtual, clppage_alloc(), PAGE_VALID | PAGE_W | PAGE_USER);

	set_table_entry(virtual, physical, flags);
	return 0;
}

void unmap(void *virtual)
{
	void *i;
	int alloc = 0;
	ppage_free((void*)(get_table_entry(virtual) & 0xFFFFF000));
	set_table_entry(virtual, NULL, 0x0);
	for (i = (void*)((unsigned int)virtual & 0xffC00000); i < (virtual + 4 M); i+= 4 K)
		if (PAGE_VALID & get_table_entry(virtual))
			alloc++;

	if (alloc == 0) {
		ppage_free((void *)(get_pagedir_entry(virtual) & 0xFFFFF000));
		set_pagedir_entry(virtual, NULL, 0x0);
	}
}

void *find_kernelp()
{
	unsigned int entry;
	int i;
	void *page;
	for (page = KERNEL_BASE; page < PAGETABLE_BASE; page += 4 M)
		if (entry = get_pagedir_entry(page), (entry & PAGE_VALID) && !(entry & PAGE_HUGE))
			for (i = 0; i < 4 M; i += 4096)
				if (!(get_table_entry(page + i) & PAGE_VALID))
					return page + i;

	for (page = KERNEL_BASE; page < PAGETABLE_BASE; page += 4 M)
		if (!(get_pagedir_entry(page) & PAGE_VALID))
			return page;

	return NULL;
}

void *find_userp()
{
	unsigned int entry;
        int i;
        void *page;
        for (page = (void*)0x1000; page < KERNEL_BASE; page += 4 M)
                if (entry = get_pagedir_entry(page), (entry & PAGE_VALID) && !(entry & PAGE_HUGE))
                        for (i = 0; i < 4 M; i += 4096)
                                if (!(get_table_entry(page + i) & PAGE_VALID))
                                        return page + i;

        for (page = (void*)0x1000; page < KERNEL_BASE; page += 4 M)
                if (!(get_pagedir_entry(page) & PAGE_VALID))
                        return page;

        return NULL;

}

void *valloc()
{
	void *virt;
	virt = find_kernelp();
	map(virt, ppage_alloc(), PAGE_VALID | PAGE_W);
	return virt;
}

void *valloc_u()
{
	void *virt;
	virt = find_userp();
	map(virt, ppage_alloc(), PAGE_VALID | PAGE_W | PAGE_USER);
	return virt;
}

void change_userptbl(struct user_pagedir *old, struct user_pagedir *new)
{
	int i;
	struct user_pagedir *dir = PAGEDIR_BASE;

	for (i = 0; i < PDIR_ENTRY(KERNEL_BASE); i++)
		old->tables[i] = dir->tables[i];

	for (i = 0; i < PDIR_ENTRY(KERNEL_BASE); i++)
		dir->tables[i] = new->tables[i];

	return;
}
