/*
 * RingBuffer.c
 *
 *  Created on: Nov 16, 2024
 *      Author: thanh
 */

// RINGBUFFER FUNCTIONS
#include "RingBuffer.h"

void rbuffer_init(volatile ringbuffer_t *rb) {
	__disable_irq();
	rb->in = 0;
	rb->out = 0;
	rb->count = 0;
	__enable_irq();
}

uint8_t rbuffer_count(volatile ringbuffer_t *rb) {
	return rb->count;
}

bool rbuffer_full(volatile ringbuffer_t *rb) {
	return (rb->count == (uint8_t) RingBufferSize);
}

bool rbuffer_empty(volatile ringbuffer_t *rb) {
	return (rb->count == 0);
}

void rbuffer_insert(char data, volatile ringbuffer_t *rb) {
	*(rb->buffer + rb->in) = data;
	__disable_irq();
	rb->in = (rb->in + 1) & ((uint8_t) RingBufferSize - 1);
	rb->count++;
	__enable_irq();
}

char rbuffer_remove(volatile ringbuffer_t *rb) {
	char data = *(rb->buffer + rb->out);
	__disable_irq();
	rb->out = (rb->out + 1) & ((uint8_t) RingBufferSize - 1);
	rb->count--;
	__enable_irq();
	return data;
}
