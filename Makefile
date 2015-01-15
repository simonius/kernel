OBJ = kernel.o  start.o vga.o kprint.o klib.o i386.o isr.o
HDR = include/kernel.h include/vga.h include/klib.h include/i386.h


CC = clang
LD = ld
AS = as

ASFLAGS = -m32 -c
CFLAGS =  -c -m32 -O0 -ffreestanding -nostdinc -nostdlib
LDFLAGS = -melf_i386 -Tkernel.ld

kernel: $(OBJ) $(HDR)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)  

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ $^

clean:
	rm *.o

.PHONY: clean 
