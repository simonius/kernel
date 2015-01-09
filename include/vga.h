#ifndef VGA_H
#define VGA_H

#define VGA_FB 0xb8000
#define VGA_BLUE 0x01
#define VGA_GREY 0x07
#define VGA_GREEN 0x0A

void clear();
void putchar(char, char);
#endif
