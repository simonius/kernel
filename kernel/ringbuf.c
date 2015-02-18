/*
 * Copyright (C) 2015 Simon Klein
 *
 */

#include <ringbuf.h>
#include <kernel.h>
#include <proc.h>

void ring_init(struct ringbuf *new, int size, char *buf)
{
	new->data = buf;
	new->size = size;
	new->count = 0;
	new->in = 0;
	new->out = 0;
	new->waiting = NULL;
}

void ring_write(struct ringbuf *buf, char *byte)
{
	while (buf->size <= buf->count)
		proc_block(&(buf->waiting));

	buf->count++;
	buf->data[buf->in] = *byte;
	buf->in = (buf->in + 1) % buf->size;

	if (buf->waiting != NULL);
		proc_unblock(&(buf->waiting));
}

void ring_read(struct ringbuf *buf, char *byte)
{
	while (buf->count < 1)
		proc_block(&(buf->waiting));

	buf->count--;
	*byte = buf->data[buf->out];
	buf->out = (buf->out + 1) % buf->size;

	if (buf->waiting != NULL);
		proc_unblock(&(buf->waiting));
}

void ring_owrite(struct ringbuf *buf, char *byte)
{
	buf->count++;
	buf->data[buf->in] = *byte;
	buf->in = (buf->in + 1) % buf->size;

	if (buf->count > buf->size) {
		buf->out = buf->in;
		buf->count = buf->size;
	}
	proc_unblock(&(buf->waiting));
}

int ring_av(struct ringbuf *buf)
{
	if (buf->count < buf->size)
		return 1;
	else
		return 0;
}

int ring_ne(struct ringbuf *buf)
{
	return buf->count;
}

