#ifndef MM_H
#define MM_H

#define K *1024
#define M K K
#define G K M

#define PAGESIZE 4096
#define BITMAPEND 64 M

#define BITMAP_I(PTR) (unsigned int)PTR/(PAGESIZE*32)
#define BITMAP_J(PTR) ((unsigned int)PTR/PAGESIZE)%32

#define KERNEL_BASE ((void *)0xC0000000)
#define VTOP(adress) ((void*)(adress - KERNEL_BASE))
#define PTOV(adress) ((void*)(adress + KERNEL_BASE))

#define PDIR_ENTRY(adress) ((unsigned int)adress >> 22)
#define PTBL_ENTRY(adress) (((unsigned int)adress >> 12) & 0x3ff)

#define PAGE_VALID	0x1
#define PAGE_W		0x2
#define PAGE_USER	0x4
#define PAGE_HUGE	0x80

#define PAGETABLE_BASE	((void*)0xFFC00000)
#define PAGEDIR_BASE	((void*)0xFFFFF000)

struct page_table {
	unsigned int entries[1024];
};

struct page_directory {
	unsigned int tables[1024];
};

struct page {
	unsigned int ints[1024];
};

struct user_pagedir {
	unsigned int tables[768];
};

unsigned int page_dir_entry(void *page_table, unsigned int flags);
unsigned int page_table_entry(void *physical, unsigned int flags);

int ppage_reserve(void *page);
void* ppage_alloc();
int ppage_free(void *page);
void pmm_init();

void vmm_init();
int map(void *virtual, void *physical, int flags);
void page_dir_init(struct page_directory *dir);
void reload_pdir(struct page_directory *dir);

void set_pagedir_entry(void *adress, void *table, int flags);
void set_table_entry(void *vadress, void *phy, int flags);

unsigned int get_table_entry(void *vadress);

void *find_kernelp();
void *find_userp();
void *valloc();
void change_userptbl(struct user_pagedir *old, struct user_pagedir *new);

void *valloc();
void *valloc_u();
#endif /* MM_H */
