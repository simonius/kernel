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


fd.img: ../simux
	dd if=/dev/zero of=fd.img bs=1k count=1440
	mke2fs fd.img
	mkdir mnt
	sudo mount fd.img mnt
	sudo chmod a+rwx mnt
	mkdir mnt/boot
	cp ../simux mnt/
	extlinux --install mnt/boot
	cp syslinux.cfg mnt/boot/
	sudo umount mnt

clean:
	rm *.o
	rm fd.img -rf mnt

.PHONY: clean 
