OBJ = kernel.o start.o vga.o kprint.o klib.o i386.o isr.o proc.o mm.o elf.o syscall.o swtch.o \
ringbuf.o 


CC = clang
LD = ld.gold
AS = as

ASFLAGS = -m32 -c -target i586-pc-none-elf
CFLAGS =  -c -m32 -O0 -ffreestanding -nostdinc -nostdlib -I../include -Wall -target i586-pc-none-elf -g 

LDFLAGS = -melf_i386 -Tkernel.ld

kernel: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)  

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ $^

clean:
	rm *.o

.PHONY: clean 
