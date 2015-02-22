#include <uart.h>
#include <errno.h>
#include <ringbuf.h>
#include <i386.h>
#include <proc.h>

int uart_flags;
struct ringbuf buf;
char data[COMBUFFER];
struct process *waiting;

void uart_init(unsigned int div)
{
	uart_flags = 0;
	register_isr(COM1_ISR, uart_int);

		outb(COM1 + 1, 0x3);		// activate ISR for Data Available and Transmitter ready
                outb(COM1 + 3, 0x80);		// set DLAB
                outb(COM1, div & 0xff);		// set divisor low byte
                outb(COM1 + 1, div >> 8);	// set divisor high byte
                outb(COM1 + 3, 0x03);		// clear DLAB, set 8 bits as length
	return;
}

int uart_open()
{
	if(uart_flags & COM1_OPEN)
		return -EBUSY;

	uart_flags |= COM1_OPEN;
	ring_init(&buf, sizeof(data), data);
	return 0;
}

int uart_close()
{
	uart_flags &= ~COM1_OPEN;
	return 0;
}

void uart_int(struct i386_state *cpu)
{
	char byte;

	if (uart_rx(&byte))
		ring_owrite(&buf, &byte);

	proc_unblock(&waiting); //unblock rx waiting process
}

int uart_tx(char byte)
{
	if (inb(COM1 + 5) & 0x20) {
		outb(COM1, byte);
		return 1;
	} else {
		return 0;
	}
}

int uart_rx(char *byte)
{
	if (inb(COM1 + 5) &0x01) {
		*byte = inb(COM1);
		return 1;
	}else{
		return 0;
	}
}

int uart_read(void *user_buf, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
		ring_read(&buf, user_buf + i);

	return i;
}

int uart_write(void *buf, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++)
		while(uart_tx(*((char*)buf + 1)) == 0)
			proc_block(&waiting);

	return i;
}
