OBJ = kernel.o  start.o vga.o kprint.o klib.o i386.o isr.o proc.o mm.o


CC = clang
LD = ld.gold
AS = as

ASFLAGS = -m32 -c -target i586-pc-none-elf
CFLAGS =  -c -m32 -O0 -ffreestanding -nostdinc -nostdlib -I../include -Wall -target i586-pc-none-elf

LDFLAGS = -melf_i386 -Tmod.ld

mod.mod: mod.o
	$(LD) $(LDFLAGS) -o $@ $^  

mod.o: mod.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm *.o

.PHONY: clean 
