#include "include/mm.h"
#include "include/kernel.h"
#include "include/multiboot.h"

unsigned int pm_bitmap[(BITMAPEND / PAGESIZE)/32];

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
	
	kprint(" allocating bitmap for "); iprint(BITMAPEND);
	kprint(" Bytes of size "); iprint(sizeof(pm_bitmap));
	kprint(" Bytes\n");

	for (i = 0; i < (sizeof(pm_bitmap) / 4); pm_bitmap[i] = ~0, i++);

	mmap = mbs->mbs_mmap;
	mmap_end = (void *)mmap + mbs->mbs_mmap_length;
	while(mmap < mmap_end) {
		kprint("BEGIN: ");pprint(mmap->baseaddr);
		kprint("END: ");pprint(mmap->length + mmap->baseaddr);
		
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

	ppage_reserve(mbs);
	ppage_reserve(mmap);

	page = & _kernel_begin;
	while(page < & _kernel_end)
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

int ppage_free(void *page)
{
	if (BITMAP_I(page) > sizeof(pm_bitmap) / 4)
		return -2;


	pm_bitmap[BITMAP_I(page)] &= ~(1<<BITMAP_J(page));
	return 0;
}
