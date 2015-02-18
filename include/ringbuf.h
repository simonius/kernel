#ifndef RINGBUF_H
#define RINGBUF_H

struct ringbuf {
	char *data;
	int size;
	int count;
	int in;
	int out;
	struct process *waiting;
};

void ring_write(struct ringbuf*, char *byte);
void ring_read(struct ringbuf*, char *byte);
void ring_owrite(struct ringbuf*, char *byte);
void ring_init(struct ringbuf*, int size, char *buf);

int ring_av(struct ringbuf*);
int ring_ne(struct ringbuf*);

#endif

