/*
 * RingBuffer.h
 *
 *  Created on: Nov 16, 2024
 *      Author: thanh
 */

#ifndef UART_RINGBUFFER_H_
#define UART_RINGBUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include <cmsis_gcc.h>	//__disable_irq() && __enable_irq();

#define RingBufferSize 64
// RINGBUFFER STRUCT
typedef struct {
	volatile char buffer[RingBufferSize];
	volatile uint8_t in;
	volatile uint8_t out;
	volatile uint8_t count;
} ringbuffer_t;

void rbuffer_init(volatile ringbuffer_t *rb);
uint8_t rbuffer_count(volatile ringbuffer_t *rb);
bool rbuffer_full(volatile ringbuffer_t *rb);
bool rbuffer_empty(volatile ringbuffer_t *rb);
void rbuffer_insert(char data, volatile ringbuffer_t *rb);
char rbuffer_remove(volatile ringbuffer_t *rb);

#endif /* UART_RINGBUFFER_H_ */
