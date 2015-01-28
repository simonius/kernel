#ifndef MM_H
#define MM_H

#define K *1024
#define M K K
#define G K M

#define PAGESIZE 4096
#define BITMAPEND 16 M

#define BITMAP_I(PTR) (unsigned int)PTR/(PAGESIZE*32)
#define BITMAP_J(PTR) ((unsigned int)PTR/PAGESIZE)%32

void* ppage_alloc();
int ppage_free(void *page);
void pmm_init();
#endif /* MM_H */
