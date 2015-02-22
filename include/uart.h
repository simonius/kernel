#ifndef UART_H
#define UART_H

#define COM1 0x3f8
#define COM1_OPEN 0x01
#define COMBUFFER 64
#define COM1_ISR 0x04

void uart_init(unsigned int div);

void uart_int();

int uart_open();
int uart_read(void *buf, unsigned int size);
int uart_write(void *buf, unsigned int size);
int uart_close();

int uart_rx(char *byte);
int uart_tx(char byte);

#endif
